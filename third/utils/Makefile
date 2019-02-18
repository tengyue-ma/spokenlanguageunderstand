CC = gcc
CPP = g++
LINK = g++
CFLAGS = -g -Wall -Werror -Wextra -fPIC
CPPFLAGS = -g -Wall -Werror -Wextra -fPIC
LIBS = -L/usr/local/lib -lgtest -lgtest_main
SRC = ./src/
INC = ./inc/
BIN = ./bin/
LIB = ./lib/

C__SOURCES = $(wildcard $(SRC)*.c)
CPPSOURCES = $(wildcard $(SRC)*.cpp)
C__OBJECT = $(wildcard $(OBJ)*.o)
CPPOBJECT = $(wildcard $(OBJ)*.o)
OBJECTS = $(patsubst %.c, %.o, $(C__SOURCES)) $(patsubst %.cpp, %.o, $(CPPSOURCES))
TARGET = $(BIN)sample_unittest

first: all

%.o: %.c
	    $(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	    $(CPP) -c $(CPPFLAGS) -I $(INC) -o $@ $<

all: $(TARGET)

$(TARGET): $(OBJECTS)
	    $(LINK) $(CPPFLAGS) -I $(INC) $(LIBS) -o $(TARGET) $(OBJECTS)
		$(CPP) -shared -fPIC -o $(LIB)libutils.so $(SRC)utils.o

.PHONY: clean

clean:
	    rm -f $(TARGET) $(OBJECTS)
