#include <iostream>
#include "RSSHServer.h"
#include <asio.hpp>
#include <iostream>
#include <system_error>
using namespace std;
int main() {
  asio::io_context context;
  try{
    auto server = RSSHServer::create(context, "127.0.0.1", "8080", "22");
    server->connect();
    server->scheduleRead();
    context.run();
  }catch(std::system_error& error){
    cerr<<error.what()<<endl;
  }
}
