#include "SSHServerDatabase.h"
#include "SSHServer.h"
#include <stdexcept>

SSHServerDatabase::SSHServerDatabase(){};

void SSHServerDatabase::registerServer(std::uint32_t id,
                                    std::weak_ptr<SSHServer> client) {
  if (mp.find(id) != mp.end()) {
    throw std::invalid_argument(
        std::to_string(id) +
        std::string(" id is already registered in database"));
  }
  mp[id] = client;
}

void SSHServerDatabase::removeServer(std::uint32_t id) {
  if (mp.find(id) == mp.end()) {
    throw std::invalid_argument(std::to_string(id) +
                                std::string(" is not registered for remove."));
  }
  mp.erase(id);
}

std::weak_ptr<SSHServer> SSHServerDatabase::getServerFromId(std::uint32_t id) const {
  if (mp.find(id) == mp.end()) {
    throw std::invalid_argument(std::to_string(id) +
                                std::string(" is not registered for get."));
  }
  return mp.find(id)->second;
}

bool SSHServerDatabase::isIDBeingUsed(std::uint32_t id) const noexcept {
  return mp.find(id) != mp.end();
}

void SSHServerDatabase::closeAll(){
  for(auto& keyValuePair : mp){
    auto server = keyValuePair.second.lock();
    server->close();
  }
  mp.clear();
}
