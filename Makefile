CFLAGS = #ajouter des flags ici
CC = gcc

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

all: map

map: codemap decodemap

codemap: codemap.c
	${CC} $^ -o $@ ${CFLAGS}

decodemap: decodemap.c
	${CC} $^ -o $@ ${CFLAGS}

%.o: %.c
	${CC} $< -c -o $@ ${CFLAGS}

clean:
	$(if $(OS) == Windows_NT, del /s *.o, rm -rf *.o)

mrproper: clean
	$(if $(OS) == Windows_NT, del /s *.exe, find . -type f -executable -delete)