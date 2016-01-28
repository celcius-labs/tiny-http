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
	$(RM) simple_benchmark

test: build
	./test_runner -s

simple_benchmark:
	$(CC) benchmarks/simple.cpp -Isrc -L. -ltinyhttp -o simple_benchmark

benchmark: build simple_benchmark
	./simple_benchmark
