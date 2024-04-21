# Linux
# Windows

# Name of the program.
NAME     := Cartes_JDR.exe

# Sources and objects.
SRCS     := main.cpp
OBJS     := $(SRCS:.cpp=.o)

# Define all the compiling flags.
CXX      := x86_64-w64-mingw32-g++
CXXFLAGS := -Wall -Werror -Wextra 

# Define path to SFML include and lib directories
SFML_INCLUDE := -I./SFML/include
SFML_LIBS    := -L./SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Compile and create everything.
all: $(NAME)

# Compile the program with the objects.
$(NAME): $(OBJS)
	@$(CXX) $(OBJS) -o $@ $(SFML_LIBS)

# This won't run if the source files don't exist or are not modified.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SFML_INCLUDE) -o $@ -c $<

# Rule to remove all the object files.
clean:
	@rm -f $(OBJS)
	@echo "[INFO] Objects removed!"

# Rule to remove everything that has been created by the makefile.
fclean: clean
	@rm -f $(NAME)
	@echo "[INFO] $(NAME) removed!"

# Rule to re-make everything.
re:     fclean all

# Makes sure that gnu make will still run even if files called
# clean / fclean / all and re already exist in the directory
.PHONY: all clean fclean format re