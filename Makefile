CXX=clang++
CFLAGS=-Wall -pedantic -std=c++11
FILES=hicolor.o ppm.o hi2ppm.o ppm2hi.o

all: compile

run: compile
	./hicolor

example: compile
	./hicolor examples/*.HI

compile: $(FILES)
	$(CXX) -o hicolor $(FILES)

clean:
	rm -rf hicolor $(FILES) examples/*.ppm

hicolor.o: src/hicolor.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

ppm.o: src/ppm.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

hi2ppm.o: src/hi2ppm.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

ppm2hi.o: src/ppm2hi.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<