CFLAGS= -g

HEADERS = tm_curl.h tm_kstat_data.h
SOURCES = tm_curl.c tm_kstat_data.c
OBJECTS:= $(SOURCES:.c=.o)

all: libtmcurl.a

libtmcurl.a: $(OBJECTS)
	ar r libtmcurl.a $(OBJECTS)

clean:
	-rm -f *.a *.o *~

