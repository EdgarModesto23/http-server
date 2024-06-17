#ifndef SERVER
#define SERVER
#include "../cmd/utils.cpp"
#include <functional>
#include <iostream>
#include <map>
#include <vector>

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
  map<string, string> urlParams;

public:
  Request();
  ~Request();
  Request(vector<char> buff);
  void setMethod(string method);
  void setPath(string path);
  void setBody(string body);
  void setContentType(string contentType);
  string getMethod();
  string getPath();
  string getBody();
  string getContentType();
  string getUrlParam(string urlParam);
  void setUrlParam(string urlParam, string value);
  void setRequest(vector<char> req);
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
  void listenAndServe();
  void registerRoute(string route,
                     function<void(Response &, Request &)> callback);
  string getUrlParam(string urlParam);
  Request getRequest();
  string getPlainRoute(string route);
  Response getResponse();
};

} // namespace http

#endif // !SERVER.h
