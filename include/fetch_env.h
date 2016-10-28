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
};

char *env_set(char *var);
s_env *struct_init(void);
int check_ip(char *env);
int check_port(char *port);
void set_root(char *path);
void fetch(s_env *env);
void fetch_env(void);

#endif
