sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)

all: friday

friday: $(objects)
	g++ -o $@ $^

src/%.o: %.cpp 
	g++ -c $?

clean:
	rm friday ./src/*.o