#include "server.h"

int main(void)
{
    pid_t   pid;

    pid = getpid();
    ft_printf("The pid is %d\n", pid);
}