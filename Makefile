CC             = g++
INCLUDES       = -Iinclude -Iexternal/endless_events/include
CFLAGS         = $(INCLUDES) -c -Wall -pedantic -std=c++14 -fexec-charset=UTF-8 -finput-charset=UTF-8 -fdiagnostics-color=auto
ifdef RELEASE
  CFLAGS += -O2
endif

ifndef RELEASE
  CFLAGS += -ggdb
endif 


LIB_PATHS      = -Llib
LIBS           =
LDFLAGS        = ${LIB_PATHS} ${LIBS}
MAKE           = make


find = $(foreach dir,$(1),$(foreach d,$(wildcard $(dir)/*),$(call find,$(d),$(2))) $(wildcard $(dir)/$(strip $(2))))

SOURCES_LIB       = $(call find, src, *.cpp)
OBJECTS           = $(SOURCES:.cpp=.o)
OBJECTS_LIB       = $(SOURCES_LIB:.cpp=.o)
STATIC_TARGET     = lib/libendless_ui.a


.PHONY: all check example clean


all: $(SOURCES) $(STATIC_TARGET) $(EXECUTABLE_TARGET) example


$(STATIC_TARGET): $(OBJECTS_LIB)
	ar rcs $@ $(OBJECTS_LIB)


check: $(STATIC_TARGET)
	+$(MAKE) -C ./test check


example: $(STATIC_TARGET)
	+$(MAKE) -C ./example all


.cpp.o:
	$(CC) ${CFLAGS} $< -o $@


clean:
	$(MAKE) -C ./test clean
	$(MAKE) -C ./example clean
	rm -f $(OBJECTS_LIB)
	rm -f $(OBJECTS)
	rm -f $(STATIC_TARGET)
	rm -f $(EXECUTABLE_TARGET)
	rm -f gmon.out

