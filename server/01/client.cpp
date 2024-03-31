#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>

int main() {
  int sockFd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in servAddr;
  bzero(&servAddr, sizeof(servAddr));

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(8888);

  connect(sockFd, reinterpret_cast<sockaddr *>(&servAddr), sizeof(servAddr));
}
