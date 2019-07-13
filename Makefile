CUR_PATH = $(shell echo $(PWD))
OBJS = $(addprefix src/objs/, ykushcmd.o commandParser.o ykushxs.o yk_usb_device.o ykush.o ykush_help.o ykush2.o ykush3.o command_parser.o string2val.o)
LIBS = -lhidapi-libusb -lusb-1.0 -ludev
LOADPATHS = -L$(CUR_PATH)/src/linux
INCLUDEPATHS= -I$(CUR_PATH)/src/linux -I$(CUR_PATH)/src/ykushxs -I$(CUR_PATH)/src/ykush -I$(CUR_PATH)/src -I$(CUR_PATH)/src/help -I$(CUR_PATH)/src/ykush2 -I$(CUR_PATH)/src/ykush3 -I$(CUR_PATH)/src/utils
CPP = g++

ykushcmd : $(OBJS)
	$(CPP) $(LOADPATHS) -o bin/ykushcmd $(OBJS) $(LIBS)

src/objs/ykushcmd.o : src/ykushcmd.cpp src/commandParser.h src/help/ykush_help.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/ykushcmd.cpp -o src/objs/ykushcmd.o

src/objs/commandParser.o : src/commandParser.cpp src/commandParser.h src/ykushxs/ykushxs.h src/ykush/ykush.h src/ykush2/ykush2.h 
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/commandParser.cpp -o src/objs/commandParser.o

src/objs/ykushxs.o : src/ykushxs/ykushxs.cpp src/ykushxs/ykushxs.h src/yk_usb_device.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/ykushxs/ykushxs.cpp -o src/objs/ykushxs.o

src/objs/ykush.o : src/ykush/ykush.cpp src/ykush/ykush.h src/yk_usb_device.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/ykush/ykush.cpp -o src/objs/ykush.o

src/objs/ykush2.o : src/ykush2/ykush2.cpp src/ykush2/ykush2.h src/yk_usb_device.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/ykush2/ykush2.cpp -o src/objs/ykush2.o

src/objs/ykush3.o : src/ykush3/ykush3.cpp src/ykush3/ykush3.h src/yk_usb_device.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/ykush3/ykush3.cpp -o src/objs/ykush3.o

src/objs/yk_usb_device.o : src/yk_usb_device.cpp src/yk_usb_device.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/yk_usb_device.cpp -o src/objs/yk_usb_device.o

src/objs/ykush_help.o : src/help/ykush_help.cpp src/help/ykush_help.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/help/ykush_help.cpp -o src/objs/ykush_help.o

src/objs/command_parser.o : src/utils/command_parser.cpp src/utils/command_parser.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/utils/command_parser.cpp -o src/objs/command_parser.o

src/objs/string2val.o : src/utils/string2val.cpp src/utils/string2val.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c src/utils/string2val.cpp -o src/objs/string2val.o


clean :
	rm -f bin/ykushcmd $(OBJS)

