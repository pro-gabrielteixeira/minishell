GREEN		= 	\033[0;32m
RED			= 	\033[0;31m
RESET		= 	\033[0m

NAME		=	minishell

LFT			=	libft/libft.a

HEADER		=	./includes -I ./libft/includes

OBJ			=	$(patsubst src%, obj%, $(SRC:.c=.o))

SRC			=	src/minishell.c \
				src/parse/parse_init.c \
				src/parse/parse_utils.c \
				src/parse/parse_quotes.c \
				src/cmd_handler/cmds.c \
				src/cmd_handler/free.c \
				src/cmd_handler/cmds_utils.c \
				src/cmd_handler/cmds_utils2.c \
				src/cmd_handler/split_cmds.c \
				src/execution/ft_exec.c \
				src/execution/ft_redirect.c \
				src/execution/ft_input.c \
				src/builtins/ft_echo.c \
				src/builtins/ft_cd.c \
				src/builtins/ft_env.c \
				src/builtins/ft_exit.c \
				src/builtins/ft_unset.c \
				src/builtins/ft_pwd.c \
				src/builtins/ft_export.c \
				src/builtins/check_builtins.c

CC			=	cc
FLAGS		=	-I${HEADER} -lreadline -L ./libft -lft -Wall -Wextra -Werror -g -fsanitize=address

all:		$(LFT) obj $(NAME)

$(NAME):	$(OBJ)
			@$(CC) -o $@ $^ $(FLAGS)

$(LFT):		
			@echo " [ .. ] | Compiling libft.."
			@make -s -C libft
			@echo " [ $(GREEN)OK$(RESET) ] | Libft ready!"

obj:
			@mkdir -p obj/parse obj/execution obj/builtins obj/cmd_handler

obj/%.o:	src/%.c ./includes/minishell.h
			@$(CC) $(FLAGS) -o $@ -c $<
			@echo "$@ $(GREEN)created$(RESET)"

valgrind:	all
			valgrind --leak-check=full --show-leak-kinds=all --quiet ./minishell

clean:
			@rm -rf $(OBJ) obj
			@echo "Object files $(RED)removed.$(RESET)"

fclean:		clean
			@rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re