#include "./headers/server.h"

using namespace std;
using namespace http;

void callback(Response &res, Request &req) {
  res.setBody(req.getUrlParam("str"));
  res.setContentType("text/plain");
}

int main(int argc, char **argv) {
  Server server = Server(4221);
  server.registerRoute("GET /echo/{str}", callback);
  server.listenAndServe();
}
