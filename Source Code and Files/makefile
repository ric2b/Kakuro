CFLAGS= -g -Wall -pedantic -std=c99

crosssums: combinacoes.o matrizes.o resolve.o kakuro.o 
	gcc $(CFLAGS) -o crosssums kakuro.o combinacoes.o matrizes.o resolve.o

kakuro.o: kakuro.h combinacoes.h resolve.h kakuro.c 
	gcc $(CFLAGS) -c kakuro.c
	
matrizes.o: kakuro.h combinacoes.h resolve.h matrizes.c
	gcc $(CFLAGS) -c matrizes.c

resolve.o: kakuro.h resolve.c
	gcc $(CFLAGS) -c resolve.c
	
combinacoes.o: combinacoes.c
	gcc $(CFLAGS) -c combinacoes.c

clean:
	rm -f *.o *.~ words *.gch
