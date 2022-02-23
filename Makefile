CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: server client

server: server.o libft
	echo "making server"
	$(CC) $(CFLAGS) -I . -L libft -lft $< -o $@ 

client:

%.o: %.c
	echo "making $<"
	$(CC) $(CFLAGS) -c $< -o $@

libft:
	echo "making libft"
	$(MAKE) -C libft

.PHONY: all server libft