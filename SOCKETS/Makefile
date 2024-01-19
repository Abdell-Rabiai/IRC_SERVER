NAME = miniClient

SRC = $(wildcard *.c *.cpp)
HEADER = $(wildcard *.hpp *.h)

OBJ = $(SRC:.cpp=.o)
CC = cc
FLAGS = -Wall -Wextra -Werror 

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re