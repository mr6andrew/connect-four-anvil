CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Werror

.PHONY: all clean

all: connect_four_anvil

connect_four_anvil: a5.cpp
	$(CXX) $(CXXFLAGS) a5.cpp -o connect_four_anvil

clean:
	rm -f connect_four_anvil
