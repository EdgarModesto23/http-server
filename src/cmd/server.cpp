#include "../headers/server.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

http::Server::Server() {
  this->port = "8080";
  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  this->response = http::Response();
  this->request = http::Request();
  this->routes =
      map<string, function<void(http::Response &, http::Request &)>>();
}

http::Server::~Server() { close(this->server_fd); }

http::Server::Server(int port) {
  this->port = to_string(port);
  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  this->response = http::Response();
  this->request = http::Request();
  this->routes =
      map<string, function<void(http::Response &, http::Request &)>>();
}
