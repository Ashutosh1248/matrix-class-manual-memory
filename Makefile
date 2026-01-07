CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra -march=native

Target = demo
SRC = demo.cpp matrix.cpp

all: $(TARGET)
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
