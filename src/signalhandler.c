#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "signalhandler.h"



void init_handler(void)
{
  struct sigaction new;
  new.sa_handler = exit_handler;
  sigemptyset(&new.sa_mask);
  new.sa_flags = 0;

  sigaction(SIGINT, &new, NULL);
  sigaction(SIGTERM, &new, NULL);
}

void exit_handler(int signum)
{
  write(STDOUT_FILENO, "handler\n", 8);
  //exit(0);
}
