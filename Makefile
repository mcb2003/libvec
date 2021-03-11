CFLAGS += -Wall -Wextra -Wsign-conversion -std=c18 -MD -D_DEFAULT_SOURCE
ARFLAGS = rcs

SOURCES = $(wildcard *.c)
OBJECTS = ${SOURCES:.c=.o}
	DEPS = $(OBJECTS:.o=.d)
	HEADERS = $(wildcard *.h)
LIBRARY = $(shell basename "${PWD}")

.PHONY: all release static shared clean fmt

all: CFLAGS += -DDEBUG -O0 -g
all: static shared

release: CFLAGS += -O3 -DNDEBUG
release: static shared
	strip ${LIBRARY}.so

static: ${LIBRARY}.a
    
${LIBRARY}.a: ${OBJECTS}
	${AR} ${ARFLAGS} ${LIBRARY}.a $^

shared: LDFLAGS += -shared
    shared: ${LIBRARY}.so

${LIBRARY}.so: ${OBJECTS}
	${LINK.C} -o ${LIBRARY}.so $^

clean:
	${RM} ${LIBRARY}.a ${LIBRARY}.so ${OBJECTS} ${DEPS}

fmt: ${SOURCES} ${HEADERS}
	clang-format -i $^

-include ${DEPS}
