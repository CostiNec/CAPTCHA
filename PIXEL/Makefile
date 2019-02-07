build: task1.o task2.o
	gcc task1.o -o task1 -lm
	gcc task2.o -o task2 -lm
task1.o: main.c
	gcc -c main.c -o task1.o -lm
task2.o: main2.c
	gcc -c main2.c -o task2.o -lm
clean:
	rm *.o
	rm task1
	rm task2
run:
	./task1
	./task2
