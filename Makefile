CC=g++
$(CPP) = g++

OBJS = src/http_parser.o
TEST = test/http_parser.o test/test.o
CFLAGS = -Isrc

all: build

build: $(OBJS) $(TEST)
	$(AR) -cvq libtinyhttp.a $(OBJS)
	$(CC) $(OBJS) $(TEST) -o test_runner

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) $(TEST)
	$(RM) test_runner
	$(RM) libtinyhttp.a

test: build
	./test_runner -s
