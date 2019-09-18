
CXX = g++

COMMON_INCLUDE_DIR ?= ../include

BIN_DIR ?= ../bin
TARGET = $(BIN_DIR)/mysql_service_test

FLAGS ?= -Wall -finline-functions -O3 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 \
		-D__USE_LARGEFILE64 -D_GNU_SOURCE -D__USE_FILE_OFFSET64 -ggdb3
		
INCLUDE := -I./include -I$(COMMON_INCLUDE_DIR) -I/usr/include/mysql 

LIBS := -lmysqlclient 

LIBS_DIR := -L/usr/lib/x86_64-linux-gnu

CXXFLAGS ?= $(FLAGS) $(INCLUDE) $(LIBS_DIR) $(LIBS)

object := ./source/mysql_service.o ./source/mysql_service_test.o ../log/source/log.o 

.PHONY:all
all:$(TARGET)

$(TARGET):$(object)
	$(CXX) $? -o $@ $(CXXFLAGS)

.PHONY:clean
clean:
	-rm -f $(object) $(TARGET)
	
