#include <stdio.h>
#include "net.h"
#include "signalhandler.h"

int main()
{
  init_handler();
  printest();
  for (int i = 0; i < 5; i++)
  {
    i = 0;
    continue;
  }
  return 0;

}
