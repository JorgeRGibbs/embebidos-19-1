#Archivo Makefile con onjetivos falsos y valriables
Datos_seno=seno.dat
comodin=$(patsubst %.c,%.o,$(wildcard *.c))
PROYECTO=muestreo
CFLAGS=-Wall
DESTDIR=/usr/local/bin
LDFLAGS=-lm
CC=gcc

all: $(PROYECTO)

%.o : %.c
	$(CC) -c $< $(CFLAGS)

$(PROYECTO): $(comodin)
	$(CC) $^ -o $@ $(LDFLAGS)


.PHONY: clean

install: all
	if [ ! -d $(DESTDIR) ]; then \
		sudo mkdir $(DESTDIR); \
	fi; \
	sudo cp $(PROYECTO) $(DESTDIR)

uninstall:
	sudo rm $(DESTDIR)/$(PROYECTO)

clean:
	rm -f *.o $(PROYECTO) $(Datos_seno)