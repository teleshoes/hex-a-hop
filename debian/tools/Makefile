CC=gcc
CFLAGS=-Wall -O2 `sdl-config --cflags`
LDFLAGS=`sdl-config --libs`
SOURCES=bmp2dat.c  dat2bmp.c 
OBJECTS=$(SOURCES:.c=.o)
PROGRAMS=$(SOURCES:.c=)

all: $(PROGRAMS)

%: %.o
	$(CC) $(LDFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROGRAMS)
	rm -f $(OBJECTS)

install:
	mkdir -p $(DESTDIR)/usr/bin
	cp $(PROGRAMS) $(DESTDIR)/usr/bin/
