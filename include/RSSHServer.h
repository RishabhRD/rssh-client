#include "SSHServerDatabase.h"
#include <asio.hpp>
#include <memory>
#include <string>

using asio::ip::tcp;

class Message;

class RSSHServer : public std::enable_shared_from_this<RSSHServer>{
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
  typedef std::shared_ptr<RSSHServer> ptr;
  static ptr create(asio::io_context &, std::string hostname,
                    std::string service, std::string localService);
  void connect();
  void scheduleRead();
  void write(const Message &msg);

private:
  RSSHServer(asio::io_context &, std::string hostname, std::string service,
             std::string localService);
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
