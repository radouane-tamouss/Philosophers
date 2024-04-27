CC = cc
CFLAGS = -Wall -Wextra -Werror #-pthread

SRCS = main.c parsing.c monitor.c routine.c init_data.c utils.c
OBJS = ${SRCS:.c=.o}

NAME = philo

all : $(NAME)

$(NAME) : $(SRCS)
	$(CC)  $(CFLAGS) -o $@ $^  #-g  -fsanitize=thread


clean : 
	rm -rf $(OBJS)

run : $(NAME)
	./$(NAME) 199 180 60 60

fclean : clean 
	rm -rf $(NAME)
push : 
	git add .
	git commit -m 'push from makefile!'
	git push

re : fclean all

.PHONY: all clean fclean re
