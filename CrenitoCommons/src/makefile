RM=rm -rf
CC=gcc

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
H_SRCS=$(shell find . -iname "*.h" | tr '\n' ' ')
OBJS=$(C_SRCS:./%.c=build/%.o)



# Clean and compile .so
all: build/libCrenitoCommons.so

create-dirs:
	mkdir -p build/crenito-commons/
	mkdir -p ../log

build/libCrenitoCommons.so: create-dirs $(OBJS)
	$(CC) -shared -o "build/libCrenitoCommons.so" $(OBJS)

build/crenito-commons/%.o: crenito-commons/%.c
	$(CC) -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

# Add debug parameters and compile
debug: CC += -DDEBUG -g
debug: all

# Clean build files
clean:
	$(RM) build

install: all
	cp -u build/libCrenitoCommons.so /usr/lib
	cp --parents -u $(H_SRCS) /usr/include

uninstall:
	rm -f /usr/lib/libCrenitoCommons.so
	rm -rf /usr/include/CrenitoCommons

.PHONY: all create-dirs clean install uninstall
