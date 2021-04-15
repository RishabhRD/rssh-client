#include "RSSHServer.h"
#include "CloseMessage.h"
#include "DataMessage.h"
#include "ListenerMessage.h"
#include "NewMessage.h"
#include "SSHServer.h"
#include <iostream>

RSSHServer::RSSHServer(asio::io_context &context, std::string hostname,
                       std::string service, std::string localService)
    : context(context), socket(context), hostname(hostname), service(service),
      localService(localService), readBuffer(1024) {}

void RSSHServer::connect() {
  tcp::resolver resolver(context);
  auto endpoints = resolver.resolve(hostname, service);
  asio::connect(socket, endpoints);
}

void RSSHServer::scheduleRead() { scheduleReadId(); }

void RSSHServer::handleReadId(std::error_code code, std::size_t readSize) {
  if (code == asio::error::eof) {
    handleConenctionClose();
    return;
  }
  scheduleReadType();
}

void RSSHServer::handleReadType(std::error_code code, std::size_t readSize) {
  if (code == asio::error::eof) {
    handleConenctionClose();
    return;
  }
  scheduleReadLength();
}

void RSSHServer::handleReadLength(std::error_code code, std::size_t readSize) {
  if (length > 0) {
    scheduleReadData();
  } else {
    Message msg(convertIntegerToMessageType(type));
    msg.setId(id);
    handleMessage(msg);
  }
}

void RSSHServer::handleReadData(std::error_code code, std::size_t readSize) {
  if (code == asio::error::eof) {
    handleConenctionClose();
    return;
  }
  Message msg(convertIntegerToMessageType(type));
  msg.setId(id);
  msg.fillData(readBuffer.begin(), readBuffer.begin() + readSize);
  handleMessage(msg);
}

void RSSHServer::handleConenctionClose() {}

void RSSHServer::handleMessage(const Message &msg) {
  if (msg.getType() == MessageType::CLOSE) {
    CloseMessage cmsg(msg);
    db.removeServer(msg.getId());
  } else if (msg.getType() == MessageType::DATA) {
    DataMessage cmsg(msg);
    db.getServerFromId(msg.getId()).lock()->write(msg);
    scheduleReadId();
  } else if (msg.getType() == MessageType::LISTEN) {
    ListenerMessage cmsg(msg);
    // TODO: handle the case when this message is listened 2 times
    std::cout << "Port on which is being listened: " << cmsg.getPort()
              << std::endl;
    scheduleReadId();
  } else {
    NewMessage cmsg(msg);
    auto ptr = SSHServer::create(context, localService, shared_from_this(),
                                 cmsg.getId());
    ptr->connect();
    ptr->scheduleRead();
    db.registerServer(cmsg.getId(), ptr);
    scheduleReadId();
  }
}

void RSSHServer::write(const Message &msg) {
  auto data = msg.serialize();
  asio::write(socket, asio::buffer(data));
}

void RSSHServer::scheduleReadId() {
  auto handleReadFp = std::bind(&RSSHServer::handleReadId, shared_from_this(),
                                std::placeholders::_1, std::placeholders::_2);
  asio::async_read(socket, asio::buffer(&id, sizeof(id)), handleReadFp);
}

void RSSHServer::scheduleReadType() {
  auto handleReadFp = std::bind(&RSSHServer::handleReadType, shared_from_this(),
                                std::placeholders::_1, std::placeholders::_2);
  asio::async_read(socket, asio::buffer(&type, sizeof(type)), handleReadFp);
}

void RSSHServer::scheduleReadLength() {
  auto handleReadFp =
      std::bind(&RSSHServer::handleReadLength, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2);
  asio::async_read(socket, asio::buffer(&length, sizeof(length)), handleReadFp);
}

void RSSHServer::scheduleReadData() {
  auto handleReadFp = std::bind(&RSSHServer::handleReadData, shared_from_this(),
                                std::placeholders::_1, std::placeholders::_2);
  asio::async_read(socket, asio::buffer(readBuffer, length), handleReadFp);
}

RSSHServer::ptr RSSHServer::create(asio::io_context &context, std::string hostname,
                                   std::string service,
                                   std::string localService) {
  return ptr(new RSSHServer(context, hostname, service, localService));
}
