#include "./cmd/server.cpp"
#include "./cmd/request.cpp"
#include "./cmd/response.cpp"

using namespace std;
using namespace http;

void callback(Response &res, Request &req) {
  res.setBody(req.getUrlParam("str"));
  res.setContentType("text/plain");
}

void callbackGet(Response &res, Request &req) {
  res.setBody("hola mundito");
  res.setContentType("text/plain");
}

int main() {
  Server server = Server(8000);
  server.registerRoute("POST /echo/{str}", callback);
  server.registerRoute("GET /echo/", callbackGet);
  server.listenAndServe();
}
