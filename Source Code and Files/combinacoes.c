#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ficheiro que contem duas funcoes associadas as hipoteses que as brancas podem tomar */
 
void combinacoes(int soma, int casas, int vector[9]){
/* calcula quais as combinacoes possiveis para uma dada branca em funcao de uma soma e do numero de casas que a contitui */

	/* soma a 2 casas, de 3 a 17 */     
	int duas_casas[15] = {120000000,103000000,123400000,120450000,123456000,123056700,123456780,123406789,23456789,3450789,456789,56089,6789,709,89};
	  
	/* soma a 3 casas, de 6 a 24 */     
	int tres_casas[19] = {123000000,120400000,123450000,123456000,123456700,123456780,123456789,123456789,123456789,123456789,123456789,123456789,
						  123456789,23456789,3456789,456789,56789,6089,789};
									
	/* soma a 4 casas, de 10 a 30 */    
	int quatro_casas[21] = {123400000,123050000,123456000,123456700,123456780,123456789,123456789,123456789,123456789,123456789,123456789,123456789,
							123456789,123456789,123456789,123456789,23456789,3456789,456789,50789,6789};
										
	/* soma a 5 casas, de 15 a 35 */    
	int cinco_casas[21] = {123450000,123406000,123456700,123456780,123456789,123456789,123456789,123456789,123456789,123456789,123456789,123456789,
						   123456789,123456789,123456789,123456789,123456789,23456789,3456789,406789,56789};
										
	/* soma a 6 casas, de 21 a 39 */    
	int seis_casas[19] = {123456000,123450700,123456789,123456789,123456789,123456789,123456789,123456789,123456789,123456789,123456789,123456789,
						  123456789,123456789,123456789,123456789,23456789,3056789,456789};
										
	/* soma a 7 casas, de 28 a 42 */    
	int sete_casas[15] = {123456700,123456080,123456789,123456789,123456789,123456789,123456789,123456789,123456789,123456789,123456789,123456789,
						  123456789,20456789,3456789};
										
	/* soma a 8 casas, de 36 a 44*/    
	int oito_casas[9] = {123456780,123456709,123456089,123450789,123406789,123056789,120456789,103456789,23456789};
		 
	/* soma a 9 casas, unico valor possivel e o 45*/          
	int nove_casas[1] = {123456789};

	char string[9] = "000000000";

	switch(casas){ /* dependendo do numero de casas */
		case 2:
			if(soma >= 3 && soma <= 17){
				sprintf(string, "%d", duas_casas[soma-3]); /* escreve na string os numeros que le do vector para a soma indicada */
			}
			else{
				printf("erro: soma não possivel com %d casas(combinacoes)\n", casas);
				exit(-1);
			}					
			break;
		case 3:
			if(soma >= 6 && soma <= 24){
				sprintf(string, "%d", tres_casas[soma-6]);
			}
			else{
				printf("erro: soma não possivel com %d casas(combinacoes)\n", casas);
				exit(-1);
			}	
			break;
		case 4:
			if(soma >= 10 && soma <= 30){
				sprintf(string, "%d", quatro_casas[soma-10]);
			}
			else{
				printf("erro: soma não possivel com %d casas(combinacoes)\n", casas);
				exit(-1);
			}	
			break;
		case 5:
			if(soma >= 15 && soma <= 35){
				sprintf(string, "%d", cinco_casas[soma-15]);
			}
			else{
				printf("erro: soma não possivel com %d casas(combinacoes)\n", casas);
				exit(-1);
			}	
			break;
		case 6:
			if(soma >= 21 && soma <= 39){
				sprintf(string, "%d", seis_casas[soma-21]);
			}
			else{
				printf("erro: soma não possivel com %d casas(combinacoes)\n", casas);
				exit(-1);
			}	
			break;
		case 7:
			if(soma >= 28 && soma <= 42){
				sprintf(string, "%d", sete_casas[soma-28]);
			}
			else{
				printf("erro: soma não possivel com %d casas(combinacoes)\n", casas);
				exit(-1);
			}	
			break;
		case 8:
			if(soma >= 36 && soma <= 44){
				sprintf(string, "%d", oito_casas[soma-36]);
			}
			else{
				printf("erro: soma não possivel com %d casas(combinacoes)\n", casas);
				exit(-1);
			}	
			break;
		case 9:
			if(soma == 45){
				sprintf(string, "%d", nove_casas[0]);
			}
			else{
				printf("erro: soma não possivel com %d casas(combinacoes)\n", casas);
				exit(-1);
			}	
			break;
		default: printf("erro: numero de casas inválido (s:%d c:%d) (combinacoes)\n", soma, casas);
			exit(-1);
	}
	
	int i;
	for(i=0; i<9; i++){
		if((string[i]-48) != 0){ /* cada elemento do vector e um numero que foi escrito numa string, mas quer-se alterar um vector que e uma mascara, com 1s onde o numero e possivel e 0s onde e impossivel */ 
			vector[string[i]-49] = 1; /* se na string o numero e diferente de 0 entao e possivel e coloca-se no indice respectivo do vector um 1 */ 
			/* altera o vector de hipoteses de cada branca, que recebe como argumento da funcao, colocando 1s na mascara onde ve que um numero e possivel */
		}
	}
	
	return; /* funcao e void, o vector e alterado dentro desta e nao e preciso devolver nada */		
}

int combinacoes_tentativa_erro(int soma, int num_casas){
/* devolve 1 ou 0 consoante uma dada ser possivel ou nao, respectivamente, com um determinado numero de casas, sendo isto usado quando se recorre a tentativa e erro */	
	switch(num_casas){ /* dependendo do numero de casas */
		case 2:
			if(soma >= 3 && soma <= 17){
				return 1; /* soma encontra-se dentro do range de somas possiveis para esse numero de casas */
			}
			else{
				return 0; /* soma impossivel com esse numero de casas */
			}					
			break;
		case 3:
			if(soma >= 6 && soma <= 24){
				return 1;
			}
			else{
				return 0;
			}	
			break;
		case 4:
			if(soma >= 10 && soma <= 30){
				return 1;
			}
			else{
				return 0;
			}	
			break;
		case 5:
			if(soma >= 15 && soma <= 35){
				return 1;
			}
			else{
				return 0;
			}	
			break;
		case 6:
			if(soma >= 21 && soma <= 39){
				return 1;
			}
			else{
				return 0;
			}	
			break;
		case 7:
			if(soma >= 28 && soma <= 42){
				return 1;
			}
			else{
				return 0;
			}	
			break;
		case 8:
			if(soma >= 36 && soma <= 44){
				return 1;
			}
			else{
				return 0;
			}	
			break;
		case 9:
			if(soma == 45){
				return 1;
			}
			else{
				return 0;
			}	
			break;
		default: 
			return 0;
	}
}
		
