PIN_ROOT=$(HOME)/Desktop/pin-3.2-81205-clang-mac

INCLUDES := \
  -I$(PIN_ROOT)/source/include/pin \
  -I$(PIN_ROOT)/source/include/pin/gen \
  -I$(PIN_ROOT)/extras/components/include \
  -I$(PIN_ROOT)/extras/xed-intel64/include/xed \
  -I$(PIN_ROOT)/source/tools/InstLib

CPPFLAGS := \
  -isystem $(PIN_ROOT)/extras/stlport/include \
  -isystem $(PIN_ROOT)/extras/libstdc++/include \
  -isystem $(PIN_ROOT)/extras/crt/include \
  -isystem $(PIN_ROOT)/extras/crt/include/arch-x86_64 \
  -isystem $(PIN_ROOT)/extras/crt/include/kernel/uapi \
  -isystem $(PIN_ROOT)/extras/crt/include/kernel/uapi/asm-x86 \
  -D__PIN__=1 \
  -DPIN_CRT=1 \
  -DBIGARRAY_MULTIPLIER=1 \
  -DTARGET_IA32E \
  -DHOST_IA32E \
  -DTARGET_MAC \
  -D__DARWIN_ONLY_UNIX_CONFORMANCE=1 \
  -D__DARWIN_UNIX03=0

CXXFLAGS := \
  -fPIC \
  -fno-stack-protector \
  -fno-exceptions \
  -funwind-tables \
  -fno-rtti \
  -fomit-frame-pointer \
  -fno-strict-aliasing

LDFLAGS := \
  -nostdlib \
  -w -Wl,-exported_symbols_list,$(PIN_ROOT)/source/include/pin/pintool.exp \
  -L$(PIN_ROOT)/intel64/runtime/pincrt \
  -L$(PIN_ROOT)/intel64/lib \
  -L$(PIN_ROOT)/intel64/lib-ext \
  -L$(PIN_ROOT)/extras/xed-intel64/lib

LIBS := \
  -lpin \
  -lxed \
  -lpin3dwarf \
  -nostdlib \
  -lstlport-dynamic \
  -lm-dynamic \
  -lc-dynamic \
  -lunwind-dynamic

TARGET=test.dylib

all: $(TARGET)

SOURCE=test.cpp
OBJECT=$(SOURCE:.cpp=.o)

%.o: %.cpp
	g++ $(INCLUDES) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(TARGET): $(OBJECT) $(PIN_ROOT)/intel64/runtime/pincrt/crtbeginS.o
	g++ -shared -o $@ $+ $(LDFLAGS) $(LIBS)
