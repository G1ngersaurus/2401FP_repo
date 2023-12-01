EXECS = p1 p2 p3 p4 p5 p6 p7 p8
GCC = gcc -o
WARNS = -Wall -Wextra

all: $(EXECS)

p1:	room.c
	$(GCC) $(WARNS) p1 room.c

p2:	evidence.c
	$(GCC) $(WARNS) p2 evidence.c

p3:	hunter.c
	$(GCC) $(WARNS) p3 hunter.c

p4:	house.c
	$(GCC) $(WARNS) p4 house.c

p5:	ghost.c
	$(GCC) $(WARNS) p5 ghost.c

p6:	main.c
	$(GCC) $(WARNS) p6 main.c

p7:	utils.c
	$(GCC) $(WARNS) p7 utils.c

p8:	logger.c
	$(GCC) $(WARNS) p8 logger.c

clean:
	rm -f $(EXECS)