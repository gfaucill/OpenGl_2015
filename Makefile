BIN = projet
# FLAG
FLAGS = -Wall  -g #-o2
# FLAG
LFLAGS = -Wall -g #-o2
# INCLUDES
INC = .
# INCLUDES
LIBDIR = /usr/lib/nvidia-331/
# Librairies
LIBS = -lGLEW -lGLU -lGL -lglut
# Compilateur
CC = g++
# Dossier objet
DOS_OBJ = obj
# Dossier sources
DOS_SRC = .
# Sources
SRC =  $(wildcard *.cpp */*.cpp)
# Compile
OBJ =  $(SRC:.cpp=.o)

all: $(OBJ)
	@echo "**** COMPILATION ****"
	$(CC) $^ -o $(BIN) $(LFLAGS) -L$(LIBDIR) $(LIBS) 

run:all
	@echo "**** RUN ****"
	$(BIN)

clean:
	rm -f $(BIN)
	rm -f */*.o
	rm -f *.o
	rm -f $(DOS_SRC)/*~

$(DOS_OBJ)/main.o: $(DOS_SRC)/main.cpp tools/*.hpp
	$(CC) $(FLAGS) -I$(INC) -c $< -o $(DOS_OBJ)/main.o

$(DOS_OBJ)/%.o: $(DOS_SRC)/%.cpp $(DOS_SRC)/%.hpp
	$(CC) $(FLAGS) -I$(INC) -c $< -o $@

