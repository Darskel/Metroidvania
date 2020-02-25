CFLAGS = -Wall
CC = gcc

h = ./headers/
c = ./sources/

ifeq ($(OS),Windows_NT)
	CFLAGS += -D WIN32
	SDL_DIR="C:\SDLib\SDL2
	SDL_LIB_DIR=${SDL_DIR}\lib"
	SDL_INC_DIR=${SDL_DIR}\include"
	LIBS=-L${SDL_LIB_DIR} -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2
	INCS=-I${SDL_INC_DIR}
else
	CFLAGS += -D unix
	SDL_DIR=${HOME}/SDL2
	SDL_LIB_DIR=${SDL_DIR}/lib
	SDL_INC_DIR=${SDL_DIR}/include
	LIBS=-L${SDL_LIB_DIR} -lSDL2 -lSDL2_image -lSDL2_ttf
	INCS=-I${SDL_INC_DIR}
endif

all: map testListe testsdl dep_matrice

map: codemap decodemap

testListe: liste.o testListe.o
	${CC} $^ -o $@ ${CFLAGS}

codemap: ${c}codemap.c
	${CC} $^ -o $@ ${CFLAGS}

decodemap: ${c}decodemap.c
	${CC} $^ -o $@ ${CFLAGS}

testsdl: sdl_fonctions.o test_SDL.o
	${CC} $^ -o $@ ${LIBS} ${INCS} ${CFLAGS}

dep_matrice: ${c}dep_matrice.c
	${CC} $^ -o $@ ${CFLAGS}

%.o: ${c}%.c
	${CC} -c $< ${LIBS} ${INCS} ${CFLAGS}

clean:
	$(if $(OS) == Windows_NT, del /s *.o, rm -rf *.o)

mrproper: clean
	$(if $(OS) == Windows_NT, del /s *.exe, find . -type f -executable -delete)
