NAME=library
V_MAJOR=1
V_MINOR=0
V_MICRO=0
LIB_NAME=lib${NAME}.so
LIB_PATH=out/${LIB_NAME}
LIB_VER=${LIB_PATH}.${V_MAJOR}.${V_MINOR}.${V_MICRO}

CXX=g++
LD_FLAGS=-Wl,-soname,${LIB_NAME} -shared
CXX_FLAGS=-std=c++17 -fPIC -Wall -Wextra -pedantic

TESTS_LD_FLAGS=${LIB_VER}
TESTS_CXX_FLAGS=-std=c++17 -Wall -Wextra -pedantic -Isrc/

SOURCES=$(wildcard src/*.cpp)
HEADERS=$(wildcard src/*.hpp)
OBJECTS=$(patsubst src/%,build/%,${SOURCES:.cpp=.o})
DIRS=$(sort $(dir ${OBJECTS})) build/include

TEST_SOURCES=$(wildcard tests/*.cpp)
TEST_OBJECTS=$(addprefix build/,${TEST_SOURCES:.cpp=.o})
TESTS=$(addprefix out/,${TEST_SOURCES:.cpp=})

DIRS+=$(sort $(dir ${TEST_OBJECTS})) $(sort $(dir ${TESTS}))

ifdef DEBUG
CXX_FLAGS += -g -DDEBUG
TESTS_CXX_FLAGS += -g -DDEBUG
endif
ifndef DEBUG
CXX_FLAGS += -O2
TESTS_CXX_FLAGS += -O2
endif

.PHONY: all
all: dirs ${LIB_VER}

.PHONY: tests
tests: all ${TESTS}

runtest_%: out/tests/% all
	LD_PRELOAD=${LIB_VER} $< ${ARGS}

${LIB_VER}: ${OBJECTS}
	${CXX} $^ ${LD_FLAGS} -o $@

build/%.o: src/%.cpp
	${CXX} $< ${CXX_FLAGS} -c -o $@

out/tests/%: build/tests/%.o
	${CXX} $^ ${TESTS_LD_FLAGS} -o $@

build/tests/%.o: tests/%.cpp
	${CXX} $< ${TESTS_CXX_FLAGS} -c -o $@

.PHONY: dirs
dirs:
	mkdir -p ${DIRS}
	mkdir -p out/

.PHONY: clean
clean:
	-rm -r build/
	-rm -r out/

.PHONY: install
install:
	cp ${LIB_VER} /usr/local/lib
	cp -r ${HEADERS} /usr/local/include/${NAME}

.PHONY: uninstall
uninstall:
	rm /usr/local/lib/${LIB_NAME}*
	rm -r /usr/local/include/${NAME}
