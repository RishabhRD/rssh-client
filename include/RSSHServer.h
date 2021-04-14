#include <asio.hpp>
#include <string>
#include "SSHServerDatabase.h"

using asio::ip::tcp;

class Message;

class RSSHServer {
  asio::io_context &context;
  tcp::socket socket;
  std::string hostname;
  std::string service;
  std::string localService;
  std::vector<std::byte> readBuffer;
  std::uint32_t id;
  std::uint8_t type;
  std::uint32_t length;
  SSHServerDatabase db;

public:
  RSSHServer(asio::io_context &, std::string hostname, std::string service,
             std::string localService);
  void connect();
  void scheduleRead();
  void write(const Message &msg);

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
  void handleMessage(const Message &msg);
};
