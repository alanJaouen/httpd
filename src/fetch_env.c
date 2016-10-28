#include "fetch_env.h"

char *env_set(char *var)
{
  char *val = getenv(var);

  if (val)
    val[strlen(val)] = '\0';

  return val;
}

void check_ip(char *ip)
{
  int scount = 0;
  int ccount = 0;
  int valseg = 0;

  if (!ip)
    exit(1);

  while(*ip)
  {
    if (*ip == '.')
    {
      if (ccount == 0)
        exit(1);

      scount++;

      if (scount == 4)
        exit(1);

      valseg = 0;
      ccount = 0;
      ip++;
      continue;
    }

    if ((*ip < '0') || (*ip > '9'))
      exit(1);

    valseg = valseg * 10 + *ip - '0';

    if (valseg > 255)
      exit(1);

    ccount++;
    ip++;
  }

  if (scount != 3)
    exit(1);

  if (ccount == 0)
    exit(1);
}

void check_port(char *port)
{
  if (!port)
    exit(1);

  int res = atoi(port);

  if (res < 0 || res > 65535)
    exit(1);
}

void set_root(char *path)
{
    char *cur_dir = get_current_dir_name();

    if (!cur_dir)
      exit(1);

    struct stat buf;

    if(stat(cur_dir, &buf))
      exit(1);

    if (!S_ISDIR(buf.st_mode))
      exit(1);

    size_t n = strlen(cur_dir);

    path = memcpy(path, cur_dir, n+1);
}

s_env *struct_init(void)
{
  s_env *env = malloc(sizeof (s_env));
  if (!env)
    exit(1);

  env->ip = malloc(sizeof (char) * 16);

  if (!env->ip)
  {
    free(env);
    exit(1);
  }

  env->port = malloc(sizeof (char) * 6);

  if (!env->port)
  {
    free(env->ip);
    free(env);
    exit(1);
  }

  env->rdir = malloc(sizeof (char) * 1024);

  if (!env->rdir)
  {
    free(env->ip);
    free(env->port);
    free(env);
    exit(1);
  }

  return env;
}

void fetch(s_env *env)
{
  env->ip = env_set("LISTEN_IP");

  check_ip(env->ip);

  env->port = env_set("LISTEN_PORT");

  check_port(env->port);

  char *p = env_set("ROOT_DIR");

  if (!p)
    set_root(env->rdir);
  else
    env->rdir = p;
}

void fetch_env(void)
{
  s_env *env = struct_init();
  fetch(env);
}
