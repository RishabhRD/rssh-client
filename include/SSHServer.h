#include <asio.hpp>
class Message;
class RSSHServer;
using asio::ip::tcp;

class SSHServer : public std::enable_shared_from_this<SSHServer> {
  asio::io_context &context;
  RSSHServer &rsshServer;
  std::vector<std::byte> readBuffer;
  tcp::socket socket;
  std::string serviceName;
  uint32_t id;

public:
  typedef std::shared_ptr<SSHServer> ptr;
  static ptr create(asio::io_context &context, std::string serviceName,
             RSSHServer &server, std::uint32_t id);
  void connect();
  void scheduleRead();
  void write(const Message &msg);

private:
  SSHServer(asio::io_context &context, std::string serviceName,
            RSSHServer &server, std::uint32_t id);
  void handleRead(std::error_code code, std::size_t readSize);
  void handleConenctionClose();
};
