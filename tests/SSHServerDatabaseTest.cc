#include <asio.hpp>
#include "defines.h"
#include <catch2/catch.hpp>
#include "SSHServerDatabase.h"
#include "SSHServer.h"
#include "RSSHServer.h"

tc("After registering server getServerFromId would give the registered server"){
  SSHServerDatabase database;
  asio::io_context context;
  RSSHServer::ptr rsshServer = RSSHServer::create(context, "127.0.0.1", "8080", "443");
  SSHServer::ptr server = SSHServer::create(context, "443", rsshServer, 1);
  database.registerServer(1, server);
  req(database.getServerFromId(1).lock() == server);
}

tc("After removing registered server, asking for it would throw exception"){
  SSHServerDatabase database;
  asio::io_context context;
  RSSHServer::ptr rsshServer = RSSHServer::create(context, "127.0.0.1", "8080", "443");
  SSHServer::ptr server = SSHServer::create(context, "443", rsshServer, 1);
  database.registerServer(1, server);
  database.removeServer(1);
  reqThrows(database.getServerFromId(1));
}

tc("Once registered server with a 1, IDBeingUsed should return true for that"){
  SSHServerDatabase database;
  asio::io_context context;
  RSSHServer::ptr rsshServer = RSSHServer::create(context, "127.0.0.1", "8080", "443");
  SSHServer::ptr server = SSHServer::create(context, "443", rsshServer, 1);
  database.registerServer(1, server);
  req(database.isIDBeingUsed(1) == true);
}

tc("Server registered with a 1 and after remvoe should return false"){
  SSHServerDatabase database;
  asio::io_context context;
  RSSHServer::ptr rsshServer = RSSHServer::create(context, "127.0.0.1", "8080", "443");
  SSHServer::ptr server = SSHServer::create(context, "443", rsshServer, 1);
  database.registerServer(1, server);
  database.removeServer(1);
  req(database.isIDBeingUsed(1) == false);
}

tc("Registering a server with preRegistered1 should throw exception"){
  SSHServerDatabase database;
  asio::io_context context;
  RSSHServer::ptr rsshServer = RSSHServer::create(context, "127.0.0.1", "8080", "443");
  SSHServer::ptr server = SSHServer::create(context, "443", rsshServer, 1);
  database.registerServer(1, server);
  reqThrows(database.registerServer(1, server));
}
