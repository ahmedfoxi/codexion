NAME = codex

CC = cc

CFLAGS = -Wall -Wextra -Werror -g -pthread

SRCS =  src/main.c \
		src/heap_queue.c \
		src/initialization.c \
		src/parser.c \
		src/time_calcule \

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
