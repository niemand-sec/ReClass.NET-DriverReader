WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I../../ReClass.NET/NativeCore/
CFLAGS = -Wall -fpermissive -fPIC -std=c++14
RESINC =
LIBDIR =
LIB =
LDFLAGS = -shared

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/debug
DEP_DEBUG =
OUT_DEBUG = build/debug/native_plugin.so

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/release
DEP_RELEASE =
OUT_RELEASE = build/release/native_plugin.so

OBJ_DEBUG = $(OBJDIR_DEBUG)/dllmain.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/dllmain.o

all: debug release

clean: clean_debug clean_release

before_debug:
	test -d build/debug || mkdir -p build/debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug:

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(CXX) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/dllmain.o: dllmain.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c dllmain.cpp -o $(OBJDIR_DEBUG)/dllmain.o

clean_debug:
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf build/debug
	rm -rf $(OBJDIR_DEBUG)

before_release:
	test -d build/release || mkdir -p build/release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release:

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/dllmain.o: dllmain.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c dllmain.cpp -o $(OBJDIR_RELEASE)/dllmain.o

clean_release:
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf build/release
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release
