CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: server client

A = Fix makefile not recompiling helpers

server: server.o helpers.o libft
	@echo "Compiling server.o"
	$(CC) $(CFLAGS) -I . -L libft -lft $< -o $@ 

client: client.o helpers.o libft
	@echo "Compiling client.o"
	$(CC) $(CFLAGS) -I . -L libft -lft $< -o $@

%.o: %.c
	@echo "Making $<"
	$(CC) $(CFLAGS) -c $< -o $@

libft:
	@echo "Making libft"
	$(MAKE) -C libft

clean:
	$(MAKE) clean -C libft
	rm -f server.o client.o

fclean:
	$(MAKE) fclean -C libft
	rm -f server.o client.o server client

re: fclean all

.PHONY: all server client libft clean fclean re