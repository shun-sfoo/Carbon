#include "InetAddress.h"
#include "Socket.h"
#include "util.h"
#include <cstdio>
#include <strings.h>
#include <unistd.h>

namespace {
constexpr int BUFFER_SIZE = 1024;
} // namespace

int main() {
  Socket *clntSocket = new Socket();
  InetAddress *clntAddr = new InetAddress("127.0.0.1", 8888);
  clntSocket->connect(clntAddr);

  while (true) {
    char buf[BUFFER_SIZE]; // 在这个版本，buf大小必须大于或等于服务器端buf大小，不然会出错，想想为什么？
    bzero(&buf, sizeof(buf));
    scanf("%s", buf);
    ssize_t write_bytes = write(clntSocket->getFd(), buf, sizeof(buf));
    if (write_bytes == -1) {
      printf("socket already disconnected, can't write any more!\n");
      break;
    }
    bzero(&buf, sizeof(buf));
    ssize_t read_bytes = read(clntSocket->getFd(), buf, sizeof(buf));
    if (read_bytes > 0) {
      printf("message from server: %s\n", buf);
    } else if (read_bytes == 0) {
      printf("server socket disconnected!\n");
      break;
    } else if (read_bytes == -1) {
      errif(true, "socket read error");
    }
  }

  delete clntAddr;
  delete clntSocket;
  return 0;
}
