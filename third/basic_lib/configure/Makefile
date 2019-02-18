ULLIB_PATH=../lib2-64/ullib
BSL_PATH=../lib2-64/bsl
SPREG_PAGH=../lib2-64/spreg

INCLUDEDIR= \
		-Iinclude \
		-I$(ULLIB_PATH)/include \
		-I$(BSL_PATH)/include \
		-I$(SPREG_PAGH)/include \

LIBDIR=	\
		-Llib -lconfig \
		-L$(ULLIB_PATH)/lib -lullib \
		-L$(BSL_PATH)/lib -lbsl \
		-L$(SPREG_PAGH)/lib -lspreg \
		-lcrypto -lpthread -lm -lstdc++ -lrt -lssl -ldl

GCC = g++
CPPFLAGS = -g -fPIC -finline-functions -Wall -Winline -pipe -ffast-math -D_LINUX_64_ 

TARGET = my_test

all: $(TARGET) mkout

$(TARGET): my_test.o 
	$(GCC) -g -o $@ $^ $(LIBDIR) $(INCLUDEDIR)

my_test.o : my_test.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

mkout:
	rm -rf *.o

clean:
	rm -rf *.o
	rm -rf core.*
	rm -rf $(TARGET)
