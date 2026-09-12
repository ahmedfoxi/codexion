NAME = codex

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread -g

SRCS =  src/main.c \
		src/initialization.c \
		src/parser.c \
		src/heap_queue.c \
		src/time_calule.c \
		src/clean_up.c \
		src/routing.c \
		src/aquire_dongle.c \
		src/dongle_utils.c \
		src/monitor.c \
		src/log_action.c \

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
