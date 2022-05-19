NAME	= pipex

SRCS	= 	$(addprefix srcs_v3/,		\
			pipex.c 				\
			error.c					\
			child.c					\
			here_doc.c				\
			path.c)

FLAGS	= -Wall -Wextra -Werror
INCL	= -I./libft -I.
LIB		= -L./libft -lft

all :	$(NAME)

pipex :	libft.a
	gcc $(FLAGS) $(INCL) $(LIB) $(SRCS) -o $(NAME)
	@printf "Pipex done!\n"

libft.a :
	make -C ./libft

clean :
	make fclean -C ./libft
	@rm pipex
	@printf "Pipex deleted\n"

fclean :	clean

bonus :		all

re :	clean all

.PHONY :	re all clean
