$(CC) = g++
$(CPP) = g++

OBJS = src/http_parser.o
TEST = test/http_parser.o test/test.o
CFLAGS = -Isrc

all: $(OBJS) $(TEST)
	g++ $(OBJS) $(TEST) -o test_runner

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) $(TEST)
	$(RM) test_runner
