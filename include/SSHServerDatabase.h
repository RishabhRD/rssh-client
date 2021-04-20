#pragma once
#include <memory>
#include <cstdint>
#include <unordered_map>
class SSHServer;
class SSHServerDatabase{
  std::unordered_map<std::uint32_t, std::weak_ptr<SSHServer>> mp;
  public:
    SSHServerDatabase();
    void registerServer(std::uint32_t id, std::weak_ptr<SSHServer> client);
    void removeServer(std::uint32_t id);
    std::weak_ptr<SSHServer> getServerFromId(std::uint32_t id) const;
    bool isIDBeingUsed(std::uint32_t id) const noexcept;
    void closeAll();
};
