CFLAGS = -Wall
CC = gcc

h = ./headers/
c = ./sources/
o = ./o/

ifeq ($(OS),Windows_NT)
	CFLAGS += -D WIN32
	SDL_DIR="C:\SDLib\SDL2
	SDL_LIB_DIR=${SDL_DIR}\lib"
	SDL_INC_DIR=${SDL_DIR}\include"
	LIBS=-L${SDL_LIB_DIR} -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2
	INCS=-I${SDL_INC_DIR}
	clr=del /s *.o
	propre=del /s *.exe
else
	CFLAGS += -D unix
	SDL_DIR=${HOME}/SDL2
	SDL_LIB_DIR=${SDL_DIR}/lib
	SDL_INC_DIR=${SDL_DIR}/include
	LIBS=-L${SDL_LIB_DIR} -lSDL2_image -lSDL2_ttf -lSDL2
	INCS=-I${SDL_INC_DIR}
	clr=rm -rf ./o/*.o
	propre=find . -type f -executable -delete
endif

all: map testListe testsdl dep_matrice testSource

map: codemap decodemap

testListe: ${o}liste.o ${o}testListe.o
	${CC} $^ -o $@ ${CFLAGS}

codemap: ${c}codemap.c
	${CC} $^ -o $@ ${CFLAGS}

decodemap: ${c}decodemap.c
	${CC} $^ -o $@ ${CFLAGS}

testsdl: ${o}sdl_fonctions.o ${o}test_SDL.o
	${CC} $^ -o $@ ${INCS} ${LIBS} ${CFLAGS}

testSource: ${o}source.o ${o}testSource.o ${o}liste.o
	${CC} $^ -o $@ ${CFLAGS}

${o}%.o: ${c}%.c
	${CC} $< -c -o $@ ${INCS} ${LIBS} ${CFLAGS}

clean:
	$(clr)

mrproper: clean
	$(propre)
