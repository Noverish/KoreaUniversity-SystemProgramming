#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <pthread.h>
#define BUF_LEN 128
#define HOST "192.168.56.101"

void *connect_thread(void *arg);
char *print_time();

int ports[] = {9000, 9001, 9002, 9003, 9004};
pthread_t threads[5];

int main(int argc, char *argv[]) {
  int i;
  for(i = 0; i < 5; i++) {
    pthread_create(&threads[i], NULL, &connect_thread, (void *)ports[i]);
    printf("[%d] %s pthread_create - %lu\n", ports[i], print_time(), threads[i]);
  }

  for(i = 0; i < 5; i++)
    pthread_join(threads[i], NULL);

  return 0;
}

void *connect_thread(void *arg) {
  const int PORT = (int) arg;

  char buffer[BUF_LEN];
  struct sockaddr_in server_addr;
  int s;

  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
      perror("ERROR - socket");
      exit(1);
  }
  printf("[%d] %s socket success\n", PORT, print_time());

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(HOST);

  if (connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
      perror("ERROR - connect");
      close(s);
      exit(1);
  }
  printf("[%d] %s connect success\n", PORT, print_time());

  while(1) {
    memset(buffer, 0, BUF_LEN);
    recv(s, buffer, BUF_LEN, 0);
    printf("[%d] %s %lu %s\n", PORT, print_time(), strlen(buffer), buffer);
  }

  pthread_exit((void *)0);
}

char *print_time() {
  char *str;
  struct timeval val;
  struct tm *ptm;

  str = (char *) malloc(10 * sizeof(char));
  gettimeofday(&val, NULL);
  ptm = localtime(&val.tv_sec);

  int hour = ptm->tm_hour;
  int min = ptm->tm_min;
  int sec = ptm->tm_sec;
  int millis = val.tv_usec / 1000;

  sprintf(str, "%02d:%02d:%02d.%03d", hour, min, sec, millis);

  return str;
}
