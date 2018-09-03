PROYECTO=muestreo
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LFLAGS=-lm
CC=gcc

src = $(wildcard *.c)

all: $(PROYECTO)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

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
