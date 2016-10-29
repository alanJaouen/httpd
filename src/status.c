#include "status.h"

void status_msg(int fd, int status, int len)
{
  char *title = malloc(sizeof (char) * 1024);
  char *str_status = malloc(sizeof(char) * 3);

  title = status_title(status);

  sprintf(str_status, "%d", status);

  write(fd, "HTTP/1.1 ", 9);
  write(fd, str_status, 3);
  write(fd, title, strlen(title));

  print_time(fd);

  if (status == 200)
  {
    write(fd, "Content-Length: ", 16);
    char *slen = malloc(sizeof (char) * 1024);
    sprintf(slen, "%d\r\n", len);
    write(fd, slen, strlen(slen));
  }

write(fd, "\r\n", 2);

}

char *status_title(int status)
{
  if (status == 200)
    return " OK\r\n";
  if (status == 403)
    return " Forbidden\r\n";
  if (status == 404)
    return " Not Found\r\n";
  if (status == 505)
    return " HTTP Version Not Supported\r\n";
  return NULL;
}

void print_time(int fd)
{
  time_t cur_time;
  struct tm *info;
  time(&cur_time);
  info = gmtime(&cur_time);

  char *s = malloc(sizeof (char) * 39);
  strftime(s, 38, "Date: %a, %d %b %Y %R:%S GMT\r\n", info);
  write(fd, s, 38);

}
