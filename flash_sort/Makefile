objects_1 = main.cpp
target_1 = main
option = -std=gnu++11 -g -O2 -pthread -march=corei7-avx -Wall -Werror
objects_2 = tmp.cpp
target_2 = tmp

all: $(target_1) $(target_2)
.PHONY: clean

$(target_1): $(objects_1)
	g++ $(objects_1) -o $(target_1) $(option)

$(target_2): $(objects_2)
	g++ $(objects_2) -o $(target_2) $(option)

clean: 
	rm -rf $(target_1)
	rm -rf $(target_2)

