#include "../headers/server.h"

using namespace http;

Response::Response() {
  this->status = "200 OK";
  this->body = "";
  this->contentType = "text/plain";
  this->headers = map<string, string>{};
}

Response::~Response() {}

string Response::getStatus() { return this->status; }

string Response::getBody() { return this->body; }

string Response::getContentType() { return this->contentType; }

void Response::setStatus(string status) { this->status = status; }

void Response::setBody(string body) { this->body = body; }

void Response::setHeader(string key, string value) {
  this->headers[key] = value;
}

string Response::HeadersToString() {
  string result{""};
  for (auto val : this->headers) {
    result += val.first + ": " + val.second + "\r\n";
  }
  return result;
}

void Response::setContentType(string contentType) {
  this->contentType = contentType;
}

string Response::toCRLF() {
  return "HTTP/1.1 " + this->status + "\r\nContent-Type: " + this->contentType +
         "\r\nContent-Length: " + to_string(this->body.length()) + "\r\n" +
         this->HeadersToString() + "\r\n" + this->body;
}
