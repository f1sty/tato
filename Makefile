SRC = tato.c
OBJ = ${SRC:.c=.o}
CFLAGS = -ggdb -Wall -Wextra ${shell pkg-config --cflags ncurses}
LDFLAGS = ${shell pkg-config --libs ncurses}
CC = gcc
DESTDIR = ${HOME}/.local

all: tato

options:
	@echo dwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h

config.h:
	cp config.def.h $@

tato: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f tato ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f deadpan ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/tato

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/tato

.PHONY: all options clean install uninstall
