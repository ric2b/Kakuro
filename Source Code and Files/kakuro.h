#ifndef __KAKURO_H_
#define __KAKURO_H_

/* ficheiro que contem as estruturas de dados utilizadas */
 
typedef struct triangular{
	int soma;
	char orientacao;
	int num_casas;
	int pos_y;
	int pos_x;		
} triangular;
/* estrutura para armazenar informaçao relativamente a cada célula triangular */

typedef struct branca{
	int valores[9];
	triangular * superior;
	triangular * inferior;
	int pos_y;
	int pos_x;	
} branca;
/* estrutura para armazenar informaçao relativamente a cada célula branca */

typedef struct celula_matriz{
	triangular * triangulo_superior;
	triangular * triangulo_inferior;
	branca * casa;
} celula_matriz;
/* estrutura com ponteiros para as estruturas definidas anteriormente */

typedef struct fila_casas{
	int i;
	int j;
	struct fila_casas * anterior;
} fila_casas;



#endif
