CC = gcc
CFLAGS = -Wall -Wextra -Werror

SERVER_SRCS = server.c server_utils_1.c server_utils_2.c
SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_SRCS = client.c client_utils.c
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

all: server client

server: $(SERVER_OBJS) libft
	@echo "Compiling server"
	$(CC) $(CFLAGS) -I . -L libft -lft $(SERVER_OBJS) -o $@ 

client: $(CLIENT_OBJS) libft
	@echo "Compiling client"
	$(CC) $(CFLAGS) -I . -L libft -lft $(CLIENT_OBJS) -o $@

%.o: %.c
	@echo "Making $<"
	$(CC) $(CFLAGS) -c $< -o $@

libft:
	@echo "Making libft"
	$(MAKE) -C libft

clean:
	$(MAKE) clean -C libft
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS)

fclean:
	$(MAKE) fclean -C libft
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) server client

re: fclean all

.PHONY: all server client libft clean fclean re