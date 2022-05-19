NAME	= pipex

FLAGS	= -Wall -Wextra -Werror -g
INCL	= -I./libft -I.
LIB		= -L./libft -lft

SRCS_DIR	= srcs
OBJS_DIR	= $(shell mkdir -p objs && printf "objs")

SRCS	=	pipex.c			\
			error.c			\
			child.c			\
			here_doc.c		\
			path.c			\

OBJS	= $(SRCS:%.c=$(OBJS_DIR)/%.o)

all :	$(NAME)

$(OBJS_DIR)/%.o :	$(SRCS_DIR)/%.c
	gcc $(FLAGS) $(INCL) -c $< -o $@

$(NAME) :	libft.a $(OBJS)
	gcc $(FLAGS) $(INCL) $(LIB) $(OBJS) -o $(NAME)
	@printf "Pipex done!\n"

libft.a :
	make -C ./libft

clean :
	make fclean -C ./libft
	rm pipex
	rm -rf $(OBJS_DIR)

fclean :	clean

bonus :		all

re :	clean all

.PHONY :	re all clean fclean bonus
