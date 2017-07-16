# DEBUG_LEVEL can have three values:
# * DEBUG_LEVEL=2; this is the ultimate debug mode. It will compile 
# without any optimizations. 
#
# * DEBUG_LEVEL=1; debug level 1 enables all assertions and debug code, but
# compiles with -O2 optimizations. this is the default debug level.
# We use this debug level when developing.
#
# * DEBUG_LEVEL=0; this is the debug level we use for release. 

# Set the default DEBUG_LEVEL to 1
DEBUG_LEVEL?=1

# compile with -O2 if debug level is not 2
ifneq ($(DEBUG_LEVEL), 2)
OPT += -O2 -fno-omit-frame-pointer
# Skip for archs that don't support -momit-leaf-frame-pointer
ifeq (,$(shell $(CXX) -fsyntax-only -momit-leaf-frame-pointer -xc /dev/null 2>&1))
OPT += -momit-leaf-frame-pointer
endif
endif

# if we're compiling for release, compile without debug code (-DNDEBUG) and
# don't treat warnings as errors
ifeq ($(DEBUG_LEVEL),0)
OPT += -DNDEBUG
DISABLE_WARNING_AS_ERROR=1
else
$(warning Warning: Compiling in debug mode. Don't use the resulting binary in production)
endif

# Valgrind options for leak checking
# VALGRIND_ERROR = 2
# VALGRIND_VER := $(join $(VALGRIND_VER),valgrind)
# VALGRIND_OPTS = --error-exitcode=$(VALGRIND_ERROR) --leak-check=full

# define source files
# include src.mk
src = $(wildcard *.cc ./*/*.cc)
dir = $(notdir $(src))
obj = $(patsubst %.cc, %.o, $(dir))

test_src = $(wildcard *.cc ./*/*_test.cc)
test_src_name= $(notdir $(test_src))
test_obj= $(test_src_name:.cc=.o) # replace .cc to .o
test_bin= $(test_src_name:.cc=)

INCLUDE_PATH = -I./include

LIB_PATH = -L./

LIBS = -lpthread

# compiler options
CXX = g++
CXXFLAGS = -Wall
CXXFLAGS += $(OPT)
CXXFLAGS += -Woverloaded-virtual -Wnon-virtual-dtor -Wno-missing-field-initializers

# auto compile all test units 
all: $(test_bin)

$(test_bin): 
	@echo ""
	@echo "[INFO] compile [$@]: "
	$(MAKE) $@.o	
	$(CXX)  -o $@ $@.o $(INCLUDE_PATH) $(CXXFLAGS)

$(test_obj): 
	$(CXX) -c src/$(@:.o=.cc) $(INCLUDE_PATH) $(CXXFLAGS)

info:
	@echo $@
	@echo "- DEBUG_LEVEL: $(DEBUG_LEVEL)"
	@echo "- OPT: $(OPT)"
	@echo $(INCLUDE_PATH)
	@echo $(src)
	@echo $(test_bin)
	@echo $(test_obj)
	@echo $(dir)
	@echo $(obj)
	@echo "end"

clean:
	rm *.o
	rm *_test

.PHONY: all clean
