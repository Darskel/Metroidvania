CFLAGS = -Wall
CC = gcc

h = ./headers/
c = ./sources/
o = ./o/

ifeq ($(OS),Windows_NT)
	CFLAGS += -D WIN32
	SDL_DIR=C:\SDLib\SDL2
	SDL_LIB_DIR=${SDL_DIR}\lib
	SDL_INC_DIR=${SDL_DIR}\include
	LIBS=-L${SDL_LIB_DIR} -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2
	INCS=-I${SDL_INC_DIR}
	clr=del /s *.o
	propre=del /s *.exe
else
	LIBS=`sdl2-config --cflags -libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2
	clr=rm -rf ./o/*.o
	propre=find . -type f -executable -delete
endif

all: testListe testsdl testSource testSprite testaffsalle testinit testManette

#map: codemap decodemap outdated/deprecated

testListe: ${o}liste.o ${o}testListe.o
	${CC} $^ -o $@ ${CFLAGS}

# outdated/deprecated programs
#codemap: ${c}codemap.c
#	${CC} $^ -o $@ ${CFLAGS}

#decodemap: ${c}decodemap.c
#	${CC} $^ -o $@ ${CFLAGS}

testsdl: ${o}sdl_fonctions.o ${o}test_SDL.o ${o}source.o ${o}liste.o ${o}comportement.o
	${CC} $^ -o $@  ${LIBS}

testManette : ${o}test_manette.o
	${CC} $^ -o $@  ${LIBS}

testSource: ${o}source.o ${o}testSource.o ${o}liste.o
	${CC} $^ -o $@ 

testSprite: ${o}source.o ${o}testSprite.o ${o}liste.o
	${CC} $^ -o $@ 

testaffsalle: ${o}liste.o ${o}source.o ${o}test_affichage_salle.o ${o}sdl_fonctions.o ${o}comportement.o
	${CC} $^ -o $@ ${LIBS}

testinit: ${o}liste.o ${o}source.o ${o}inits.o ${o}testInits.o
	${CC} $^ -o $@ ${LIBS}

testDepPers: ${o}testDepPers.o ${o}source.o ${o}comportement.o ${o}liste.o
	${CC} $^ -o $@  ${LIBS}

${o}%.o: ${c}%.c
	${CC} $< -c -o $@ ${LIBS} 

clean:
	$(clr)

mrproper: clean
	$(propre)
