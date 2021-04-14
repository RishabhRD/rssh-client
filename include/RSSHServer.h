#include "Message.h"
#include <asio.hpp>
#include <string>

using asio::ip::tcp;

class RSSHServer {
  asio::io_context &context;
  tcp::socket socket;
  std::string hostname;
  std::string service;
  std::vector<std::byte> readBuffer;
  std::uint32_t id;
  std::uint8_t type;
  std::uint32_t length;

public:
  RSSHServer(asio::io_context &, std::string hostname, std::string service);
  void connect();
  void scheduleRead();

private:
  void handleConenctionClose();
  void scheduleReadId();
  void scheduleReadType();
  void scheduleReadLength();
  void scheduleReadData();
  void handleReadId(std::error_code code, std::size_t readSize);
  void handleReadType(std::error_code code, std::size_t readSize);
  void handleReadLength(std::error_code code, std::size_t readSize);
  void handleReadData(std::error_code code, std::size_t readSize);
  void handleMessage(const Message& msg);
};
