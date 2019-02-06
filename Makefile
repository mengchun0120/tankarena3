SOURCES=$(shell find src -name '*.c')
OBJS=$(SOURCES:src/%.c=build/%.o)
LIBS=$(shell pkg-config --static --libs x11 xrandr xi xxf86vm glew glfw3)
TARGET=tankarena3

.PHONY: build

default: build

build/%.o: src/%.c
	gcc -c -o $@ $<

build: $(OBJS)
	gcc -o build/$(TARGET) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) build/$(TARGET)
