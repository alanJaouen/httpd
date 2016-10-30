#ifndef STATUS_H
# define STATUS_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "fetch_env.h"

void status_msg(int fd,int status, size_t len);
void print_time(int fd);
char *status_title(int status);

#endif
