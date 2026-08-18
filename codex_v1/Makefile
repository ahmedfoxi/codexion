NAME = codex

CC = cc

CFLAGS = -Wall -Wextra -Werror -g -pthread

SRCS =  main1.c \
		heap_queue.c\
		help.c\
		monitor.c\
		time_calule.c\

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
