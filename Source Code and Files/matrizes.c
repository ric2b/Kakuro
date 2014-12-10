#include "kakuro.h"
#include "resolve.h"
#include "combinacoes.h"

/* ficheiro que contem as varias funcoes associadas a manipulacao, analise e libertacao de matrizes */

branca * cria_branca(int pos_y, int pos_x, int altura, int largura, celula_matriz matriz_estruturas[altura][largura]){
/* funcao que aloca e preenche a estrutura de cada celula branca */
	 branca* novo;
	 int i,j;
	
	novo = (branca*) malloc(sizeof(branca));
	/* aloca espaco necessario para uma estrutura deste tipo */
	if (novo == NULL){
		printf("erro: memoria cheia\n");
		exit(-1);
	}	
	
	for(i=0; i<9; i++){
		novo->valores[i] = 1;
	} /* inicializa o vector das hipoteses possiveis para a branca a 1s */

	novo->superior = NULL; /* ponteiro para a triangular superior inicializado a NULL */
	for(j=pos_x-1; j >= 0; j--){
		if(matriz_estruturas[pos_y][j].triangulo_superior != NULL){
		/* se verificar que existe triangular superior para essa branca */
			novo->superior = matriz_estruturas[pos_y][j].triangulo_superior; 
			/* altera ponteiro para passar a apontar para a triangular correspondente */
			break;
		}			
	}
	
	novo->inferior = NULL; /* ponteiro para a triangular inferior inicializado a NULL */
	for(i=pos_y-1; i >= 0; i--){
		if(matriz_estruturas[i][pos_x].triangulo_inferior != NULL){
		/* se verificar que existe triangular superior para essa branca */
			novo->inferior = matriz_estruturas[i][pos_x].triangulo_inferior;
			/* altera ponteiro para passar a apontar para a triangular correspondente */
			break;
		}			
	}	
	
	novo->pos_y = pos_y; /* guarda aqui o indice da matriz na vertical */
	novo->pos_x = pos_x; /* guarda aqui o indice da matriz na horizontal */	
	
	return novo; /* devolve ponteiro para a estrutura */
}

triangular * cria_triangulo(int valor, char orientacao, int pos_y, int pos_x, int altura, int largura, int matriz[altura][largura]){
/* funcao que aloca e preenche a estrutura de cada celula triangular */
	triangular * novo;
	int i,j;
	int casas;
	
	novo = (triangular*) malloc(sizeof(triangular));
	/* aloca o espaco necessario para uma estrutura deste tipo */
	if (novo == NULL){
		printf("erro: memoria cheia\n");
		exit(-1);
	}
		
	novo->soma = valor; /* guarda o valor da soma */
	novo->orientacao = orientacao; /* guarda a orientacao da soma da triangular, 'h' se for na horizontal e 'v' se for na vertical */
	
	if(orientacao == 'h'){
		casas=0;
		for(j=pos_x+1; j < largura; j++){
			if(matriz[pos_y][j] != -1){
				break;
			}
			casas++;
		} /* conta o numero de casas brancas que constituem essa soma */
		novo->num_casas = casas;			
	}
	if(orientacao == 'v'){
		casas=0;
		for(i=pos_y+1; i < altura; i++){
			if(matriz[i][pos_x] != -1){
				break;
			}
			casas++;
		} /* conta o numero de casas brancas que constituem essa soma */
		novo->num_casas = casas;				
	}	
	
	novo->pos_x = pos_x; /* guarda aqui o indice da matriz na vertical */
	novo->pos_y = pos_y; /* guarda aqui o indice da matriz na horizontal */
	
	return novo; /* devolve ponteiro para a estrutura */
}

void cria_matriz_estruturas(int altura, int largura, int matriz[altura][largura], celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que altera a matriz de estruturas, em que cada estrutura é um ponteiro para estruturas */
	int i,j, k;
	int valor;
	
	for(i=0; i < altura; i++){
		for(j=0; j < largura; j++){
			/* celula branca */
			if(matriz[i][j] == -1){
				matriz_estruturas[i][j].triangulo_superior = NULL; /* ponteiros das estruturas triangulares a NULL */
				matriz_estruturas[i][j].triangulo_inferior = NULL; /* ponteiros das estruturas triangulares a NULL */
				matriz_estruturas[i][j].casa = cria_branca(i,j, altura, largura, matriz_estruturas); /* chama a funcao que preenche a estrutura relativamente a uma celula branca */
			}
			else{
				valor = matriz[i][j];
				if (valor != 0){
					int inferior = (int) (valor/100); /* calcula o valor da soma inferior */ 
					int superior = valor - inferior*100; /* calcula o valor da soma superior */		
					
					/*dependendo do caso vai chamar a funcao que preenche a estrutura relativamente a uma celula triangular */
					
					/* triangular de dois tipos */
					if(inferior != 0 && superior != 0){
						matriz_estruturas[i][j].triangulo_superior = cria_triangulo(superior, 'h', i, j, altura, largura, matriz);						
						matriz_estruturas[i][j].triangulo_inferior = cria_triangulo(inferior, 'v', i, j, altura, largura, matriz);
					}
					/* triangular inferior */
					if(inferior != 0 && superior == 0){
						matriz_estruturas[i][j].triangulo_superior = NULL;
						matriz_estruturas[i][j].triangulo_inferior = cria_triangulo(inferior, 'v', i, j, altura, largura, matriz);
					}
					/* triangular superior */	
					if(inferior == 0 && superior != 0){
						matriz_estruturas[i][j].triangulo_superior = cria_triangulo(superior, 'h', i, j, altura, largura, matriz);
						matriz_estruturas[i][j].triangulo_inferior = NULL;
					}
										
					matriz_estruturas[i][j].casa = NULL; /* para as triangulares o ponteio para a estrutura de uma casa do tipo branca e NULL */
				}
				else{
					/* celula preta, ponteiros todos a NULL */
					matriz_estruturas[i][j].triangulo_superior = NULL;
					matriz_estruturas[i][j].triangulo_inferior = NULL;
					matriz_estruturas[i][j].casa = NULL;
				}
			}		
		}		
	}
	
	for(i=0; i < altura; i++){
		for(j=0; j < largura; j++){
			if(matriz[i][j] == -1){
				if(cruza_somas(i, j, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				/* funcao cruza_somas devolveu 1 portanto a casa so tem um valor possivel */
					for(k=0; k<9; k++){
						if(matriz_estruturas[i][j].casa->valores[k] == 1){ /* procura onde esta esse valor no vecto de hipoteses da branca */
							matriz_solucao[i][j] = k+1; /* preenche a solucao na matriz de solucao no respectivo sitio */
						}
					}
				}
			}
		}
	}
	
	return; /* funcao e void, nao precisa de devolver a matriz ou um ponteiro para esta, pois ela e recebida por referencia como argumento da funcao, e alterada dentro desta */	
}

void cria_matriz_solucao(int altura, int largura, int matriz[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que altera a matriz de solucao a partir da leitura da matriz */	
	int i = 0;
	int j;
		
	for(i=0; i < altura; i++){
		
		matriz_solucao[i][0] = -1;
		
		for(j=1; j < largura; j++){
			if(matriz[i][j] == -1){
				matriz_solucao[i][j] = 0; /* celula branca, quando for descoberto o seu valor muda-se o 0 para o valor pretendido */
			}
			else{
				matriz_solucao[i][j] = -1; /* celula preta ou triangular, nao havendo distincao entre as duas */
			}		 	
		}
   }	
	return; /* funcao e void, nao precisa de devolver a matriz ou um ponteiro para esta, pois ela e recebida por referencia como argumento da funcao, e alterada dentro desta */
}

int analisa_matriz_solucao(int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que analisa a matriz de solucao e verifica se o puzzle esta preenchido ou nao, e se estiver verifica se a solucao esta correcta */
	int i,j,k;
	int soma;
	int incompleto = 0;
	
	for(i=0; i<altura; i++){
		for(j=0; j<largura; j++){
			if(matriz_solucao[i][j] == 0){ /* ainda existem brancas com o seu valor por descobrir */				
				incompleto = 1;
			}
		}
	}
	
	if(incompleto == 0){ /* puzzle esta completo */
		for(i=0; i<altura; i++){
			for(j=0; j<largura; j++){
				soma=0;
				if(matriz_estruturas[i][j].triangulo_superior != NULL){
					/* para uma dada soma na horizontal */
					for(k= j+1; k <= j + matriz_estruturas[i][j].triangulo_superior->num_casas; k++){
						soma += matriz_solucao[i][k]; /* vai somando os valores que estao na matriz de solucao para as brancas */
					}
					if(soma != matriz_estruturas[i][j].triangulo_superior->soma){ /* verifica se as somas correspondem ao valo que esta nas triangulares correspondentes */
						return 1; /* puzzle esta completo mas mal resolvido */
					}
				}
				soma=0;	
				if(matriz_estruturas[i][j].triangulo_inferior != NULL){
					/* para uma dada soma na vertical */
					for(k = i+1; k <= i + matriz_estruturas[i][j].triangulo_inferior->num_casas; k++){
						soma += matriz_solucao[k][j];
					}
					if(soma != matriz_estruturas[i][j].triangulo_inferior->soma){ /* verifica se as somas correspondem ao valo que esta nas triangulares correspondentes */
						return 1; /* puzzle esta completo mas mal resolvido */
					}
					
				}
			}
		}	
	return 0; /* puzzle esta bem resolvido */
	}
	
	return 2; /* puzzle ainda nao esta resolvido */
}

void cria_matrizes_temporarias(int altura, int largura, int matriz[altura][largura], celula_matriz matriz_estruturas[altura][largura], celula_matriz matriz_estruturas_temporaria[altura][largura], int matriz_solucao[altura][largura], int matriz_solucao_temporaria[altura][largura]){
/* funcao que altera a matriz de estruturas temporaria que vai ser usada no metodo de tentativa e erro, em que cada estrutura é um ponteiro para estruturas */
	int i, j;
	
	for(i = 0; i<altura; i++){
		for(j = 0; j<largura; j++){
			matriz_solucao_temporaria[i][j] = matriz_solucao[i][j];
			/* a matriz de solucao temporaria e uma copia da matriz de solucao de como ela esta quando o metodo de tentativa e erro vai ser chamado */
		}
	}		
	
	for(i = 0; i<altura; i++){
		for(j = 0; j<largura; j++){
			/* inicializa ponteiros a NULL */
			matriz_estruturas_temporaria[i][j].casa = NULL;
			matriz_estruturas_temporaria[i][j].triangulo_inferior = NULL;
			matriz_estruturas_temporaria[i][j].triangulo_superior = NULL;			
			
			/* celula branca */
			if(matriz_estruturas[i][j].casa != NULL){
				matriz_estruturas_temporaria[i][j].casa = cria_branca(i, j, altura, largura, matriz_estruturas_temporaria);				
			}
			/* triangular superior */
			if(matriz_estruturas[i][j].triangulo_superior != NULL){
				matriz_estruturas_temporaria[i][j].triangulo_superior = cria_triangulo(matriz_estruturas[i][j].triangulo_superior->soma, 'h', i, j, altura, largura, matriz);
			}
			/* triangular inferior */
			if(matriz_estruturas[i][j].triangulo_inferior != NULL){
				matriz_estruturas_temporaria[i][j].triangulo_inferior = cria_triangulo(matriz_estruturas[i][j].triangulo_inferior->soma, 'v', i, j, altura, largura, matriz);
			}			
		}
	}
	
	return; /* funcao e void, nao precisa de devolver a matriz ou um ponteiro para esta, pois ela e recebida por referencia como argumento da funcao, e alterada dentro desta */	
}

fila_casas * push_coordenadas(int i, int j, fila_casas * ultima){
	fila_casas * novo;
	
	/* aloca espaco necessario para uma estrutura deste tipo */
	novo = (fila_casas *) malloc(sizeof(fila_casas));
	if (novo == NULL){
		printf("erro: memoria cheia\n");
		exit(-1);
	}
	
	novo->anterior = ultima;
	novo->i = i;
	novo->j = j;
	
	ultima = novo;	
	
	return ultima;
}

fila_casas * pop_coordenadas(int * i, int *j, fila_casas * ultima){
	fila_casas * aux;
	
	if(ultima != NULL){
		aux = ultima;
		ultima = ultima->anterior;
		
		(*i) = aux->i;
		(*j) = aux->j;
			
		free(aux);
	}
	
	else{
		(*i) = -2;
		(*j) = -2;
	}
			
	return ultima;
}

void limpa_memoria(int altura, int largura, celula_matriz matriz_estruturas[altura][largura]){
/* funcao para libertar a memoria alocada para a matriz de estruturas */
	int i, j;
	
	/* vai percorrendo a matriz de estruturas para libertar entrada a entrada */
	for(i=0; i < altura; i++){
		for(j=0; j < largura; j++){
			if(matriz_estruturas[i][j].casa != NULL){
				free(matriz_estruturas[i][j].casa); 
				/* libertar memoria alocada referente a estrutura de uma celula do tipo branca */
			}
			
			if(matriz_estruturas[i][j].triangulo_superior != NULL){
				free(matriz_estruturas[i][j].triangulo_superior); 
				/* libertar memoria alocada referente a estrutura de uma celula do tipo triangular */
			}
			
			if(matriz_estruturas[i][j].triangulo_inferior != NULL){
				free(matriz_estruturas[i][j].triangulo_inferior);
				/* libertar memoria alocada referente a estrutura de uma celula do tipo triangular */
			}
		}
	}
	
	return;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
