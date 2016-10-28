#include "fetch_env.h"

char *env_set(char *var)
{
  char *val = getenv(var);

  if (val)
    val[strlen(val)] = '\0';

  return val;
}

int check_ip(char *ip)
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
        return 0;

      scount++;

      if (scount == 4)
        return 0;

      valseg = 0;
      ccount = 0;
      ip++;
      continue;
    }

    if ((*ip < '0') || (*ip > '9'))
      return 0;

    valseg = valseg * 10 + *ip - '0';

    if (valseg > 255)
      return 0;

    ccount++;
    ip++;
  }

  if (scount != 3)
    return 0;

  if (ccount == 0)
    return 0;

  return 1;
}

int check_port(char *port)
{
  if (!port)
    return 0;

  int res = atoi(port);

  if (res < 0 || res > 65535)
    return 0;

  return 1;
}

void set_root(char *path)
{
  if (!path)
  {
    char *cur_dir = get_current_dir_name();

    if (!cur_dir)
      exit(1);

    struct stat buf;

    if(stat(cur_dir, &buf))
      exit(1);

    if (!S_ISDIR(buf.st_mode))
      exit(1);

    path = cur_dir;
  }
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

  int res1 = check_ip(env->ip);

  if (!res1)
    exit(1);

  env->port = env_set("LISTEN_PORT");

  int res2 = check_port(env->port);

  if (!res2)
    exit(1);

  env->rdir = env_set("ROOT_DIR");

  if (!env->rdir)
    set_root(env->rdir);
}

void fetch_env(void)
{
  s_env *env = struct_init();
  fetch(env);
}
