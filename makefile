AR ?= ar
CC ?= gcc
INSTALL_DIR ?= /usr/local

LIBNAME = nativemsg

BUILD_CFLAGS = -Wall -O3 -flto -Iinclude
TEST_CFLAGS = $(BUILD_CFLAGS) -Lbuild/ -l$(LIBNAME)

SRC = src/nativemsg.c
OBJ = $(SRC:.c=.o)
HEADER = nativemsg

%.o: %.c
	$(CC) -c -o $@ $< $(BUILD_CFLAGS)

build: $(OBJ)
	@mkdir build
	$(AR) rcs $@/lib$(LIBNAME).a $^

install: build
	cp -f $^/lib$(LIBNAME).a $(INSTALL_DIR)/lib/lib$(LIBNAME).a
	cp -f include/$(HEADER).h $(INSTALL_DIR)/include/$(HEADER).h

uninstall:
	rm -f $(INSTALL_DIR)/lib/lib$(LIBNAME).a
	rm -f $(INSTALL_DIR)/include/$(HEADER).h

clean:
	rm -fr build ./src/*.o ./test_src/*o ./test

test: build
	$(CC) -o $@ test_src/tester.c $(TEST_CFLAGS)
