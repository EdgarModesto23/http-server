#include "../headers/server.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <ostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace http;

bool matchRoute(const string routePath, const string requestPath,
                http::Request &req) {
  vector<string> routeParts = utils::split(routePath, "/");
  vector<string> requestParts = utils::split(requestPath, "/");
  if (routeParts.size() != requestParts.size()) {
    return false;
  }

  for (size_t i = 0; i < routeParts.size(); ++i) {
    if (routeParts[i] != requestParts[i] && routeParts[i][0] != '{') {
      return false;
    }
  }

  for (size_t i = 0; i < routeParts.size(); ++i) {
    for (size_t j = 0; j < routeParts[i].size(); ++j) {
      string param = "";
      if (routeParts[i][j] != '{') {
        continue;
      }
      if (routeParts[i][j] == '{') {
        while (routeParts[i][j] != '}') {
          if (routeParts[i][j] == '{') {
            j++;
            continue;
          } else {
            param += routeParts[i][j];
            j++;
          }
        }
        req.setUrlParam(param, requestParts[i]);
      }
    }
  }
  return true;
}

string Server::getPlainRoute(string route) {
  vector<string> parts = utils::split(route, " ");
  return parts[1];
}

Server::Server() {
  this->port = "4221";
  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  this->response = http::Response();
  this->request = http::Request();
  this->routes =
      map<string, function<void(http::Response &, http::Request &)>>();
}

Server::~Server() { close(this->server_fd); }

Request Server::getRequest() { return this->request; }

Response Server::getResponse() { return this->response; }

Server::Server(int port) {
  this->port = to_string(port);
  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  this->response = http::Response();
  this->request = http::Request();
  this->routes =
      map<string, function<void(http::Response &, http::Request &)>>();
}

void Server::registerRoute(string route,
                           function<void(Response &, Request &)> callback) {
  this->routes[route] = callback;
};

string Server::getUrlParam(string url) {
  string req_path = this->request.getPath();
  string param = "";
  if (req_path.find(url) != string::npos) {
    param = req_path.substr(req_path.find(url) + url.length());
  }
  return param;
}

void Server::listenAndServe() {
  if (this->server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return;
  }
  int reuse = 1;
  if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse,
                 sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(stoi(this->port));

  if (bind(this->server_fd, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port " << this->port << "\n";
    return;
  }

  if (listen(this->server_fd, 10) != 0) {
    std::cerr << "Failed to listen on port " << this->port << "\n";
    return;
  }

  std::cout << "Listening on port " << this->port << "\n";

  while (true) {
    int client_fd = accept(this->server_fd, NULL, NULL);
    if (client_fd < 0) {
      std::cerr << "Failed to accept connection\n";
      return;
    }
    std::thread([this, client_fd]() {
      char buffer[1024] = {0};
      read(client_fd, buffer, 1024);
      vector<char> req(buffer, buffer + sizeof(buffer) / sizeof(buffer[0]));

      this->request.setRequest(req);
      this->response = http::Response();

      string path = this->request.getPath();
      for (auto route : this->routes) {
        if (matchRoute(this->getPlainRoute(route.first),
                       this->getPlainRoute(path), this->request)) {
          route.second(this->response, this->request);
          break;
        } else {
          this->response.setStatus("404 Not Found");
          this->response.setBody("Route not found");
        }
      }

      string response = this->response.toCRLF();
      send(client_fd, response.c_str(), response.length(), 0);
      close(client_fd);
    }).detach();
  }
}
