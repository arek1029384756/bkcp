TARGET=bkcp
all:
	g++ -Wall -Wextra -pedantic -O2 -std=c++11 main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
