CFLAGS =-Wall
CC = gcc

h = ./headers/
c = ./sources/
o = ./o/
i = ./icon/

EXEC=diskosieni

ifeq ($(OS),Windows_NT)
	#CFLAGS += -D WIN32
	#SDL_DIR=C:\SDLib\SDL2
	#SDL_LIB_DIR=${SDL_DIR}\lib
	#SDL_INC_DIR=${SDL_DIR}\include
	RETIRERCONSOLE= -mwindows #pour retirer la console à l'exécution du programme
	ICONRES=${i}icon.res
	ICONRC=${i}icon.rc
	COMPILERICON=windres.exe -i ${ICONRC} -o ${ICONRES} --input-format=rc -O coff
	LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2 #-L${SDL_LIB_DIR} -I${SDL_INC_DIR}
	clr=del /s *.o
	clrres=del /s *.res
	propre=del /s *.exe
else
	LIBS=`sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2
	clr=rm -rf ./o/*.o
	clrres=rm -rf ./icon/*.res
	propre=rm -rf ${EXEC}
endif

all: mrproper ${EXEC}

${EXEC}: ${o}sdl_fonctions.o ${o}test_SDL.o ${o}source.o ${o}liste.o ${o}comportement.o
	${COMPILERICON}
	${CC} ${ICONRES} $^ ${CFLAGS} -o $@ ${LIBS}
	$(clr)
	$(clrres)

${o}%.o: ${c}%.c
	${CC} $< -c -o $@ ${LIBS}

clean:
	$(clr)
	$(clrres)

mrproper: clean
	$(propre)
