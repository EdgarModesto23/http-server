#include "../headers/server.h"
#include <vector>

using namespace http;

Request::Request() {
  this->path = "/";
  this->body = "";
  this->method = "GET";
  this->contentType = "text/plain";
  this->urlParams = map<string, string>();
}

Request::~Request() {}

Request::Request(vector<char> buff) {}

string Request::getBody() { return this->body; }

string Request::getMethod() { return this->method; }

string Request::getPath() { return this->path; }

string Request::getContentType() { return this->contentType; }

void Request::setBody(string body) { this->body = body; }

void Request::setPath(string path) { this->path = path; }

void Request::setMethod(string method) { this->method = method; }

void Request::setContentType(string contentType) {
  this->contentType = contentType;
}

string Request::getUrlParam(string urlParam) {
  if (this->urlParams.find(urlParam) != this->urlParams.end()) {
    return this->urlParams[urlParam];
  }
  return "";
}

void Request::setUrlParam(string urlParam, string value) {
  this->urlParams[urlParam] = value;
}

void Request::setRequest(vector<char> req) {
  string request(req.begin(), req.end());
  vector<string> lines = split(request, "\r\n");
  vector<string> first_line = split(lines[0], " ");
  this->path = first_line[0] + " " + first_line[1];
  this->method = first_line[0];
}
