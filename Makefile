CFLAGS = -Wall
CC = gcc


SDL_DIR=${HOME}/SDL2
SDL_LIB_DIR=${SDL_DIR}/lib
SDL_INC_DIR=${SDL_DIR}/include
LIBS=-L${SDL_LIB_DIR} -lSDL2 -lSDL2_image -lSDL2_ttf
INCS=-I${SDL_INC_DIR}


ifeq ($(OS),Windows_NT)
	CFLAGS += -D WIN32
else
	ifeq ($(UNAME), Linux)
		CFLAGS += -D unix
	endif
	ifeq ($(UNAME), Linux)
		CFLAGS += -D unix
	endif
endif

all: map testListe testsdl

map: codemap decodemap

testListe: liste.o testListe.o
	${CC} $^ -o $@ ${CFLAGS}

codemap: codemap.c
	${CC} $^ -o $@ ${CFLAGS}

decodemap: decodemap.c
	${CC} $^ -o $@ ${CFLAGS}

testsdl: sdl_fonctions.o test_SDL.o
	${CC} $^ -o $@ ${LIBS} ${INCS} ${CFLAGS}

%.o: %.c
	${CC} $< -c -o $@ ${LIBS} ${INCS} ${CFLAGS}

clean:
	$(if $(OS) == Windows_NT, del /s *.o, rm -rf *.o)

mrproper: clean
	$(if $(OS) == Windows_NT, del /s *.exe, find . -type f -executable -delete)
