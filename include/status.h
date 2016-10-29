#ifndef STATUS_H
# define STATUS_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

void status_msg(int fd,int status, int len);
void print_time(int fd);
char *status_title(int status);

#endif
