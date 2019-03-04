SOURCES=$(shell find src -name '*.cpp')
OBJS=$(SOURCES:src/%.cpp=build/%.o)
TEST_LIBS=build/file_util.o build/opengl_util.o
TEST_SOURCES=$(shell find test/src -name '*.cpp')
TEST_OBJS=$(TEST_SOURCES:test/src/%.c=test/build/%.o)
LIBS=$(shell pkg-config --static --libs x11 xrandr xi xxf86vm glew glfw3)
TARGET=tankarena3
CPPFLAGS=-std=c++11 -Werror -g -Isrc -DENABLE_LOG

.PHONY: build

default: build

build/%.o: src/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

build: check_dir $(OBJS)
	g++ $(CPPFLAGS) -o build/$(TARGET) $(OBJS) $(LIBS)

check_dir:
	if [ ! -d "build" ]; then \
		mkdir build; \
	fi

clean:
	rm -f build/*

test/build/%.o: test/src/%.c $(TEST_LIBS)
	g++ $(CPPFLAGS) -c -o $@ $<

test/build/test: $(TEST_OBJS) $(TEST_LIBS)
	g++ $(CPPFLAGS) -o $@ $(TEST_LIBS) $(TEST_OBJS) $(LIBS)

build_test: $(TEST_OBJS) test/build/test

clean_test:
	rm -f test/build/*

run_test:
	cd test/scripts; \
	./run_test.sh
