#include "../headers/server.h"
#include <zlib.h>

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

string Response::getHeader(string key) {
  if (this->headers.find(key) != this->headers.end()) {
    return this->headers[key];
  }
  return "";
}

void Response::setContentType(string contentType) {
  this->contentType = contentType;
}

string Response::CompressData(string &data) {
  z_stream zs;
  zs.zalloc = Z_NULL;
  zs.zfree = Z_NULL;
  zs.opaque = Z_NULL;

  // Initialize gzip with default compression level
  if (deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8,
                   Z_DEFAULT_STRATEGY) != Z_OK) {
    throw std::runtime_error("Failed to initialize gzip compression");
  }

  zs.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(data.data()));
  zs.avail_in = data.size();

  int ret;
  char buffer[32768];
  std::string compressedData;

  do {
    zs.next_out = reinterpret_cast<Bytef *>(buffer);
    zs.avail_out = sizeof(buffer);

    ret = deflate(&zs, Z_FINISH);

    if (compressedData.size() < zs.total_out) {
      compressedData.append(buffer, zs.total_out - compressedData.size());
    }
  } while (ret == Z_OK);

  deflateEnd(&zs);

  if (ret != Z_STREAM_END) {
    throw std::runtime_error("Failed to compress data with gzip");
  }

  return compressedData;
}

string Response::toCRLF() {
  return "HTTP/1.1 " + this->status + "\r\nContent-Type: " + this->contentType +
         "\r\nContent-Length: " + to_string(this->body.length()) + "\r\n" +
         this->HeadersToString() + "\r\n" + this->body;
}
