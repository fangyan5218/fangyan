
CXX = g++

COMMON_INCLUDE_DIR ?= ../include

BIN_DIR ?= ../bin
TARGET = $(BIN_DIR)/server_test
TARGET1 = $(BIN_DIR)/client_test

FLAGS ?= -Wall -finline-functions -O3 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 \
		-D__USE_LARGEFILE64 -D_GNU_SOURCE -D__USE_FILE_OFFSET64 -ggdb3
		
INCLUDE := -I. -I$(COMMON_INCLUDE_DIR)

CXXFLAGS ?= $(FLAGS) $(INCLUDE)  

object := ./socket.o ./socket_test.o ../log/source/log.o 
object1 := ./client_test.o ../log/source/log.o

.PHONY:all
all:$(TARGET) $(TARGET1)

$(TARGET):$(object)
	$(CXX) $? -o $@ $(CXXFLAGS)
	
$(TARGET1):$(object1)
	$(CXX) $? -o $@ $(CXXFLAGS)
	
.PHONY:clean
clean:
	-rm -f $(object) $(TARGET)
	-rm -f $(object1) $(TARGET1)
	
