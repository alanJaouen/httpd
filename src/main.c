#include <stdio.h>
#include "fetch_env.h"
#include "free_mem.h"
#include "net.h"
#include "signalhandler.h"

int main()
{
  atexit(free_mem);
  fetch_env();
  init_handler();
  init_server();
  return 9;
  for (int i = 0; i < 5; i++)
  {
    i = 0;
    continue;
  }
  return 0;
}
