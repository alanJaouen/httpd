#include <stdio.h>
#include "fetch_env.h"
#include "net.h"
#include "signalhandler.h"

int main()
{
  fetch_env();
  init_handler();
  printest();
  for (int i = 0; i < 5; i++)
  {
    i = 0;
    continue;
  }
  return 0;

}
