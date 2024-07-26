#include "./headers/server.h"
#include <cstring>
#include <fstream>

using namespace std;
using namespace http;

std::string DIR;

void callback(Response &res, Request &req) {
  res.setBody(req.getUrlParam("str"));
  res.setContentType("text/plain");
  res.setStatus("200 OK");
}

void emptyPath(Response &res, Request &req) {
  res.setContentType("text/plain");
  res.setStatus("200 OK");
}

void readHeader(Response &res, Request &req) {
  res.setBody(req.getHeader("User-Agent"));
  res.setContentType("text/plain");
  res.setStatus("200 OK");
}

void getFile(Response &res, Request &req) {
  string path = req.getUrlParam("path");
  ifstream file(DIR + path);
  if (!file.is_open()) {
    res.setBody("");
    res.setStatus("404 Not Found");
    return;
  }
  string content((istreambuf_iterator<char>(file)),
                 (istreambuf_iterator<char>()));
  res.setBody(content);
  file.close();
  res.setContentType("application/octet-stream");
  res.setStatus("200 OK");
}

int main(int argc, char **argv) {
  if (argc == 3 && strcmp(argv[1], "--directory") == 0) {
    DIR = argv[2];
  }
  Server server = Server(4221);
  server.registerRoute("GET /", emptyPath);
  server.registerRoute("GET /echo/{str}", callback);
  server.registerRoute("GET /user-agent", readHeader);
  server.registerRoute("GET /file/{path}", getFile);
  server.listenAndServe();
}
