#pragma once
#ifndef SERVER
#define SERVER
#include <functional>
#include <iostream>
#include <map>
#include <vector>
#include <zlib.h>

using namespace std;

namespace utils {
vector<std::string> split(string str, string token);
} // namespace utils
namespace http {

class Response {
private:
  string status;
  string body;
  string contentType;
  map<string, string> headers;

public:
  Response();
  ~Response();
  void setStatus(string status);
  void setBody(string body);
  void setContentType(string contentType);
  void setHeader(string header, string value);
  string getHeader(string key);
  string CompressData(string &data);
  string getStatus();
  string getBody();
  string getContentType();
  string toCRLF();
  string HeadersToString();
};

class Request {
private:
  string method;
  string path;
  string body;
  string contentType;
  map<string, string> headers;
  map<string, string> urlParams;

public:
  Request();
  ~Request();
  Request(vector<char> buff);
  void setMethod(string method);
  void setPath(string path);
  void setBody(string body);
  void setContentType(string contentType);
  void setHeader(string header, string value);
  string getHeader(string header);
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
  void getClientEncoding();
};

} // namespace http

#endif // !SERVER.h
