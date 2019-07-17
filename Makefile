SOURCE += tests/test_usbhid.cpp
SOURCE += usbhid/usbhid.cpp


SOURCE_FULL = $(addprefix src/, $(SOURCE))
OBJS = $(SOURCE_FULL:.cpp=.o)

CUR_PATH = $(shell echo $(PWD))
INCLUDEPATHS = $(addprefix -I$(CUR_PATH)/, $(dir $(SOURCE_FULL)))
LOADPATHS =
LIBS = -lusb-1.0
CPP = g++



#OBJS = $(addprefix src/objs/, ykushcmd.o commandParser.o ykushxs.o yk_usb_device.o ykush.o ykush_help.o ykush2.o ykush3.o command_parser.o string2val.o)
#LOADPATHS = -L$(CUR_PATH)/src/linux
#INCLUDEPATHS= -I$(CUR_PATH)/src/linux -I$(CUR_PATH)/src/ykushxs -I$(CUR_PATH)/src/ykush -I$(CUR_PATH)/src -I$(CUR_PATH)/src/help -I$(CUR_PATH)/src/ykush2 -I$(CUR_PATH)/src/ykush3 -I$(CUR_PATH)/src/utils


ykushcmd : $(OBJS)
	$(CPP) $(LOADPATHS) -o bin/$@ $(OBJS) $(LIBS)

$(OBJS) : %.o : %.cpp 
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c $< -o $@

clean :
	rm -f bin/ykushcmd $(OBJS)

