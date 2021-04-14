#include "SSHServer.h"
#include "Message.h"
#include "RSSHServer.h"

SSHServer::SSHServer(asio::io_context &context, std::string serviceName,
                     RSSHServer &server, std::uint32_t id)
    : context(context), socket(context), rsshServer(server),
      serviceName(serviceName), readBuffer(1024), id(id) {}

SSHServer::ptr SSHServer::create(asio::io_context &context,
                                 std::string serviceName, RSSHServer &server,
                                 std::uint32_t id) {
  return ptr(new SSHServer(context, serviceName, server, id));
}

void SSHServer::connect() {
  tcp::resolver resolver(context);
  auto endpoints = resolver.resolve("127.0.0.1", serviceName);
  asio::connect(socket, endpoints);
}

void SSHServer::scheduleRead() {
  socket.async_read_some(
      asio::buffer(readBuffer),
      [this](auto error, auto size) { handleRead(error, size); });
}

void SSHServer::handleRead(std::error_code error, std::size_t readSize) {
  if (error == asio::error::eof) {
    handleConenctionClose();
    return;
  }
  Message readMessage(MessageType::DATA);
  readMessage.setId(id);
  readMessage.fillData(readBuffer.begin(), readBuffer.begin() + readSize);
  rsshServer.write(readMessage);
  scheduleRead();
}

void SSHServer::handleConenctionClose() { socket.close(); }

void SSHServer::write(const Message &msg) {
  auto data = msg.serialize();
  asio::async_write(socket, asio::buffer(data),
                    [](auto error, auto sizeWritten) {});
}
