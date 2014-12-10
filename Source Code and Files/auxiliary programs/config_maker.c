#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void criar_ficheiro(char nome_entrada[20]){
	
	FILE * ficheiro_entrada;
	FILE * ficheiro_saida;
	
	char linha [1024];
	char nome_saida[20];
	char * temp;
	int linhas_em_branco = 0;	
	
	strcpy(nome_saida, nome_entrada);
	temp = strrchr(nome_saida, '.');
	strcpy(temp, ".cfg");
	
	ficheiro_entrada = fopen(nome_entrada, "r");
	if(ficheiro_entrada == NULL){
		printf("error: input file could not be opened.\n");
		exit(-1);
	}
	
	ficheiro_saida = fopen(nome_saida, "w");
	if(ficheiro_saida == NULL){
		printf("error: output file could not be created.\n");
		exit(-1);
	}	

	while(fgets(linha, 1024, ficheiro_entrada) != NULL && linhas_em_branco < 2){
		fprintf(ficheiro_saida, "%s", linha);
		if(linha[0] == '\n'){
			linhas_em_branco++;
		}
	}
	
	fclose(ficheiro_entrada);
	fclose(ficheiro_saida);
		
	return;
}

int main(){
	
	int i;
	char nome[20];
	
	
	for(i=1; i<= 254; i++){
		if(i < 10){
			sprintf(nome, "Kakuro_00%d.sol", i); 
		}
		else{
			if(i < 100){
				sprintf(nome, "Kakuro_0%d.sol", i); 
			}
			else{
				sprintf(nome, "Kakuro_%d.sol", i); 
			}
		}
		
		criar_ficheiro(nome);		
	}	
	
	printf("done :)\n");
	
	exit(0);
}
