#ifndef SERVER
#define SERVER
#include <functional>
#include <iostream>
#include <map>

using namespace std;
namespace http {

class Response {
private:
  string status;
  string body;
  string contentType;

public:
  Response();
  ~Response();
  void setStatus(string status);
  void setBody(string body);
  void setContentType(string contentType);
  string getStatus();
  string getBody();
  string getContentType();
  string toCRLF();
};

class Request {
private:
  string method;
  string path;
  string body;
  string contentType;

public:
  Request();
  ~Request();
  void setMethod(string method);
  void setPath(string path);
  void setBody(string body);
  void setContentType(string contentType);
  string getMethod();
  string getPath();
  string getBody();
  string getContentType();
};

class Server {
private:
  string port;
  http::Response response;
  http::Request request;
  int server_fd;
  map<string, function<void(Response &, Request &)>> routes;

public:
  Server();
  ~Server();
  Server(int port);
  void listenAndServer();
  void registerRoute(string route,
                     function<void(Response &, Request &)> &callback);
  void sendResponse();
  void writeResponse(Response response);
  void getUrlParam(string urlParam);
};

} // namespace http

#endif // !SERVER.h
