CC             = g++

INCLUDES       = -Iinclude -I/c/msys64/mingw64/include -I/c/msys64/mingw64/x86_64-w64-mingw32/include
EXECUTABLE_SUFFIX = 
ifeq ($(MSYSTEM), MINGW64)
	EXECUTABLE_SUFFIX = .exe
	INCLUDES += -I/mingw64/include/freetype2 -IC:/msys64/mingw64/include/freetype2
endif

ifeq ($(MSYSTEM), MSYS)
	EXECUTABLE_SUFFIX = .exe
	INCLUDES += -I/mingw64/include/freetype2 -IC:/msys64/mingw64/include/freetype2
endif

CFLAGS         = $(INCLUDES) -O1 -c -Wall -pedantic -std=c++11 -finput-charset=UTF-8 -DGLEW_STATIC -DDEBUG -ggdb

LIB_PATHS      = -Llib
LIBS_STATIC_OPENGL    = -lglew32 -lglfw3 -lopengl32 -lgdi32
LIBS_STATIC_FREETYPE  = -lfreetype64
LIBS_STATIC_BOOST     = -lboost_system-mt -lboost_program_options-mt -lboost_filesystem-mt
LIBS_STATIC_SYSTEM    = -lwinmm
LIBS_STATIC_JS        = -lduktape
LIBS_SHARED_SYSTEM    =

ifeq ($(MSYSTEM), MINGW64)
	LIBS_SYSTEM += -lwinmm -mwindows
else
endif

LIBS_STATIC    = -Wl,-Bstatic  $(LIBS_STATIC_FREETYPE) $(LIBS_STATIC_OPENGL) $(LIBS_STATIC_SYSTEM) $(LIBS_STATIC_BOOST) $(LIBS_STATIC_SYSTEM) $(LIBS_STATIC_JS)
LIBS_SHARED    = -Wl,-Bdynamic $(LIBS_SHARED_SYSTEM)
LIBS           = $(LIBS_STATIC) $(LIBS_SHARED)

LIBS_LIB       =
LDFLAGS        = ${LIB_PATHS} ${LIBS}
LDFLAGS_LIB    = ${LIB_PATHS} ${LIBS_LIB}
MAKE           = make


find = $(foreach dir,$(1),$(foreach d,$(wildcard $(dir)/*),$(call find,$(d),$(2))) $(wildcard $(dir)/$(strip $(2))))

SOURCES            = $(call find, src, *.cpp)
OBJECTS            = $(SOURCES:.cpp=.o)
EXECUTABLE_SOURCES = foo.cpp bar.cpp
EXECUTABLE_TARGETS = $(patsubst %.cpp, %, $(EXECUTABLE_SOURCES))
EXECUTABLE_OBJECTS = $(patsubst %.cpp, %.o, $(EXECUTABLE_SOURCES))
BINARY_TARGETS     = $(patsubst %, %$(EXECUTABLE_SUFFIX), $(EXECUTABLE_TARGETS))

FOO = $(call pwd)

.PHONY: all clean


all: $(OBJECTS) $(BINARY_TARGETS)


.SECONDEXPANSION:
EXECUTABLE_OBJECT = $(patsubst %$(EXECUTABLE_SUFFIX),%.o,$@)
EXECUTABLE_BINARY = $(patsubst %$(EXECUTABLE_SUFFIX),bin/%$(EXECUTABLE_SUFFIX),$@)


%$(EXECUTABLE_SUFFIX): $(OBJECTS) $(EXECUTABLE_OBJECTS)
	$(CC) $(OBJECTS) $(EXECUTABLE_OBJECT) -o $(EXECUTABLE_BINARY) $(LDFLAGS)


$(STATIC_TARGET): $(OBJECTS_LIB)
	ar rcs $@ $(OBJECTS_LIB)


.cpp.o:
	$(CC) ${CFLAGS} $< -o $@


clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE_OBJECTS)
	rm -f $(BINARY_TARGETS)
	rm -f gmon.out
	$(MAKE) -C ./bin clean

