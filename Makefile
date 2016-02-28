CC=g++
$(CPP) = g++

OBJS = src/http_request.o src/http_response.o src/http_router.o
TEST = test/http_request.o test/http_response.o test/http_router.o test/test.o
CFLAGS = -Isrc -Wall -Werror -march=native -O3 -g
LDFLAGS = -O3 -g

all: build

build: $(OBJS) $(TEST)
	$(AR) -cvq libtinyhttp.a $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(TEST) -o test_runner

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
	$(CC) $(LDFLAGS) benchmarks/simple.cpp -Isrc -L. -ltinyhttp -o simple_benchmark

benchmark: build simple_benchmark
	./simple_benchmark
