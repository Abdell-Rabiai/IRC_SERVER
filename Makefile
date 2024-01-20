NAME = IRC_SERVER

Color_Off=\033[0m
LIGHTRED=\033[1;31m
LightBlue=\033[1;34m
LightPurple=\033[1;35m
Cyan=\033[0;36m
GREEN = \033[92m

BANNER=\
'██ ██████   ██████         ███████ ███████ ██████  ██    ██ ███████ ██████  '\
'██ ██   ██ ██              ██      ██      ██   ██ ██    ██ ██      ██   ██ '\
'██ ██████  ██              ███████ █████   ██████  ██    ██ █████   ██████  '\
'██ ██   ██ ██                   ██ ██      ██   ██  ██  ██  ██      ██   ██ '\
'██ ██   ██  ██████ ███████ ███████ ███████ ██   ██   ████   ███████ ██   ██ '\                                                                                              
																									 
SOURCE = $(wildcard SOURCE/SERVER/*.cpp)
SOURCE += $(wildcard SOURCE/CLIENT/*.cpp)

OBJ_DIR = .OBJ_DIR

SRCS = $(SOURCE)

MAIN_SRC = SOURCE/main.cpp

FLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address

HEADER = HEADERS/$(wildcard *.hpp)

CC = c++ -std=c++98

OBJ = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o)) 

all : banner2 $(NAME)

banner2:
	@clear
	@echo "\n"
	@echo "$(LightPurple)"
	@for s in $(BANNER) ; do \
		printf "%s\n" "$$s" ; \
		sleep 0.03 ; \
	done ; \
	echo "$(LIGHTRED)                     Copyright : Black-Hole1004, Abdell-rabiai :) \n$(Color_Off)"

$(OBJ_DIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@printf "$(GREEN) Generating SERVER objects... %-33.33s\r" $@
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME) : $(OBJ) $(MAIN_SRC) $(HEADER)
	@$(CC) $(FLAGS) $(MAIN_SRC) $(OBJ) -o $(NAME)
	@echo "\n\n\t$(Cyan) Enjoooy :)\n $(Color_Off)"

clean :
	@rm -rf $(OBJ) $(OBJ_DIR)

fclean : clean
	@rm -rf $(NAME)
	@rm -rf *.dSYM .vscode

re : fclean all

.PHONY : all clean fclean re
