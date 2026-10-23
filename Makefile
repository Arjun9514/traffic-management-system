CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRCS = src/main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = traffic_sim.exe

all: $(TARGET)

$(TARGET): $(OBJS)
.git$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%%.o: %%.cpp
.git$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
.gitdel /Q src\*.o $(TARGET) 2>nul
