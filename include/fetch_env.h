#ifndef FETCH_ENV_H
# define FETCH_ENV_H

# define _BSD_SOURCE
# define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

typedef struct env s_env;

struct env
{
  char *ip;
  char *port;
  char *rdir;
  int fd_server;
};

extern s_env *g_env;

char *env_set(char *var);
void struct_init(void);
void check_ip(char *ip);
void check_port(char *port);
void set_root(char *path);
void fetch(void);
void fetch_env(void);

#endif
