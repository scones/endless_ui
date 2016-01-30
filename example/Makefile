CC             = g++

INCLUDES       = -Iinclude -I../include -I/c/msys64/mingw64/include -I/c/msys64/mingw64/x86_64-w64-mingw32/include
EXECUTABLE_SUFFIX = 
ifeq ($(MSYSTEM), MINGW64)
	EXECUTABLE_SUFFIX = .exe
	INCLUDES += -I/mingw64/include/freetype2 -IC:/msys64/mingw64/include/freetype2
endif

ifeq ($(MSYSTEM), MSYS)
	EXECUTABLE_SUFFIX = .exe
	INCLUDES += -I/mingw64/include/freetype2 -IC:/msys64/mingw64/include/freetype2
endif

CFLAGS         = $(INCLUDES) -c -Wall -pedantic -std=c++14 -finput-charset=UTF-8 -DGLEW_STATIC -DDEBUG
ifdef RELEASE
  CFLAGS += -O2 -fomit-frame-pointer
endif

ifndef RELEASE
  CFLAGS += -ggdb
endif 

LIB_PATHS             = -Llib -L../lib
LIBS_STATIC_UI        = -lendless_ui
LIBS_STATIC_OPENGL    = -lglew32 -lglfw3 -lopengl32 -lgdi32
LIBS_STATIC_FREETYPE  = -lfreetype
LIBS_STATIC_BOOST     = -lboost_system-mt -lboost_program_options-mt -lboost_filesystem-mt
LIBS_STATIC_SYSTEM    =
LIBS_STATIC_JS        = -lduktape
LIBS_SHARED_SYSTEM    = -ldbghelp

ifeq ($(MSYSTEM), MINGW64)
	LIBS_STATIC_SYSTEM += -lwinmm -mwindows
endif

ifeq ($(MSYSTEM), MSYS)
	LIBS_STATIC_SYSTEM += -lwinmm -mwindows
endif


LIBS_STATIC    = -Wl,-Bstatic  $(LIBS_STATIC_UI) $(LIBS_STATIC_FREETYPE) $(LIBS_STATIC_OPENGL) $(LIBS_STATIC_SYSTEM) $(LIBS_STATIC_BOOST) $(LIBS_STATIC_SYSTEM) $(LIBS_STATIC_JS)
LIBS_SHARED    = -Wl,-Bdynamic $(LIBS_SHARED_SYSTEM)
LIBS           = $(LIBS_STATIC) $(LIBS_SHARED)

LIBS_LIB       =
LDFLAGS        = ${LIB_PATHS} ${LIBS}
LDFLAGS_LIB    = ${LIB_PATHS} ${LIBS_LIB}
MAKE           = make


find = $(foreach dir,$(1),$(foreach d,$(wildcard $(dir)/*),$(call find,$(d),$(2))) $(wildcard $(dir)/$(strip $(2))))

SOURCES_CORE       = $(call find, src/core, *.cpp)
SOURCES_BINARY     = $(wildcard src/*.cpp)
SOURCES_ALL        = $(SOURCES_CORE) $(SOURCES_BINARY)
OBJECTS_ALL        = $(patsubst %.cpp, %.o, $(SOURCES_ALL))
OBJECTS_CORE       = $(patsubst %.cpp, %.o, $(SOURCES_CORE))
OBJECTS_BINARY     = $(patsubst %.cpp, %.o, $(SOURCES_BINARY))
BINARY_TARGETS     = $(notdir $(patsubst %, %$(EXECUTABLE_SUFFIX), $(patsubst %.cpp, %, $(SOURCES_BINARY))))


.PHONY: all debug clean
.SECONDEXPANSION:


all: debug $(OBJECTS_CORE) $(BINARY_TARGETS)


$(BINARY_TARGETS): $(OBJECTS_CORE) $$(addprefix src/, $$(patsubst %.exe, %.o, $$@))
	$(CC) $^ -o $@ $(LDFLAGS)


debug:


.cpp.o:
	$(CC) ${CFLAGS} $< -o $@


clean:
	rm -f $(OBJECTS_ALL)
	rm -f $(BINARY_TARGETS)
	rm -f gmon.out
	$(MAKE) -C ./bin clean

