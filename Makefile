CFLAGS += -Wall -Wextra -Wsign-conversion -std=c18 -MD -D_DEFAULT_SOURCE
ARFLAGS = rcs

SOURCES = $(wildcard *.c)
OBJECTS = ${SOURCES:.c=.o}
	DEPS = $(OBJECTS:.o=.d)
	HEADERS = $(wildcard *.h)
LIBRARY = $(shell basename "${PWD}").a

.PHONY: all release clean fmt

all: CFLAGS += -DDEBUG -O0 -g
all: ${LIBRARY}

release: CFLAGS += -O3
release: ${LIBRARY}

${LIBRARY}: ${OBJECTS}
	${AR} ${ARFLAGS} $@ $^

clean:
	${RM} ${LIBRARY} ${OBJECTS} ${DEPS}

fmt: ${SOURCES} ${HEADERS}
	clang-format -i $^

-include ${DEPS}
