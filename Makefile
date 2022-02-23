SRCS	= pipex.c

FLAGS	= -Wall -Wextra -Werror
INCL	= -I./libft -I.
LIB		= -L./libft -lft

all :	pipex

pipex :	libft.a
	gcc $(FLAGS) $(INCL) $(LIB) $(SRCS) -o pipex

libft.a :
	make -C ./libft

clean :
	make clean -C ./libft
	rm pipex

re :	clean all

.PHONY :	re all clean
