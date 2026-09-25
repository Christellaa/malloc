ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := ft_malloc
CC := gcc
CFLAGS := -Wall -Wextra -Werror
CPPFLAGS := -Iincludes

SRC_DIR := src
OBJ_DIR := obj

SOURCES := ft_malloc.c allocate.c zones.c utils.c debug.c
SRC := $(addprefix $(SRC_DIR)/,$(SOURCES))
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f visualizer/allocator.json

re: fclean all

debug: CFLAGS += -DDEBUG -g
debug: all
	./ft_malloc > visualizer/allocator.json
	python -m http.server 8000

.PHONY: all clean fclean re debug