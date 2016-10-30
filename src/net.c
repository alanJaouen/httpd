#define _POSIX_C_SOURCE 201112L
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <regex.h>
#include <sys/mman.h>
#include <ctype.h>
#include "net.h"
#include "fetch_env.h"
#include "status.h"
#define BUFFSIZE 1024

void removetrailing(char *str)
{
  char *end = str + strlen(str) + 1;
  while (end > str && !isgraph(*end))
    --end;
  end[1] = '\0';
  return;
}

//return true if the file exists
int file_exist(char *path)
{
  removetrailing(path);
  struct stat tmp;
  //write(STDOUT_FILENO, path, 140);
  printf("%s\n", path);
  if (stat(path, &tmp))
  {
    perror("stat");
     return 0;
  }
  return S_ISREG(tmp.st_mode);
}

void get_file(char *name, int fd_user)
{
  char f[1024];
  strcpy(f, g_env->rdir);
  strcat(f, name);
  if (!file_exist(f))
    status_msg(fd_user, 404, 0);
  else if (access(f, R_OK))
    status_msg(fd_user, 403, 0);
  else
  {
    struct stat tmp;
    stat(f, &tmp);
    FILE *ff = fopen(f, "r");
    if (!ff)
      return;
    ssize_t nbread = 0;
    char *line = NULL;
    size_t len = 0;
    while ((nbread = getline(&line, &len, ff)) != -1)
      send(fd_user, line, nbread, 0);
    fclose(ff);
  }
    return;
}

void make_response(int fd_user)
{
  char buff[BUFFSIZE];//TODO voir la doc
  int s_read = read_socket(fd_user, buff, BUFFSIZE);
  char *file = malloc(1024);

  int t =check_requestline(buff, file, fd_user);
  while (s_read > 0)
  {
    write(STDOUT_FILENO, buff, s_read);
    s_read = read_socket(fd_user, buff, BUFFSIZE);
    if (!strncmp("\r\n", buff, 2))
      break;
  }
  switch (t)
  {
    case -1:
      return;
    case GET:
      get_file(file, fd_user);

      break;
    case POST:
      //TODO that
      break;
  }
}

//return -1 in case of fail
int check_requestline(char *req, char *file, int fd_user)
{
  char *r = "^(GET|POST) ([\\/_a-zA-Z0-9.-]*) HTTP\\/([0-9]*|[0-9]*\\.[0-9]*)";
  regex_t regex2;
  if (regcomp(&regex2, r, REG_EXTENDED))
    return -1;
  char *cur = req;
  regmatch_t arr[4];
  if (regexec(&regex2, cur, 3, arr, 0))
    return -1;
  int type = -1;
  if (!strncmp("GET", req,
        3))
    type = GET;
  else if (!strncmp("POST", req,
        4))
    type = POST;
  else
    status_msg(fd_user, 505, 0);
  if (strncmp("1.1", req + arr[2].rm_eo + 6, 3))
  {
    status_msg(fd_user, 505, 0);
    type = -1;
  }
  file = memcpy(file, req + arr[2].rm_so,
       arr[2].rm_eo - arr[2].rm_so);
  file[arr[2].rm_eo - arr[2].rm_so + 1] = '\0';
  regfree(&regex2);
  return type;
}

int run_server(int fd_sock )
{

  if (listen(fd_sock, 10))
  {
    perror("listen");
    return 1;
  }
  struct sockaddr_in user;
  void *v = &user;
  struct sockaddr *s = v;
  socklen_t len = sizeof(struct sockaddr);

  while (1)
  {
    int fd_user = accept(fd_sock, s, &len);
    printf("client connecté\n%s:%d\n", inet_ntoa(user.sin_addr), ntohs(user.sin_port));
    if (fd_user == -1)
      perror("accept");
    make_response(fd_user);
    close(fd_user);
  }

  return 0;
}

int init_server(void)
{
  int fd_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_sock < 0)
  {
    perror("socket");
    return 1;
  }
  int opt = 1;
  void *v = &opt;
  char *c = v;
  setsockopt(fd_sock, SOL_SOCKET, SO_REUSEADDR, c, sizeof(int));
  g_env->fd_server = fd_sock;
  struct sockaddr_in serv;
  serv.sin_family = AF_INET;
  serv.sin_port = htons(atoi(g_env->port));//port
  inet_aton(g_env->ip, &(serv.sin_addr));//ip

  v = &serv;
  struct sockaddr *s = v;
  if (bind(fd_sock, s, sizeof(struct sockaddr)) != 0)
  {
    perror("bind");
    return 2;
  }
  return run_server(fd_sock);
}

size_t read_socket(int fd, char *buff, size_t max)
{
  size_t i = 0;
  buff[max] = '\0';
  while (i < max - 1)
  {
    int n = recv(fd, buff + i, 1, 0);
    if (n < 1)
    {
      buff[i] = '\0';
      break;
    }
    if (buff[i] == '\n' && i && buff[i - 1] == '\r')
    {
      buff[i + 1] = '\0';
      i++;
      break;
    }
    i++;
  }
  return i;
}
