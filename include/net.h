#ifndef _H_NET
#define _H_NET

#include <stddef.h>

int run_server(int fd);

size_t read_socket(int fd, char *buff, size_t max);

int init_server(void);

void make_response(int fd_user);

int check_requestline(char *req, char *file, int fd);

enum type
{
  GET,
  POST
};

#endif
