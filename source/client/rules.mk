ifeq ($(OS),Windows_NT)
LDFLAGS := $(LDFLAGS) -lws2_32
CXXFLAGS := $(CXXFLAGS) -D_WIN32_WINNT=0x0600 -DWINVER=0x0600
else
export SKIP := 1
endif
