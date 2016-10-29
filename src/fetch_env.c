#include "fetch_env.h"

s_env *g_env = NULL;

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

void struct_init(void)
{
  g_env = malloc(sizeof (s_env));
  if (!g_env)
    exit(1);

  g_env->ip = malloc(sizeof (char) * 16);

  if (!g_env->ip)
  {
    free(g_env);
    exit(1);
  }

  g_env->port = malloc(sizeof (char) * 6);

  if (!g_env->port)
  {
    free(g_env->ip);
    free(g_env);
    exit(1);
  }

  g_env->rdir = malloc(sizeof (char) * 1024);

  if (!g_env->rdir)
  {
    free(g_env->ip);
    free(g_env->port);
    free(g_env);
    exit(1);
  }

}

void fetch(void)
{
  g_env->ip = env_set("LISTEN_IP");

  check_ip(g_env->ip);

  g_env->port = env_set("LISTEN_PORT");

  check_port(g_env->port);

  char *p = env_set("ROOT_DIR");

  if (!p)
    set_root(g_env->rdir);
  else
    g_env->rdir = p;
}

void fetch_env(void)
{
  struct_init();
  fetch();
}
