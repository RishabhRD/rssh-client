#include <iostream>
#include "RSSHServer.h"
#include <asio.hpp>
#include <iostream>
#include <system_error>
using namespace std;
int main(int argc, char** argv) {
  if(argc != 4){
    std::cerr<<"Usage: rssh-client <ip> <port> <ssh-port>"<<std::endl;
    return 1;
  }
  std::string ip = argv[1];
  std::string port = argv[2];
  std::string sshPort = argv[3];
  asio::io_context context;
  try{
    auto server = RSSHServer::create(context, ip, port, sshPort);
    server->connect();
    server->scheduleRead();
    context.run();
  }catch(std::system_error& error){
    cerr<<error.what()<<endl;
  }
}
