PROYECTO=muestreo
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LFLAGS=-lm
CC=gcc

src = $(wildcard *.c)

all: $(PROYECTO)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

# archivos.o: archivos.c
#	$(CC) -c archivos.c $(CFLAGS)
#
# procesamiento.o: procesamiento.c
#	$(CC) -c procesamiento.c $(CFLAGS)
#
# muestreo.o: muestreo.c 
#	$(CC) -c muestreo.c $(CFLAGS)
#
$(PROYECTO): $(src:%.c=%.o)
	$(CC) $^ -o $@ $(LFLAGS)
	

.PHONY: clean

install: all
	if [ ! -d $(DESTDIR) ] ; then \
	sudo mkdir $(DESTDIR); \
	fi; \
	sudo cp $(PROYECTO) $(DESTDIR)

unistall:
	rm /$(DESTDIR)/$(PROYECTO)

clean:
	 rm -f *.o $(PROYECTO)
