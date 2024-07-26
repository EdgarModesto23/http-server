#include "../headers/server.h"
#include <ostream>
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

void Request::setHeader(string header, string value) {
  this->headers[header] = value;
}

string Request::getHeader(string header) {
  if (this->headers.find(header) != this->headers.end()) {
    return this->headers[header];
  }
  return "";
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
  vector<string> lines = utils::split(request, "\r\n");
  string body = utils::split(request, "\r\n\r\n")[1];
  vector<string> first_line = utils::split(lines[0], " ");
  this->path = first_line[0] + " " + first_line[1];
  this->method = first_line[0];
  for (size_t i = 0; i < lines.size(); ++i) {
    vector<string> header = utils::split(lines[i], ":");
    if (header.size() > 1) {
      this->setHeader(header[0], header[1]);
    }
  }
  if (body.length() > 0 && this->getHeader("Content-Length") != "") {
    int content_length = stoi(this->getHeader("Content-Length"));
    body = body.substr(0, content_length);
    this->setBody(body);
  }
}
