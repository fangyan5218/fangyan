
CXX = g++

COMMON_INCLUDE_DIR ?= ../include

BIN_DIR ?= ../bin
TARGET = $(BIN_DIR)/log_test

FLAGS ?= -Wall -finline-functions -O3 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 \
		-D__USE_LARGEFILE64 -D_GNU_SOURCE -D__USE_FILE_OFFSET64 -ggdb3
		
INCLUDE := -I./include -I$(COMMON_INCLUDE_DIR)

CXXFLAGS ?= $(FLAGS) $(INCLUDE)

object := ./source/log.o ./source/log_test.o

.PHONY:all
all:$(TARGET)

$(TARGET):$(object)
	$(CXX) $(CXXFLAGS) $? -o $@

.PHONY:clean install
clean:
	-rm -f $(object) $(TARGET)
	
install: