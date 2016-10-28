#include <stdio.h>
#include "fetch_env.h"
#include "net.h"
#include "signalhandler.h"

int main()
{
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
