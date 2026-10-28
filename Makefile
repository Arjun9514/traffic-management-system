CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
ifeq ($(OS),Windows_NT)
TARGET = traffic_sim.exe
else
TARGET = traffic_sim
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

ifeq ($(OS),Windows_NT)
clean:
	del /Q $(subst /,\,$(OBJS)) $(TARGET) 2>NUL
else
clean:
	$(RM) $(OBJS) $(TARGET)
endif