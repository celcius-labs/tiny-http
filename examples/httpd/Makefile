CFLAGS=-I../../src -Wall -O3
LDFLAGS=-L../.. -ltinyhttp -O3

all: httpd

httpd.o: httpd.c
	$(CC) -c $(CFLAGS) httpd.c -o httpd.o

httpd: httpd.o
	$(CC) $(LDFLAGS) httpd.o -o httpd

clean:
	@rm -f *.o httpd
