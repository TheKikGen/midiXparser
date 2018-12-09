<<<<<<< HEAD
CXX = g++
=======
XX = g++
>>>>>>> 86af314095ba28d3079c2ab310b1e9387f0fdb54
CXXFLAGS = -Wall -g -std=gnu++11

SRCS =  test-mxp.cpp ../midiXparser.cpp
OBJS = ${SRCS:.cpp=.o}
HEADERS = ../midiXparser.h

MAIN = tst-midixparser

all: ${MAIN}
	@echo   Tests for $(MAIN) was succefully compiled

${MAIN}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${MAIN}

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	${RM} ${PROGS} ${OBJS} *.o *~.
