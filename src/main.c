#include <stdio.h>
#include <signal.h>
#include "fetch_env.h"
#include "net.h"


void free_mem(void)
{
  free(g_env->rdir);
  free(g_env);
}

void clo(int a)
{
  a = a + 1;
  exit(0);
}

int main()
{
  signal(SIGINT, clo);
  signal(SIGTERM, clo);
  atexit(free_mem);
  fetch_env();
  init_server();
  return 9;
  for (int i = 0; i < 5; i++)
  {
    i = 0;
    continue;
  }
  return 0;
}
