CXX:=$(shell sh -c 'type $(CXX) >/dev/null 2>/dev/null && echo $(CXX) || echo g++')
CFLAGS=-O2 --std=c++11

INC=-I ../inc \
	-I ../third/hiredis \
	-I ../third/hobotlog/include

LIB=-L ../third/hiredis -lhiredis \
	-L ../third/hobotlog/lib -llogmodule -llog4cpp -lpthread

OBJS=$(wildcard ../src/*.o)
TEST=semantic_parser

$(TEST): src/test.cc $(OBJS)
	$(CXX) $(CFLAGS) $< $(OBJS) -o $@ $(INC) $(LIB)

.PHONY: clean

clean:
	rm semantic_parser
