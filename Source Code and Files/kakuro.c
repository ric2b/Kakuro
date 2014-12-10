/* projecto de aed - cross sums */

/* Maria Margarida Dias dos Reis, 73099 */
/* Ricardo Filipe Amendoeira, 73373 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kakuro.h"
#include "combinacoes.h"
#include "resolve.h"
#include "matrizes.h"

/* ficheiro que contem as funcoes de escrita e leitura de ficheiros mais a funcao main */
 
void ler_ficheiro(FILE * fpin, int altura, int largura, int matriz[altura][largura]){
/* funcao que vai ler o ficheiro de entrada linha a linha e a partir deste preencher a matriz */	
	char linha [1024];
	int temp;
	int i,j,k;
	int linhas_em_branco = 0;	
			
	fgets(linha, 1024, fpin);
	
	i = 0;
	while(fgets(linha, 1024, fpin) != NULL && linhas_em_branco == 0){ 
	/* enquanto o ficheiro não tiver acabado */
		j = 0;
		
		if(sscanf(linha, "%d", &temp) == 1){	
		/* verifica se cada linha começa com um numero, como e suposto */
		
			matriz[i][j] = temp; /* guarda na matriz o primeiro valor que ler de cada linha */
			j++;
		
			k=0;		
			while(j < largura){
				k += strlen(&linha[k]) - strlen(strchr(&linha[k], ':')+1);
				/* vai avancando na linha, procurando a cada repetição dois pontos e vendo o que está para a frente */
			
				if(sscanf(&linha[k], "%d", &temp) == 1){
					matriz[i][j] = temp; /* le o que esta a frente e se for o guarda-o como esta na matriz */ 				
				}
				else{ 
					matriz[i][j] = -1; /* se for uma célula branca guarda como sendo um -1 */
				}
				j++;
			}		
			i++;
		}
		else{
			linhas_em_branco++;
		}
	}
		
	return;
}

void escreve_solucao(FILE * fpin, char * argv[], int solucao, int altura, int largura, int matriz[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que escreve no ficheiro de saida uma copia do ficheiro de entrada mais a solucao do puzzle, se esta existir */
	FILE * fpout;
	char nome[50];
	char * temp;
	char linha[1024];
	
	strcpy(nome, argv[1]);
	temp = strchr(nome, '.');
	*temp = '\0';
	strcat(nome, ".sol");
	/* procura o nome do ficheiro de entrada e muda o que esta do ponto para a frente e passa de ".cfg" para ".sol" */
	
	fpout = fopen(nome, "w");
	if(fpout == NULL){
		printf("error: output file could not be created.\n");
		exit(-1);
	}	
	
	rewind(fpin); /* coloca o ponteiro para a leitura do ficheiro no inicio deste para se poder ler o ficheiro de entrada do inicio de novo*/
		
	while(fgets(linha, 1024, fpin) != NULL){
		fprintf(fpout, "%s", linha);
	} /* copia o ficheiro de entrada para o de saida */
		
	int i = 0;
	int j;
	
	if(solucao == 0){
		for(i=0; i<altura; i++){
			
			fprintf(fpout, "-");
					
			for(j=1; j<largura; j++){
				fprintf(fpout, ":");
				if(matriz[i][j] == -1){		
					fprintf(fpout, "%c", matriz_solucao[i][j]+48); 
					/* percorre a matriz de solucao a procura da solucao para as brancas e escreve-os no ficheiro de entrada */
				}
				else{
					fprintf(fpout, "-"); /* pretas ou triangulares aparecem no ficheiro de saida com um hifen */
				}			
			}
			fprintf(fpout, "\n");	 
		}
	}
	fprintf(fpout, "\n");	
	
	fclose(fpout);
	return;
}

int main (int argc, char * argv[]){
	FILE * fpin;		
	
	/* verifica numero de argumentos */
	if(argc != 2){
		printf("usage: \"%s name.cfg\"\n", argv[0]);  
		exit(-1);
	}
	
	/* verifica se o ficheiro tem extensao */
	if(strrchr(argv[1], '.') == NULL){
		printf("error: file extension not supported. only takes \".cfg\"\n");  
		exit(-1);
	}
		
	/* verifica extensao do ficheiro com a configuracao do puzzle */
	if(strcmp(strrchr(argv[1], '.'), ".cfg") != 0){
		printf("error: file extension not supported. only takes \".cfg\"\n");  
		exit(-1);		
	}
	
	/* abre ficheiro com a configuracao do puzzle */
	fpin = fopen(argv[1], "r");
	if(fpin == NULL){
		printf("error: file could not be opened.\n");
		exit(-1);
	}
	
	char linha [100];	
	int largura, altura;	

	altura  = atoi(fgets(linha, 100, fpin)); /* le 1a linha do ficheiro e guarda a altura do puzzle */
	largura = atoi(fgets(linha, 100, fpin)); /* le 2a linha do ficheiro e guarda a largura do puzzle */
	
	/* as matrizes sao declaradas no main e passadas por referencia como argumentos das varias funcoes, sendo que estas funcoes nao as criam mas alteram os seus dados */
	int matriz[altura][largura]; /* matriz que contem uma copia do tabuleiro de jogo */
	int matriz_solucao[altura][largura]; /* matriz que vai guardando a solucao do jogo */
	int matriz_solucao_temporaria[altura][largura]; /* matriz de solucao temporaria para o caso de ser necessário aplicar a tentativa e erro */
	celula_matriz matriz_estruturas[altura][largura]; /* matriz de estruturas para armazenar informacao para cada tipo de celula de jogo */
	celula_matriz matriz_estruturas_temporaria[altura][largura]; /* matriz de estruturas temporária para o caso de ser necessário aplicar tentativa e erro */
	
	ler_ficheiro(fpin, altura, largura, matriz); /* ler ficheiro com a configuraçao do puzzle */
	cria_matriz_solucao(altura, largura, matriz, matriz_solucao); /* preencher a matriz de solucao */ 
	cria_matriz_estruturas(altura, largura, matriz, matriz_estruturas, matriz_solucao); /* preencher a matriz de estruturas */
	procura_soma_triangulares(altura, largura, matriz_estruturas, matriz_solucao); /* funcao que vai iniciar a resoluçao do puzzle, com a aplicacao de varias restriçoes */
	procura_soma_triangulares(altura, largura, matriz_estruturas, matriz_solucao); /* chama-se outra vez esta funcao pois pode ter aplicado mais restricoes mas nao ter chegado a descobrir nenhum valor e ao chamar-se de novo pode aplicar tudo uma ultima vez e ver se resolve o puzzle */
	
		
	
	if(analisa_matriz_solucao(altura, largura, matriz_estruturas, matriz_solucao) == 2){
	/* se entrar dentro deste if e porque o puzzle ainda nao esta resolvido e já e necessario recorrer a metodo de tentativa e erro */
		cria_matrizes_temporarias(altura, largura, matriz, matriz_estruturas, matriz_estruturas_temporaria, matriz_solucao, matriz_solucao_temporaria);	
		/* cria a matriz temporaria que e uma copia da matriz original de como ela esta no momento da chamada desta funcao */
		
		int pos_y, pos_x;
		int lixo;
		fila_casas * ultima = NULL;
		
		do{			
			while(ultima != NULL){
				ultima = pop_coordenadas(&lixo, &lixo, ultima);
			}			
			
			procura_soma_triangulares(altura, largura, matriz_estruturas, matriz_solucao);
			encontra_casa_teste(&pos_y, &pos_x, altura, largura, matriz_estruturas, matriz_solucao);						
		
		}	while(testa_hipoteses(pos_y, pos_x, &ultima, altura, largura, matriz, matriz_estruturas, matriz_solucao, matriz_estruturas_temporaria, matriz_solucao_temporaria) == 1);
		
		
		while(ultima != NULL){
			ultima = pop_coordenadas(&lixo, &lixo, ultima);
		}
		
		limpa_memoria(altura, largura, matriz_estruturas_temporaria);
		/* liberta a memoria alocada na matriz de estruturas temporaria */
	}
	
	int solucao;
	
	solucao = analisa_matriz_solucao(altura, largura, matriz_estruturas, matriz_solucao);
	/* verifica qual o valor que esta funcao devolve par passar como argumento para a funcao que escreve a solucao no ficheiro de saida para que esta saiba se o puzzle tem solucao ou nao */ 
	escreve_solucao(fpin, argv, solucao, altura, largura, matriz, matriz_solucao); /* escrever no ficheiro de saida */
		
	limpa_memoria(altura, largura, matriz_estruturas); /* funcao que liberta a memoria alocada na matriz de estruturas */	
	
	fclose(fpin);
	exit(0);
}
