NAME        := libftpp.a
CXX         := c++
CXXFLAGS    := -Wall -Wextra -Werror -std=c++17
AR          := ar rcs
RM          := rm -f

INCLUDE_DIR := include
SRC_DIR     := src
OBJ_DIR     := obj

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

# Convert .cpp to .o by replacing src/ with obj/ and .cpp with .o
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Archiving library: $(NAME)"
	$(AR) $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	$(RM) -r $(OBJ_DIR)

fclean: clean
	@echo "Removing library..."
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re