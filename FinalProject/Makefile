FINAL = fp
OUTPUTS = evidence.o ghost.o house.o hunter.o logger.o main.o room.o utils.o
GCC1 = gcc -o
GCC2 = gcc -Wall -Wextra -c

all: ${FINAL}

${FINAL}: ${OUTPUTS}
	$(GCC1) ${FINAL} ${OUTPUTS} -lm

evidence.o: evidence.c defs.h
	$(GCC2) evidence.c

ghost.o: ghost.c defs.h
	$(GCC2) ghost.c

house.o: house.c defs.h
	$(GCC2) house.c

hunter.o: hunter.c defs.h
	$(GCC2) hunter.c

logger.o: logger.c defs.h
	$(GCC2) logger.c

main.o: main.c defs.h
	$(GCC2) main.c

room.o: room.c defs.h
	$(GCC2) room.c

utils.o: utils.c defs.h
	$(GCC2) utils.c

clean:
	rm -f ${FINAL} ${OUTPUTS}