#include "ListenerMessage.h"
#include <iostream>
#include "RSSHServer.h"
#include "CloseMessage.h"
#include "NewMessage.h"

RSSHServer::RSSHServer(asio::io_context &context, std::string hostname,
                       std::string service)
    : context(context), socket(context), hostname(hostname), service(service) {}

void RSSHServer::connect() {
  tcp::resolver resolver(context);
  auto endpoints = resolver.resolve(hostname, service);
  asio::connect(socket, endpoints);
}

void RSSHServer::scheduleRead(){
  scheduleReadId();
}

void RSSHServer::scheduleReadId() {
  asio::async_read(socket, asio::buffer(&id, sizeof(id)),
                   [this](auto errorCode, auto readSize) {
                     handleReadId(errorCode, readSize);
                   });
}

void RSSHServer::handleReadId(std::error_code code, std::size_t readSize) {
  if (code == asio::error::eof) {
    handleConenctionClose();
    return;
  }
  scheduleReadType();
}

void RSSHServer::scheduleReadType() {
  asio::async_read(socket, asio::buffer(&type, sizeof(type)),
                   [this](auto errorCode, auto readSize) {
                     handleReadType(errorCode, readSize);
                   });
}

void RSSHServer::handleReadType(std::error_code code, std::size_t readSize) {
  if (code == asio::error::eof) {
    handleConenctionClose();
    return;
  }
  scheduleReadLength();
}

void RSSHServer::scheduleReadLength() {
  asio::async_read(socket, asio::buffer(&type, sizeof(type)),
                   [this](auto errorCode, auto readSize) {
                     handleReadLength(errorCode, readSize);
                   });
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

void RSSHServer::scheduleReadData() {
  asio::async_read(
      socket, asio::buffer(readBuffer, length),
      [this](auto error, auto size) { handleReadData(error, size); });
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

void RSSHServer::handleConenctionClose() { socket.close(); }

void RSSHServer::handleMessage(const Message& msg){
  if(msg.getType() == MessageType::CLOSE){
    CloseMessage cmsg(msg);
    socket.close();
  }else if(msg.getType() == MessageType::DATA){
    // TODO: what to do with data
    scheduleReadId();
  }else if(msg.getType() == MessageType::LISTEN){
    ListenerMessage cmsg(msg);
    // TODO: handle the case when this message is listened 2 times
    std::cout<<"Port on which is being listened: "<<cmsg.getPort()<<std::endl;
    scheduleReadId();
  }else{
    // TODO: handle how to create new client.
    scheduleReadId();
  }
}
