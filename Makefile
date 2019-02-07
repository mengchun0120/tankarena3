SOURCES=$(shell find src -name '*.c')
OBJS=$(SOURCES:src/%.c=build/%.o)
TEST_LIBS=build/file_util.o build/opengl_util.o
TEST_SOURCES=$(shell find test/src -name '*.c')
TEST_OBJS=$(TEST_SOURCES:test/src/%.c=test/build/%.o)
LIBS=$(shell pkg-config --static --libs x11 xrandr xi xxf86vm glew glfw3)
TARGET=tankarena3
CFLAGS=-Werror -g -Isrc

.PHONY: build

default: build

build/%.o: src/%.c
	gcc $(CFLAGS) -c -o $@ $<

build: $(OBJS)
	gcc $(CFLAGS) -o build/$(TARGET) $(OBJS) $(LIBS)

clean:
	rm -f build/*

test/build/%.o: test/src/%.c $(TEST_LIBS)
	gcc $(CFLAGS) -c -o $@ $<

test/build/test: $(TEST_OBJS) $(TEST_LIBS)
	gcc $(CFLAGS) -o $@ $(TEST_LIBS) $(TEST_OBJS) $(LIBS)

build_test: $(TEST_OBJS) test/build/test

clean_test:
	rm -f test/build/*

run_test:
	cd test/scripts; \
	./run_test.sh
