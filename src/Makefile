CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o utils.o dataprocessing.o multiply.o singledatatransfer.o branch.o input.o

emulate: emulate.o utils.o outputReg.o dataprocessing.o singledatatransfer.o input.o execute.o decode.o

assemble.o: utils.h dataprocessing.h multiply.h singledatatransfer.h branch.h map.h input.h

emulate.o: emulate.h utils.h outputReg.h dataprocessing.h singledatatransfer.h input.h execute.h decode.h

branch.o: branch.h map.h utils.h

dataprocessing.o: utils.h dataprocessing.h

decode.o: utils.h decode.h

execute.o: execute.h utils.h dataprocessing.h singledatatransfer.h

input.o: input.h utils.h

multiply.o: multiply.h utils.h

outputReg.o: outputReg.h utils.h

singledatatransfer.o: singledatatransfer.h utils.h

utils.o: utils.h

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
