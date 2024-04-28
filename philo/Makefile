CC = cc
CFLAGS = -Wall -Wextra -Werror #-g  -fsanitize=thread

SRCS = main.c parsing.c monitor.c routine.c init_data.c utils.c
OBJS = ${SRCS:.c=.o}

NAME = philo

all : $(NAME)

$(NAME) : $(SRCS)
	$(CC)  $(CFLAGS) -o $@ $^  


clean : 
	rm -rf $(OBJS)

fclean : clean 
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re
