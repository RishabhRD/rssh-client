#include <iostream>
#include "RSSHServer.h"
#include <asio.hpp>
#include <iostream>
#include <system_error>
using namespace std;
int main() {
  asio::io_context context;
  try{
    RSSHServer server(context, "127.0.0.1", "8080", "443");
    server.connect();
    server.scheduleRead();
    context.run();
  }catch(std::system_error& error){
    cerr<<error.what()<<endl;
  }
}
