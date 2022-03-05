CFLAGS=-std=c++14

SRC=$(wildcard src/*.cpp)
OBJS=$(addprefix build/,$(patsubst %.cpp, %.o, $(SRC)))

all: check lexer

.PHONY: check
check:
	if [ ! -d ./build ] ; then mkdir -p build; fi
	if [ ! -d ./build/src ] ; then mkdir -p build/src; fi

lexer: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o build/lexer
	
build/src/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean	
clean:
	rm build/lexer; \
	rm build/src/*.o;
	
