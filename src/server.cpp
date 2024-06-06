#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

void splitString(string &input, char delimiter, string arr[], int &index) {
  // Creating an input string stream from the input string
  istringstream stream(input);

  // Temporary string to store each token
  string token;

  // Read tokens from the string stream separated by the
  // delimiter
  while (getline(stream, token, delimiter)) {
    // Add the token to the subset
    arr[index++] = token;
  }
}

int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }
  // // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) !=
      0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  std::cout << "Waiting for a client to connect...\n";

  int new_socket = accept(server_fd, (struct sockaddr *)&client_addr,
                          (socklen_t *)&client_addr_len);
  if (new_socket < 0) {
    std::cerr << "accept failed\n";
    return 1;
  }

  char buffer[1024] = {0};
  string subset[100];
  int index = 0;
  char delimiter = '\n';

  read(new_socket, buffer, 1024);
  std::cout << "Received: " << buffer << std::endl;
  string input = buffer;
  splitString(input, delimiter, subset, index);
  string path[100];
  string received = subset[0];
  char whitespace = ' ';
  int new_index = 0;
  splitString(subset[0], whitespace, path, new_index);

  const char *msg;

  if (path[1] == "/") {
    msg = "HTTP/1.1 200 OK\r\n\r\n";
  } else {
    msg = "HTTP/1.1 404 Not Found \r\n\r\n";
  }

  send(new_socket, msg, strlen(msg), 0);

  std::cout << "Client connected\n";

  close(server_fd);

  return 0;
}
