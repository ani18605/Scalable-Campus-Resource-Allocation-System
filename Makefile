CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRCS = main.cpp SegmentTree.cpp TimeTree.cpp RoomManager.cpp ClassroomManager.cpp Allocator.cpp Reporter.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = campus_allocator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
