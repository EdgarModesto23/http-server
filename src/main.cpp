#include "./headers/server.h"

using namespace std;
using namespace http;

void callback(Response &res, Request &req) {
  res.setBody(req.getHeader("User-Agent"));
  res.setContentType("text/plain");
  res.setStatus("200 OK");
}

void emptyPath(Response &res, Request &req) {
  res.setContentType("text/plain");
  res.setStatus("200 OK");
}

int main(int argc, char **argv) {
  Server server = Server(4221);
  server.registerRoute("GET /", emptyPath);
  server.registerRoute("GET /user-agent", callback);
  server.listenAndServe();
}
