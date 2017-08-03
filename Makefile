CUR_PATH = $(shell echo $(PWD))
OBJS = $(addprefix ykushcmd/objs/,ykushcmd.o commandParser.o usbcom.o)
CPP = g++

ifeq ($(shell uname),Darwin)
LIBS = $(shell pkg-config --libs hidapi) -lusb-1.0
PLATFORM_DEFS = -DOSX $(shell pkg-config --cflags hidapi)
endif

ifeq ($(shell uname),Linux)
LIBS = $(shell pkg-config --libs hidapi-libusb) -lusb-1.0 -ludev
PLATFORM_DEFS = -DLINUX $(shell pkg-config --cflags hidapi-libusb)
endif

all : bin/ykushcmd

bin/ykushcmd : $(OBJS)
	$(CPP) -o bin/ykushcmd $(OBJS) $(LIBS)

ykushcmd/objs/ykushcmd.o : ykushcmd/ykushcmd.cpp ykushcmd/commandParser.h
	$(CPP) $(PLATFORM_DEFS) -c ykushcmd/ykushcmd.cpp -o ykushcmd/objs/ykushcmd.o

ykushcmd/objs/commandParser.o : ykushcmd/commandParser.cpp ykushcmd/commandParser.h ykushcmd/usbcom.h 
	$(CPP) $(PLATFORM_DEFS) -c ykushcmd/commandParser.cpp -o ykushcmd/objs/commandParser.o

ykushcmd/objs/usbcom.o : ykushcmd/usbcom.cpp ykushcmd/usbcom.h
	$(CPP) $(PLATFORM_DEFS) -c ykushcmd/usbcom.cpp -o ykushcmd/objs/usbcom.o

clean :
	rm -f bin/ykushcmd $(OBJS)

install : bin/ykushcmd
	install -m755 $< /usr/local/bin

