NAME		= libunit.a
CC			= cc
CFLAGS		= -Wextra -Wall -Werror -Wno-format -I./inc

MAKE		= make
SRCS		= unit.c
OBJS		= $(SRCS:.c=.o)
HEADERS		= unit.h

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean FORCE

