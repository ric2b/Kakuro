#include "kakuro.h"
#include "resolve.h"
#include "combinacoes.h"
#include "matrizes.h"

/* ficheiro que contem as varias funcoes de restricoes a aplicar no puzzle mais a funcao do metodo de tentativa e erro */

int resolve_casas_adjacentes(int y, int x, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que quando descobre o valor de uma casa, retira esse valor as hipoteses das casas adjacentes pelo facto de nao poder haver numeros repetidos numa soma */	
	triangular * superior = matriz_estruturas[y][x].casa->superior;
	triangular * inferior = matriz_estruturas[y][x].casa->inferior;
	
	int i, j;
	int n = -1;
	int contador;
	
	for(i=0; i<9; i++){
		if(matriz_estruturas[y][x].casa->valores[i] == 1){
			n = i; /* verifica qual o valor resolvido */
			break;
		} 
	}
	
	if(n != -1){
		if(superior != NULL){ /* para a soma na horizontal */
			for(j=(superior->pos_x)+1; j <= (superior->pos_x) + (superior->num_casas); j++){
			/* percorrer as brancas dessa soma e colocar um 0 nessa hipotese no vector que serve como mascara, indicando assim que o numero e impossivel */ 
				matriz_estruturas[superior->pos_y][j].casa->valores[n] = 0;		
			}
		}
		
		if(inferior != NULL){ /* para a soma na vertical */
			for(i=(inferior->pos_y)+1; i <= (inferior->pos_y) + (inferior->num_casas); i++){
			/* percorrer as brancas dessa soma e colocar um 0 nessa hipotese no vector que serve como mascara, indicando assim que o numero e impossivel */ 
				matriz_estruturas[i][inferior->pos_x].casa->valores[n] = 0;
			}
		}
		
		matriz_estruturas[y][x].casa->valores[n] = 1; /* voltar a colocar um 1 no indice correcto no vector de hipoteses da casa resolvida, pois anteriormente colocou-se a 0s para todas as casas */
		
		if(superior != NULL){ /* para a soma na horizontal */
			for(j=(superior->pos_x)+1; j <= (superior->pos_x) + (superior->num_casas); j++){
				contador = 0;
				for(i=0; i<9; i++){
					if(matriz_estruturas[superior->pos_y][j].casa->valores[i] == 1){
						contador++; /* verificar se alguma casa podera ter ficado resolvida */
					} 
				}
				if(contador == 1 && (matriz_solucao[superior->pos_y][j] == 0)){ /* casa ficou resolvida e ja nao o estava anteriormente */
					for(i=0; i<9; i++){
						if(matriz_estruturas[superior->pos_y][j].casa->valores[i] == 1){
							matriz_solucao[superior->pos_y][j] = i+1; /* coloca a solucao na matriz de solucao nos indices respectivos dessa casa */
						}
					}
					resolve_casas_adjacentes(superior->pos_y, j, altura, largura, matriz_estruturas, matriz_solucao);
					/* chama recursivamente a funcao para esta nova casa resolvida */
				}
				if(contador == 0){ 
				/* casa ficou sem hipoteses, o numero nao esta correcto, sendo esta uma verificacao pertinente para quando se aplicar tentativa e erro */
					return 0; /* devolve 0 para se saber que houve erro */
				} 	
			}
		}	
		
		if(inferior != NULL){ /* para a soma na vertical */
			for(i=(inferior->pos_y)+1; i < (inferior->pos_y) + (inferior->num_casas) + 1; i++){
				contador = 0;
				for(j=0; j<9; j++){
					if(matriz_estruturas[i][inferior->pos_x].casa->valores[j] == 1){
						contador++; /* verificar se alguma casa podera ter ficado resolvida */
					} 
				}
				if(contador == 1 && (matriz_solucao[i][inferior->pos_x] == 0)){ /* casa ficou resolvida e ja nao o estava anteriormente */
					for(j=0; j<9; j++){
						if(matriz_estruturas[i][inferior->pos_x].casa->valores[j] == 1){
							matriz_solucao[i][inferior->pos_x] = j+1; /* coloca a solucao na matriz de solucao nos indices respectivos dessa casa */
						}
					}
					resolve_casas_adjacentes(i, inferior->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
					/* chama recursivamente a funcao para esta nova casa resolvida */
				}
				if(contador == 0){
				/* casa ficou sem hipoteses, o numero nao esta correcto, sendo esta uma verificacao pertinente para quando se aplicar tentativa e erro */
					return 0; /* devolve 0 para se saber que houve erro */
				} 	
			}
		}
		return 1; /* nao deu erros e devolve 1 */
	}	
		
	return 0; /* valor da casa invalido, tem-se erro e devolve 0 */
}

int cruza_somas(int y, int x, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que cruza para uma branca as hipoteses provenientes da pista vertical e as da pista horizontal */
	int vector_superior[9] = {1,1,1,1,1,1,1,1,1};
	int vector_inferior[9] = {1,1,1,1,1,1,1,1,1};
	int contador = 0;
	int i;
	
	if(matriz_estruturas[y][x].casa == NULL){
		printf("erro: a celula nao existe\n");
		exit(-1);
	}
	
	if(matriz_estruturas[y][x].casa->superior != NULL){ /* soma na horizontal */
		for(i=0; i<9; i++){
			vector_superior[i] = 0; /* inicializa vector que sera mandado como argumento para a funcao das combinacoes a 0s */
		}
		combinacoes(matriz_estruturas[y][x].casa->superior->soma, matriz_estruturas[y][x].casa->superior->num_casas, vector_superior);
		/* chama a funca que devolvera as combinacaoes para as brancas desta soma */
	}
	if(matriz_estruturas[y][x].casa->inferior != NULL){ /* soma na vertical */
		for(i=0; i<9; i++){
			vector_inferior[i] = 0; /* inicializa vector que sera mandado como argumento para a funcao das combinacoes a 0s */
		}		
		combinacoes(matriz_estruturas[y][x].casa->inferior->soma, matriz_estruturas[y][x].casa->inferior->num_casas, vector_inferior);
		/* chama a funca que devolvera as combinacaoes para as brancas desta soma */
	}
	
	for(i=0; i<9; i++){
		matriz_estruturas[y][x].casa->valores[i] = vector_superior[i] * vector_inferior[i]; 
		/* cruza os dois vectores, da pista vertical e da horizontal, atraves de uma multiplicao, pois os vectores tem 0s e 1s */
		if(matriz_estruturas[y][x].casa->valores[i] == 1){
			contador++; /* verifica se alguma casa ficou resolvida */
		} 
	}	
	if(contador == 1){ /* resolveu-se uma casa e como tal chama-se a resolve_casas_adjacentes */		
		resolve_casas_adjacentes(y, x, altura, largura, matriz_estruturas, matriz_solucao);
	}
	
	return contador; /* devolve o numero de hipoteses que a branca tem */
}
	
void completar_soma(triangular * celula, int valores_descobertos[9], int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que se numa soma so faltar preencher uma casa descobre qual o valor ai correcto */ 
	int valor_em_falta;
	int i, j;
	
	valor_em_falta = celula->soma; 
	for(i=0; i<9; i++){
		valor_em_falta -= (valores_descobertos[i]*(i+1));
	} /* calcula qual o valor em falta nessa soma para que ela esteja correcta */
	
	/* soma na horizontal */
	if(celula->orientacao == 'h'){	
		for(j=(celula->pos_x)+1; j <= (celula->pos_x) + celula->num_casas; j++){
			if(matriz_solucao[celula->pos_y][j] == 0){ /* procura a branca nao resolvida */				
				
				matriz_solucao[celula->pos_y][j] = valor_em_falta; /* coloca na matriz de soluca nos indices respectivos dessa branca por resolver o valor em falta calculado anteriormente */
				
				for(i=0; i<9; i++){
					matriz_estruturas[celula->pos_y][j].casa->valores[i] = 0;
				}
				
				if(valor_em_falta > 0 && valor_em_falta <= 9){
					matriz_estruturas[celula->pos_y][j].casa->valores[valor_em_falta-1] = 1; /* altera-se tambem no vector de hipoteses dessa casa que passa a ter um so 1 */
				}
					
				resolve_casas_adjacentes(matriz_estruturas[celula->pos_y][j].casa->pos_y, matriz_estruturas[celula->pos_y][j].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
				/* resolveu-se uma casa, tem de se chamar a resolve_casas_adjacentes */
				
				return;
			}					
		}
	}
	
	/*soma na vertical */
	if(celula->orientacao == 'v'){	
		for(i=(celula->pos_y)+1; i <= (celula->pos_y) + celula->num_casas; i++){
			if(matriz_solucao[i][celula->pos_x] == 0){ /* procua a branca nao resolvida */		
				
				matriz_solucao[i][celula->pos_x] = valor_em_falta; /* coloca na matriz de soluca nos indices respectivos dessa branca por resolver o valor em falta calculado anteriormente */
				
				for(j=0; j<9; j++){
					matriz_estruturas[i][celula->pos_x].casa->valores[j] = 0;
				}
				matriz_estruturas[i][celula->pos_x].casa->valores[valor_em_falta-1] = 1; /* altera-se tambem no vector de hipoteses dessa casa que passa a ter um so 1 */
					
				resolve_casas_adjacentes(matriz_estruturas[i][celula->pos_x].casa->pos_y, matriz_estruturas[i][celula->pos_x].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
				/* resolveu-se uma casa, tem de se chamar a resolve_casas_adjacentes */
				
				return;
			}					
		}
	}
	
	return;	
}

int combinacoes_pares_reais(triangular * celula, int valores_descobertos[9], int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que verifica para uma soma a duas casas, se os valores que estao numa casa encontram na outra o par correspondente para essa combinacao */ 
	int i, j, k;
	int nova_soma;
	int contagem = 0;
		
	nova_soma = celula->soma;
	for(i=0; i<9; i++){
		nova_soma -= ((valores_descobertos[i])*(i+1));
	} /* calcula a nova soma a duas casas */
	
	/* soma na horizontal */
	if(celula->orientacao == 'h'){
		for(j=(celula->pos_x)+1; j <= (celula->pos_x) + (celula->num_casas); j++){
			if(matriz_solucao[celula->pos_y][j] == 0){ /* percorre as brancas da soma ate encontrar a primeira nao resolvida */
				for(k = j+1; k <= (celula->pos_x) + (celula->num_casas); k++){
					if(matriz_solucao[celula->pos_y][k] == 0){ /* percorre dessa branca ja descoberta para a frente à procura da segunda branca nao resolvida */
						
						for(i=0; i<9; i++){
							if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){ /* testa uma hipotese possivel na primeira branca */
								if((nova_soma-i-2 >= 0) && (nova_soma-i-2 < 9)){
									if(matriz_estruturas[celula->pos_y][k].casa->valores[nova_soma-i-2] == 0){ /* verifica que nao encontra correspodente para essa combinacao na segunda branca */
										matriz_estruturas[celula->pos_y][j].casa->valores[i] = 0; /* hipotese testada impossivel na primeira branca */									
									}
								}
								else{
									matriz_estruturas[celula->pos_y][j].casa->valores[i] = 0; 
									/* valor da nova soma impossivel, hipotese está errada, sendo esta uma verificacao pertinente para quando se aplicar tentativa e erro */
								}																																	
							}
						}														
						
						for(i=0; i<9; i++){
							matriz_estruturas[celula->pos_y][k].casa->valores[i] = 0; 
						}
						
						for(i=0; i<9; i++){
							if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){								
								matriz_estruturas[celula->pos_y][k].casa->valores[nova_soma-i-2] = 1; /* na segunda branca so interessam ja as hipoteses que ainda fazem combinacao possivel com o que esta na primeira branca */
								contagem++; /* vai verificando se as brancas ficaram resolvidas */
							}
						}
						
						if(contagem == 1){ /* brancas ficaram resolvidas */
							for(i=0; i<9; i++){
								if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){
									matriz_solucao[celula->pos_y][j] = i + 1; /* coloca valor descoberto na matriz de solucao para a primeira branca */																				
								}
								if(matriz_estruturas[celula->pos_y][k].casa->valores[i] == 1){
									matriz_solucao[celula->pos_y][k] = i + 1; /* coloca o valor descoberto na matriz de solucao para a segunda branca */				
								}
							}
							resolve_casas_adjacentes(matriz_estruturas[celula->pos_y][j].casa->pos_y, matriz_estruturas[celula->pos_y][j].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
							/* chama resolve_casas_adjacentes para a primeira branca */
							resolve_casas_adjacentes(matriz_estruturas[celula->pos_y][k].casa->pos_y, matriz_estruturas[celula->pos_y][k].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
							/* chama resolve_casas_adjacentes para a segunda branca */
							return 1;
							/* devolve 1 para indicar que foi feita uma alteracao */
						}
						return 0; /* devolve 0 se nao descobriu nada */
					}	
				}
				break;		
			}					
		}	
	}
	
	
	/* soma na vertical */
	if(celula->orientacao == 'v'){
		for(i=(celula->pos_y)+1; i <= (celula->pos_y) + (celula->num_casas); i++){ 
			if(matriz_solucao[i][celula->pos_x] == 0){ /* percorre as brancas da soma ate encontrar a primeira nao resolvida */
				for(k = i+1; k <= (celula->pos_y) + (celula->num_casas); k++){
					if(matriz_solucao[k][celula->pos_x] == 0){ /* percorre dessa branca ja descoberta para a frente à procura da segunda branca nao resolvida */
						
						for(j=0; j<9; j++){
							if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){ /* testa uma hipotese possivel na primeira branca */
								if((nova_soma-j-2 >= 0) && (nova_soma-j-2 < 9) ){
									if(matriz_estruturas[k][celula->pos_x].casa->valores[nova_soma-j-2] == 0){ /* verifica que nao encontra correspodente para essa combinacao na segunda branca */
										matriz_estruturas[i][celula->pos_x].casa->valores[j] = 0; /* hipotese testada impossivel na primeira branca */
									}
								}
								else{
									matriz_estruturas[i][celula->pos_x].casa->valores[j] = 0;
									/* valor da nova soma impossivel, hipotese está errada, sendo esta uma verificacao pertinente para quando se aplicar tentativa e erro */
								}																									
							}
						}
																				
						for(j=0; j<9; j++){
							matriz_estruturas[k][celula->pos_x].casa->valores[j] = 0;
						}										
						
						for(j=0; j<9; j++){
							if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
								matriz_estruturas[k][celula->pos_x].casa->valores[nova_soma-j-2] = 1; /* na segunda branca so interessam ja as hipoteses que ainda fazem combinacao possivel com o que esta na primeira branca */
								contagem++; /* vai verificando se as brancas ficaram resolvidas */
							}
						}
						
						if(contagem == 1){
							for(j=0; j<9; j++){
								if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
									matriz_solucao[i][celula->pos_x] = j + 1;																				
								}										
								if(matriz_estruturas[k][celula->pos_x].casa->valores[j] == 1){
									matriz_solucao[k][celula->pos_x] = j + 1;
								}
							}
							resolve_casas_adjacentes(matriz_estruturas[i][celula->pos_x].casa->pos_y, matriz_estruturas[i][celula->pos_x].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
							/* chama resolve_casas_adjacentes para a primeira branca */
							resolve_casas_adjacentes(matriz_estruturas[k][celula->pos_x].casa->pos_y, matriz_estruturas[k][celula->pos_x].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
							/* chama resolve_casas_adjacentes para a segunda branca */
							return 1;
							/* devolve 1 para indicar que foi feita uma alteracao */
						}
						return 0; /* devolve 0 se nao descobriu nada */
					}	
				}
				break;		
			}					
		}	
	}
	
	return 0; /* devolve 0 se nao descobriu nada */
}

int eliminar_maximos_minimos(triangular * celula, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que testa o maximo de umaca som os minimos das outras a ver se esse valor e possivel e tambem o faz para testar o minimo dessa com os maximos das outras */
	int i,j,k,l,m;

	int nova_soma = celula->soma;
	int soma_de_teste;
	int contagem;
	
	/* soma na horizontal */
	if(celula->orientacao == 'h'){
		for(j=(celula->pos_x)+1; j <= (celula->pos_x) + (celula->num_casas); j++){
			if(matriz_solucao[celula->pos_y][j] != 0){
				nova_soma -= matriz_solucao[celula->pos_y][j];
			}
		} /* calcula a nova soma */
		
		
		for(j=(celula->pos_x)+1; j <= (celula->pos_x) + (celula->num_casas); j++){ /* para cada casa */
			if(matriz_solucao[celula->pos_y][j] == 0){ /* se a casa não estiver preenchida */
				
				soma_de_teste = nova_soma;
				for(k=8; k >=0 ; k--){ /* testa o maximo dessa casa */
					if(matriz_estruturas[celula->pos_y][j].casa->valores[k] == 1){
						soma_de_teste -= (k+1);
					
						for(l=(celula->pos_x)+1; l <= (celula->pos_x) + (celula->num_casas); l++){ /* com o minimo das restantes casas dessa soma */
							if(matriz_solucao[celula->pos_y][l] == 0){
								if(l != j){		
									for(m=0; m<9; m++){
										if(matriz_estruturas[celula->pos_y][l].casa->valores[m] == 1){
											soma_de_teste -= (m+1);							
											break;
										}
									}
								}
							}
						}
						
						if(soma_de_teste < 0){ /* se ao retirar o valor maximo dessa casa e os minimos das restantes o resultado é menor que 0 sabemos que esse valor maximo e impossivel para essa casa */
							matriz_estruturas[celula->pos_y][j].casa->valores[k] = 0; /* retira-se essa hipoteses à branca com que se testou */
							
							contagem = 0;
							for(m=0; m<9; m++){
								if(matriz_estruturas[celula->pos_y][j].casa->valores[m] == 1){
									contagem++; /* verificar se a branca podera ter ficado resolvida */
								}
							}
							
							if(contagem == 1){ /* a casa ficou resolvida */
								
								for(m=0; m<9; m++){
									if(matriz_estruturas[celula->pos_y][j].casa->valores[m] == 1){
										matriz_solucao[celula->pos_y][j] = m+1;
									}
								}								
								
								resolve_casas_adjacentes(celula->pos_y, j, altura, largura, matriz_estruturas, matriz_solucao);
								/* chama resolve_casas_adjacentes para essa branca que ficou resolvida */
								return 1;
								/* devolve 1 para indicar que se fez uma alteracao */
							}
						}
														
						break;
					}
				}
				
				soma_de_teste = nova_soma;
				for(k=0; k<9 ; k++){ /* testar o minimo dessa casa */
					if(matriz_estruturas[celula->pos_y][j].casa->valores[k] == 1){
						soma_de_teste -= (k+1);
					
						for(l=(celula->pos_x)+1; l <= (celula->pos_x) + (celula->num_casas); l++){ /* com o maximo das restantes casas */
							if(matriz_solucao[celula->pos_y][l] == 0){
								if(l != j){		
									for(m=8; m >= 0; m--){
										if(matriz_estruturas[celula->pos_y][l].casa->valores[m] == 1){
											soma_de_teste -= (m+1);							
											break;
										}
									}
								}
							}
						}
						
						if(soma_de_teste > 0){ /* se ao retirar o valor dessa casa e os minimos das restantes o resultado ainda é maior que 0 esse valor minimo é impossivel para essa casa */
							matriz_estruturas[celula->pos_y][j].casa->valores[k] = 0; /* retira-se essa hipoteses à branca com que se testou */
							
							
							contagem = 0;
							for(m=0; m<9; m++){
								if(matriz_estruturas[celula->pos_y][j].casa->valores[m] == 1){
									contagem++; /* verificar se a branca podera ter ficado resolvida */
								}
							}
							
							if(contagem == 1){ /* a casa ficou resolvida */
								
								for(m=0; m<9; m++){
									if(matriz_estruturas[celula->pos_y][j].casa->valores[m] == 1){
										matriz_solucao[celula->pos_y][j] = m+1;
									}
								}								
								
								resolve_casas_adjacentes(celula->pos_y, j, altura, largura, matriz_estruturas, matriz_solucao);
								/* chama resolve_casas_adjacentes para essa branca que ficou resolvida */
								return 1;
								/* devolve 1 para indicar que se fez uma alteracao */
							}	
						}						
						break;
					}
				}
			}
		}
	}

	/* soma na vertical */
	if(celula->orientacao == 'v'){
		for(i=(celula->pos_y)+1; i <= (celula->pos_y) + (celula->num_casas); i++){
			if(matriz_solucao[i][celula->pos_x] != 0){
				nova_soma -= matriz_solucao[i][celula->pos_x];
			}
		} /* calcula a nova soma */
		
		
		for(i=(celula->pos_y)+1; i <= (celula->pos_y) + (celula->num_casas); i++){ /* para cada casa branca */
			if(matriz_solucao[i][celula->pos_x] == 0){ /* se não estiver preenchida */
				
				soma_de_teste = nova_soma;
				for(k=8; k >=0 ; k--){ /* testar o maximo dessa casa */
					if(matriz_estruturas[i][celula->pos_x].casa->valores[k] == 1){
						soma_de_teste -= (k+1);
					
						for(l=(celula->pos_y)+1; l <= (celula->pos_y) + (celula->num_casas); l++){ /* com o minimo das restantes casas */
							if(matriz_solucao[l][celula->pos_x] == 0){
								if(l != i){		
									for(m=0; m<9; m++){
										if(matriz_estruturas[l][celula->pos_x].casa->valores[m] == 1){
											soma_de_teste -= (m+1);							
											break;
										}
									}
								}
							}
						}
						
						if(soma_de_teste < 0){ /* se ao retirar o valor maximo dessa casa e os minimos das restantes o resultado é menor que 0 entao esse valor maximo é impossivel para essa casa */
							matriz_estruturas[i][celula->pos_x].casa->valores[k] = 0; 
							
							contagem = 0;
							for(m=0; m<9; m++){
								if(matriz_estruturas[i][celula->pos_x].casa->valores[m] == 1){
									contagem++; /* verificar se a branca podera ter ficado resolvida */
								}
							}
							
							if(contagem == 1){ /* a casa ficou resolvida */
								
								for(m=0; m<9; m++){
									if(matriz_estruturas[i][celula->pos_x].casa->valores[m] == 1){
										matriz_solucao[i][celula->pos_x] = m+1;
									}
								}
																
								resolve_casas_adjacentes(i, celula->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
								/* chama resolve_casas_adjacentes para essa branca que ficou resolvida */
								return 1;
								/* devolve 1 para indicar que se fez uma alteracao */
							}
						}						
						
						break;
					}
				}
				
				soma_de_teste = nova_soma;
				for(k=0; k<9 ; k++){ /* testar o minimo dessa casa */
					if(matriz_estruturas[i][celula->pos_x].casa->valores[k] == 1){
						soma_de_teste -= (k+1);
					
						for(l=(celula->pos_y)+1; l <= (celula->pos_y) + (celula->num_casas); l++){ /* com o maximo das restantes casas */
							if(matriz_solucao[l][celula->pos_x] == 0){
								if(l != i){		
									for(m=8; m >= 0; m--){
										if(matriz_estruturas[l][celula->pos_x].casa->valores[m] == 1){
											soma_de_teste -= (m+1);							
											break;
										}
									}
								}
							}
						}
						
						if(soma_de_teste > 0){ /* se ao retirar o valor  dessa casa e os minimos das restantes o resultado ainda é maior que 0 entao esse valor minimo é impossivel para essa casa */ 
							matriz_estruturas[i][celula->pos_x].casa->valores[k] = 0; /* retirar essa hipotese a essa casa */
							
							contagem = 0;
							for(m=0; m<9; m++){
								if(matriz_estruturas[i][celula->pos_x].casa->valores[m] == 1){
									contagem++; /* verificar se a branca podera ter ficado resolvida */
								}
							}
							
							if(contagem == 1){ /* a casa ficou resolvida */
								
								for(m=0; m<9; m++){
									if(matriz_estruturas[i][celula->pos_x].casa->valores[m] == 1){
										matriz_solucao[i][celula->pos_x] = m+1;
									}
								}
								
								resolve_casas_adjacentes(i, celula->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
								/* chama resolve_casas_adjacentes para essa branca que ficou resolvida */
								return 1;
								/* devolve 1 para indicar que se fez uma alteracao */
							}							
						}								
						break;
					}
				}
			}
		}
	}	
	
	return 0;
}

int n_possibilidades_n_casas(triangular * celula, int novas_possibilidades[9], int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que verifica se numa soma a "n" existem "n" possiblidades para cada casa e tenta determinar se sabe o arranjo destas hipoteses */
	int i,j, k;
	int posicoes_validas;
	int alteracoes = 0;
	
	/* soma na horizontal */
	if(celula->orientacao == 'h'){	
		for(i=0; i<9; i++){	
			if(novas_possibilidades[i] == 1){ /* para cada uma dessas "n" hipoteses */	
				posicoes_validas = 0;
				for(j=(celula->pos_x)+1; j <= (celula->pos_x) + celula->num_casas; j++){
					if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){
						posicoes_validas++; /* determina em quantas casas brancas uma dessas "n" hipoteses e valida */
					}
				}			
				if(posicoes_validas == 1){ /* se so for valida numa casa entao sabe-se que esse o valor que essa tomará */
					for(j=(celula->pos_x)+1; j <= (celula->pos_x) + celula->num_casas; j++){
						if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){
							matriz_solucao[celula->pos_y][j] = i+1; /* coloca na matriz de solucao o valor que descobriu */
							for(k=0; k<9; k++){
								matriz_estruturas[celula->pos_y][j].casa->valores[k] = 0;
							}
							matriz_estruturas[celula->pos_y][j].casa->valores[i] = 1;
							resolve_casas_adjacentes(matriz_estruturas[celula->pos_y][j].casa->pos_y, matriz_estruturas[celula->pos_y][j].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
							/* casa ficou resolvida e chama-se a resolve_casas_adjacentes */
							alteracoes = 1;					
						}
					}
				}
			}				
		}		
		return alteracoes;	
		/* devolve se fez alteracoes ou nao */
		
	}	
	
	/* soma na vertical */			
	if(celula->orientacao == 'v'){	
		for(j=0; j<9; j++){	
			if(novas_possibilidades[j] == 1){ /* para cada uma dessas "n" hipoteses */	
				posicoes_validas = 0;
				for(i=(celula->pos_y)+1; i <= (celula->pos_y) + celula->num_casas; i++){
					if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
						posicoes_validas++; /* determina em quantas casas brancas uma dessas "n" hipoteses e valida */
					}
				}			
				if(posicoes_validas == 1){ /* se so for valida numa casa entao sabe-se que esse o valor que essa tomará */
					for(i=(celula->pos_y)+1; i <= (celula->pos_y) + celula->num_casas; i++){
						if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
							matriz_solucao[i][celula->pos_x] = j+1; /* coloca na matriz de solucao o valor que descobriu */
							
							for(k=0; k<9; k++){
								matriz_estruturas[i][celula->pos_x].casa->valores[k] = 0;
							}
							matriz_estruturas[i][celula->pos_x].casa->valores[j] = 1;
							resolve_casas_adjacentes(matriz_estruturas[i][celula->pos_x].casa->pos_y, matriz_estruturas[i][celula->pos_x].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
							/* casa ficou resolvida e chama-se a resolve_casas_adjacentes */
							alteracoes = 1;					
						}
					}
				}
			}				
		}			
	}	
	
	return alteracoes;
	/* devolve se fez alteracoes ou nao */
} 

int resolver_como_soma_menor(triangular * celula, int novas_possibilidades[9], int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* esta funcao analisa somas que ja tenham casas resolvidas e para uma soma menor feita a menos casas cruza estas novas hipoteses com as anteriores */
	int i, j;
	int valores_possiveis;
	int alteracoes = 0;
	
	/* soma na horizontal */
	if(celula->orientacao == 'h'){
		for(j=(celula->pos_x)+1; j <= (celula->pos_x) + celula->num_casas; j++){
			if(matriz_solucao[celula->pos_y][j] == 0){
				for(i=0; i<9; i++){
					matriz_estruturas[celula->pos_y][j].casa->valores[i] *= novas_possibilidades[i];
					/* cruza as hipoteses que a branca ja tinha com as novas da soma menor a menos casas */
				}
				valores_possiveis = 0;
				for(i=0; i<9; i++){
					if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){
						valores_possiveis++; /* verifica se a branca ficou resolvida */
					}
				}
				if(valores_possiveis == 1){ /* branca ficou resolvida */
					for(i=0; i<9; i++){
						if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){
							matriz_solucao[celula->pos_y][j] = i + 1; /* coloca na matriz de solucao o valor que se descobriu para essa branca */																				
						}										
						resolve_casas_adjacentes(matriz_estruturas[celula->pos_y][j].casa->pos_y, matriz_estruturas[celula->pos_y][j].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
						/* casa ficou resolvida e chama-se a resolve_casas_adjacentes */
						alteracoes = 1;
					}
				}
			}
		}
	}	
	
	/* soma na vertical */
	if(celula->orientacao == 'v'){
		for(i=(celula->pos_y)+1; i <= (celula->pos_y) + celula->num_casas; i++){
			if(matriz_solucao[i][celula->pos_x] == 0){
				for(j=0; j<9; j++){
					matriz_estruturas[i][celula->pos_x].casa->valores[j] *= novas_possibilidades[j];
					/* cruza as hipoteses que a branca ja tinha com as novas da soma menor a menos casas */
				}
				valores_possiveis = 0;
				for(j=0; j<9; j++){
					if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
						valores_possiveis++; /* verifica se a branca ficou resolvida */
					}
				}
				if(valores_possiveis == 1){ /* branca ficou resolvida */
					for(j=0; j<9; j++){
						if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
							matriz_solucao[i][celula->pos_x] = j + 1; /* coloca na matriz de solucao o valor que se descobriu para essa branca */																																								
						}										
						resolve_casas_adjacentes(matriz_estruturas[i][celula->pos_x].casa->pos_y, matriz_estruturas[i][celula->pos_x].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
						/* casa ficou resolvida e chama-se a resolve_casas_adjacentes */
						alteracoes = 1;
					}
				}
			}
		}
	}
	
	return alteracoes; /* devolve se fez alteracoes ou nao */
}

int possibilidades_exclusivas(triangular * celula, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* esta funcao verifica se numa soma existem possilibidades exclusivas de certas casas e retira-as entao às possibilidades das outras a que nao estao afectas */
	int i, j, k;
	int possibilidades_1, possibilidades_2;
	int casas_iguais;
	int vector_igual;
	int alteracoes = 0;

	/* soma na horizontal */
	if(celula->orientacao == 'h'){
		for(j=(celula->pos_x)+1; j <= (celula->pos_x) + celula->num_casas; j++){ /* percorrendo as brancas */
			casas_iguais = 1;
			possibilidades_1 = 0;
			for(i=0; i<9; i++){
				if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){
					possibilidades_1++; /* verifica quantas hipoteses tem uma branca */
				}
			}			
			for(k=j+1; k <= (celula->pos_x) + celula->num_casas; k++){ /* percorrendo as brancas a partir dessa */
				possibilidades_2 = 0;
				for(i=0; i<9; i++){
					if(matriz_estruturas[celula->pos_y][k].casa->valores[i] == 1){
						possibilidades_2++; /* verifica quantas hipoteses tem as restantes brancas */
					}
				}
				if(possibilidades_1 == possibilidades_2){ /* se as hipoteses forem iguais tem de verificar se sao exactamente as mesmas hipoteses */
					vector_igual = 1;
					for(i=0; i<9 ; i++){
						if(matriz_estruturas[celula->pos_y][j].casa->valores[i] != matriz_estruturas[celula->pos_y][k].casa->valores[i]){
							vector_igual = 0; /* descobriu um valor diferente nesses dois vectores e ja sabe que entao nao se aplica este algoritmo */
						}
					}
					if(vector_igual == 1){
						casas_iguais++; /* se os vectores sao iguais incrementa o numero de casas iguais */
					}
				}				
			}
			if(possibilidades_1 == casas_iguais){
				for(k=j+1; k <= (celula->pos_x) + celula->num_casas; k++){
					
					possibilidades_2 = 0;
					for(i=0; i<9; i++){
						if(matriz_estruturas[celula->pos_y][k].casa->valores[i] == 1){
							possibilidades_2++;
						}
					}
					if(possibilidades_2 == 1){
						break; /* casa ja esta resolvida, pode sair fora */
					}					
					
					vector_igual = 1;
					for(i=0; i<9; i++){
						if(matriz_estruturas[celula->pos_y][j].casa->valores[i] != matriz_estruturas[celula->pos_y][k].casa->valores[i]){
							vector_igual = 0;
						}
					}
					if(vector_igual == 0){
						for(i=0; i<9; i++){
							if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){ /* as possibilidades sao exclusivas das casas que tem vectores de hipoteses iguais */
								matriz_estruturas[celula->pos_y][k].casa->valores[i] = 0; /* retira essas hipoteses as restantes casas */
							}
						}
						possibilidades_2 = 0;
						for(i=0; i<9; i++){
							if(matriz_estruturas[celula->pos_y][k].casa->valores[i] == 1){
								possibilidades_2++;
							}
						}
						if(possibilidades_2 == 1){  /* casa ficou resolvida */
							for(i=0; i<9; i++){
								if(matriz_estruturas[celula->pos_y][k].casa->valores[i] == 1){
									matriz_solucao[celula->pos_y][k] = i+1; /* coloca valor na matriz de solucao */
								}
							}							
							resolve_casas_adjacentes(matriz_estruturas[celula->pos_y][k].casa->pos_y, matriz_estruturas[celula->pos_y][k].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
							/* branca ficou resolvida e chama-se a resolve_casas_adjacentes */
							alteracoes = 1;
						}
					}
				}
			}
		}
	}
	
	/* soma na vertical */
	if(celula->orientacao == 'v'){
		for(i=(celula->pos_y)+1; i <= (celula->pos_y) + celula->num_casas; i++){ /* percorrendo as brancas */
			casas_iguais = 1;
			possibilidades_1 = 0;
			for(j=0; j<9; j++){
				if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
					possibilidades_1++; /* verifica quantas hipoteses tem uma branca */
				}
			}			
			for(k=i+1; k <= (celula->pos_y) + celula->num_casas; k++){ /* percorrendo as brancas a partir dessa */
				possibilidades_2 = 0;
				for(j=0; j<9; j++){
					if(matriz_estruturas[k][celula->pos_x].casa->valores[j] == 1){ 
						possibilidades_2++; /* verifica quantas hipoteses tem as restantes brancas */
					}
				}
				if(possibilidades_1 == possibilidades_2){ /* se as hipoteses forem iguais tem de verificar se sao exactamente as mesmas hipoteses */
					vector_igual = 1;
					for(j=0; j<9 ; j++){
						if(matriz_estruturas[i][celula->pos_x].casa->valores[j] != matriz_estruturas[k][celula->pos_x].casa->valores[j]){
							vector_igual = 0; /* descobriu um valor diferente nesses dois vectores e ja sabe que entao nao se aplica este algoritmo */
						}
					}
					if(vector_igual == 1){
						casas_iguais++; /* se os vectores sao iguais incrementa o numero de casas iguais */
					}
				}				
			}
			if(possibilidades_1 == casas_iguais){
				for(k=i+1; k <= (celula->pos_y) + celula->num_casas; k++){
					
					possibilidades_2 = 0;
					for(j=0; j<9; j++){
						if(matriz_estruturas[k][celula->pos_x].casa->valores[j] == 1){
							possibilidades_2++;
						}
					}
					if(possibilidades_2 == 1){
						break; /* casa ja esta resolvida, pode sair fora */
					}					
					
					vector_igual = 1;
					for(j=0; j<9; j++){
						if(matriz_estruturas[i][celula->pos_x].casa->valores[j] != matriz_estruturas[k][celula->pos_x].casa->valores[j]){
							vector_igual = 0;
						}
					}
					if(vector_igual == 0){
						for(j=0; j<9; j++){
							if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){ /* as possibilidades sao exclusivas das casas que tem vectores de hipoteses iguais */
								matriz_estruturas[k][celula->pos_x].casa->valores[j] = 0; /* retira essas hipoteses as restantes casas */
							}
						}
						possibilidades_2 = 0;
						for(j=0; j<9; j++){
							if(matriz_estruturas[k][celula->pos_x].casa->valores[j] == 1){ /* verifica se resolveu alguma das restantes casas a que se tiraram essas hipoteses */
								possibilidades_2++;
							}
						}
						if(possibilidades_2 == 1){ /* casa ficou resolvida */
							for(j=0; j<9; j++){
								if(matriz_estruturas[k][celula->pos_x].casa->valores[j] == 1){
									matriz_solucao[k][celula->pos_x] = j+1; /* coloca valor na matriz de solucao */
								}
							}							
							resolve_casas_adjacentes(matriz_estruturas[k][celula->pos_x].casa->pos_y, matriz_estruturas[k][celula->pos_x].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
							/* branca ficou resolvida e chama-se a resolve_casas_adjacentes */
							alteracoes = 1;
						}
					}
				}
			}
		}
	}
	
	return alteracoes; /* devolve se fez alteracoes ou nao */
}			

int valores_max_min(triangular * celula, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que verifica se a soma dos maximos de uma soma ja a resolve e faz o mesmo com os minimos */
	int alteracoes = 0;
	int i,j;
	
	int soma_maiores = 0;
	int soma_menores = 0;
	
	int max_usados[9] = {0,0,0,0,0,0,0,0,0};
	int min_usados[9] = {0,0,0,0,0,0,0,0,0};
	
	int max_validos = 1;
	int min_validos = 1;
	
	int max = 0;
	int min = 9;
	
	/* soma na horizontal */
	if(celula->orientacao == 'h'){
		for(j=(celula->pos_x)+1; j <= (celula->pos_x) + celula->num_casas; j++){
			max = 0;
			min = 9;
			
			for(i=0; i<9; i++){
				if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){
					if(i > max){ 					
						max = i; /* procurar maximo dessa casa */	
					}
					if(i < min){						
						min = i; /* procurar minimo dessa casa */
					}
				}
			}
			
			/* a funcao aplica-se quando todos os maximos sao diferentes, e o mesmo vai para os minimos */
			if(max_usados[max] == 0){ /* verifica se esse maximo ja foi usado */
				max_usados[max] = 1;
			}
			else{
				max_validos = 0;
			}
			if(min_usados[min] == 0){ /* verifica se esse minimo ja foi usado */
				min_usados[min] = 1;
			}
			else{
				min_validos = 0;
			}
			
			soma_maiores += max+1; /* calcula a soma dos maximos dessas casas */
			soma_menores += min+1; /* calcula a soma dos minimos dessas casas */
		}		
		
		if(max_validos == 1 && celula->soma == soma_maiores){ /* os maximos sao todos diferentes e a soma destes corresponde a soma que se quer */
			for(j=(celula->pos_x)+1; j <= (celula->pos_x) + celula->num_casas; j++){
				max = 0;	
					
				for(i=0; i<9; i++){
					if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){
						if(i > max){							
							max = i;
						}
					}
				}
				
				for(i=0; i<9; i++){
					matriz_estruturas[celula->pos_y][j].casa->valores[i] = 0;
				}
				matriz_estruturas[celula->pos_y][j].casa->valores[max] = 1;	/* resolve cada casa consoante o seu valor maximo */			
				matriz_solucao[celula->pos_y][j] = max + 1; /* coloca esse valor na matriz de solucao */
				
				resolve_casas_adjacentes(matriz_estruturas[celula->pos_y][j].casa->pos_y, matriz_estruturas[celula->pos_y][j].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
				/* resolveu essa casa e chama a resolve_casas_adjacentes */
				alteracoes = 1;
			}				
		}
			
		if(min_validos == 1 && celula->soma == soma_menores){ /* os minimos sao todos diferentes e a soma destes corresponde a soma que se quer */
			for(j=(celula->pos_x)+1; j <= (celula->pos_x) + celula->num_casas; j++){
				min = 9;	
					
				for(i=0; i<9; i++){
					if(matriz_estruturas[celula->pos_y][j].casa->valores[i] == 1){
						if(i < min){
							min = i;
						}
					}
				}
				
				min_usados[min] = 1;
				
				for(i=0; i<9; i++){
					matriz_estruturas[celula->pos_y][j].casa->valores[i] = 0;
				}
				matriz_estruturas[celula->pos_y][j].casa->valores[min] = 1;	/* resolve cada casa consoante o seu valor minimo */				
				matriz_solucao[celula->pos_y][j] = min + 1; /* coloca esse valor na matriz de solucao */
				
				resolve_casas_adjacentes(matriz_estruturas[celula->pos_y][j].casa->pos_y, matriz_estruturas[celula->pos_y][j].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
				/* resolveu essa casa e chama a resolve_casas_adjacentes */
				alteracoes = 1;
			}			
		}
	}
			
	
	/* soma na vertical */
	if(celula->orientacao == 'v'){
		for(i=(celula->pos_y)+1; i <= (celula->pos_y) + celula->num_casas; i++){
			max = 0;
			min = 9;
			
			for(j=0; j<9; j++){
				if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
					if(j > max){ 
						max = j; /* procurar maximo dessa casa */	
					}
					if(j < min){
						min = j; /* procurar minimo dessa casa */	
					}
				}
			}
			
			/* a funcao aplica-se quando todos os maximos sao diferentes, e o mesmo vai para os minimos */
			if(max_usados[max] == 0){ /* verifica se esse maximo ja foi usado */
				max_usados[max] = 1;
			}
			else{
				max_validos = 0;
			}
			if(min_usados[min] == 0){ /* verifica se esse minimo ja foi usado */
				min_usados[min] = 1;
			}
			else{
				min_validos = 0;
			}
			
			soma_maiores += max+1; /* calcula a soma dos maximos dessas casas */
			soma_menores += min+1; /* calcula a soma dos minimos dessas casas */
		}
		
		if(max_validos == 1 && celula->soma == soma_maiores){ /* os maximos sao todos diferentes e a soma destes corresponde a soma que se quer */
			for(i=(celula->pos_y)+1; i <= (celula->pos_y) + celula->num_casas; i++){
				max = 0;	
					
				for(j=0; j<9; j++){
					if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
						if(j > max){
							max = j;
						}
					}
				}
				
				max_usados[max] = 1;
				
				for(j=0; j<9; j++){
					matriz_estruturas[i][celula->pos_x].casa->valores[j] = 0;
				}
				matriz_estruturas[i][celula->pos_x].casa->valores[max] = 1;	/* resolve cada casa consoante o seu valor maximo */				
				matriz_solucao[i][celula->pos_x] = max + 1; /* coloca esse valor na matriz de solucao */
				
				resolve_casas_adjacentes(matriz_estruturas[i][celula->pos_x].casa->pos_y, matriz_estruturas[i][celula->pos_x].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
				/* resolveu essa casa e chama a resolve_casas_adjacentes */
				alteracoes = 1;
			}				
		}
			
		if(min_validos == 1 && celula->soma == soma_menores){ /* os minimos sao todos diferentes e a soma destes corresponde a soma que se quer */
			for(i=(celula->pos_y)+1; i <= (celula->pos_y) + celula->num_casas; i++){
				min = 9;	
					
				for(j=0; j<9; j++){
					if(matriz_estruturas[i][celula->pos_x].casa->valores[j] == 1){
						if(j < min){
							min = j;
						}
					}
				}
				
				min_usados[min] = 1;
				
				for(j=0; j<9; j++){
					matriz_estruturas[i][celula->pos_x].casa->valores[j] = 0;
				}
				matriz_estruturas[i][celula->pos_x].casa->valores[min] = 1;	/* resolve cada casa consoante o seu valor minimo */			
				matriz_solucao[i][celula->pos_x] = min + 1; /* coloca esse valor na matriz de solucao */
				
				resolve_casas_adjacentes(matriz_estruturas[i][celula->pos_x].casa->pos_y, matriz_estruturas[i][celula->pos_x].casa->pos_x, altura, largura, matriz_estruturas, matriz_solucao);
				/* resolveu essa casa e chama a resolve_casas_adjacentes */
				alteracoes = 1;
			}			
		}			
	}
	
	return alteracoes; /* devolve se fez alteracoes ou nao */
}

int chama_algoritmos(triangular * celula, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que verifica os varios contextos do problema e determina qual os algoritmos que deve chamar */
	int casas_totais = celula->num_casas; /* verifica o numero de casas da soma */
	int casas_preenchidas;
	int i, j;
	int possibilidades = 0;
	int nova_soma;
	
	int valores_descobertos[9] = {0,0,0,0,0,0,0,0,0};
	int novas_possibilidades[9] = {0,0,0,0,0,0,0,0,0};
	
	/* para triangulares horizontais */
	if(celula->orientacao == 'h'){
		
		/* calcular as casas preenchidas */
		casas_preenchidas = 0;
		nova_soma = celula->soma;
		for(j=(celula->pos_x)+1; j <= (celula->pos_x) + casas_totais; j++){
			if(matriz_solucao[celula->pos_y][j] != 0){
				casas_preenchidas++;
				valores_descobertos[(matriz_solucao[celula->pos_y][j])-1] = 1;
				nova_soma -= matriz_solucao[celula->pos_y][j]; /* calcula qual a nova soma se ja houver casas preenchidas para uma dada soma */
			}
		}
		
		if(casas_totais - casas_preenchidas > 1){ 
			combinacoes(nova_soma, casas_totais - casas_preenchidas, novas_possibilidades);
				
			for(i=0; i<9; i++){
				if(novas_possibilidades[i] == 1){
					if(valores_descobertos[i] == 1){
						novas_possibilidades[i] = 0;
					}
					else{
						possibilidades++;
					}
				}
			}
		}
				
		/* para cada uma destas funcoes se elas devolveram 1 e porque fizeram alteracoes */
		/* chama_algoritmos devolve 1 tambem e como esta num ciclo volta a ser chamada ate detectar que nao faz mais alteracoes */
		
		/* usar completa soma */
		if(casas_totais - casas_preenchidas == 1){
			completar_soma(celula, valores_descobertos, altura, largura, matriz_estruturas, matriz_solucao);
			return 1;
		}
		
		/* calcula combinacoes de pares reais */
		if(casas_totais - casas_preenchidas == 2){
			if(combinacoes_pares_reais(celula, valores_descobertos, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}		
		}
		
		/* existem possibilidades exclusivas && teste dos maximos e minimos && eliminar maximos e minimos */
		if(casas_totais - casas_preenchidas > 2){
			if(possibilidades_exclusivas(celula, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}
			if(valores_max_min(celula, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}
			if(eliminar_maximos_minimos(celula, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}						
		}
		
		/* "n" possibilidades a "n" casas vazias */
		if(possibilidades == (casas_totais - casas_preenchidas) && possibilidades != 0){
			if(n_possibilidades_n_casas(celula, novas_possibilidades, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;			
			}
		}
		
		/* resolver como soma de menos casas */
		if(casas_preenchidas > 0){
			if(resolver_como_soma_menor(celula, novas_possibilidades, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}
		}
	}
	
	/* para triangulares verticais */
	if(celula->orientacao == 'v'){
		
		/* descobrir casas preenchidas */
		casas_preenchidas = 0;
		nova_soma = celula->soma;
		for(i=(celula->pos_y)+1; i <= (celula->pos_y) + casas_totais; i++){
			if(matriz_solucao[i][celula->pos_x] != 0){
				casas_preenchidas++;
				valores_descobertos[(matriz_solucao[i][celula->pos_x])-1] = 1;
				nova_soma -= matriz_solucao[i][celula->pos_x];
			}
		}
		
		if(casas_totais - casas_preenchidas > 1){
			combinacoes(nova_soma, casas_totais - casas_preenchidas, novas_possibilidades);
			
			for(i=0; i<9; i++){
				if(novas_possibilidades[i] == 1){
					if(valores_descobertos[i] == 1){
						novas_possibilidades[i] = 0;
					}
					else{
						possibilidades++;
					}
				}
			}
		}		
		
		/* para cada uma destas funcoes se elas devolveram 1 e porque fizeram alteracoes */
		/* chama_algoritmos devolve 1 tambem e como esta num ciclo volta a ser chamada ate detectar que nao faz mais alteracoes */
		
		/* usar completa soma */
		if(casas_totais - casas_preenchidas == 1){
			completar_soma(celula, valores_descobertos, altura, largura, matriz_estruturas, matriz_solucao);
			return 1;
		}
		
		/* calcula combinacoes de pares reais */
		if(casas_totais - casas_preenchidas == 2){
			if(combinacoes_pares_reais(celula, valores_descobertos, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}		
		}
		
		/* existem possibilidades exclusivas && teste dos maximos e minimos */
		if(casas_totais - casas_preenchidas > 2){
			if(possibilidades_exclusivas(celula, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}
			if(valores_max_min(celula, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}
			if(eliminar_maximos_minimos(celula, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}	
		}		
		
		/* "n" possibilidades a "n" casas vazias */
		if(possibilidades == (casas_totais - casas_preenchidas) && possibilidades != 0){
			if(n_possibilidades_n_casas(celula, novas_possibilidades, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;			
			}
		}
		
		/* resolver como soma de menos casas */
		if(casas_preenchidas > 0){
			if(resolver_como_soma_menor(celula, novas_possibilidades, altura, largura, matriz_estruturas, matriz_solucao) == 1){
				return 1;
			}
		}
	}
		
	return 0; /* devolve 0 entao significa que entretano nao devolveu ja 1 e sabe que nao foram feitas alteracoes e esta na altura da chama_algoritmos sair do ciclo onde esta inserida */	
}

int procura_soma_triangulares(int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
/* funcao que procura celulas triangulares ao longo do puzzle, sempre a verificar quantas casas de cada soma ja estao preenchidas */
	int i, j, l, m;
	int alteracoes;
	int soma_min, soma_max, preenchidas;
	int contagem;
	int nova_soma;
	
	do{
		alteracoes = 0;
		for(i=0; i<altura; i++){
			for(j=0; j<largura; j++){
				
				/* aqui iniciam-se varios testes que sao principalmente pertinentes para quando se aplica tentativa e erro e se quer testar se um dado numero ja esta mal ou nao */
				
				/* triangular inferior */
				if(matriz_estruturas[i][j].triangulo_inferior != NULL){
					soma_min = 0;
					soma_max = 0;
					preenchidas = 0;			
					for(l = (matriz_estruturas[i][j].triangulo_inferior->pos_y)+1; l <= (matriz_estruturas[i][j].triangulo_inferior->pos_y) + (matriz_estruturas[i][j].triangulo_inferior->num_casas); l++){
						
						soma_min += matriz_solucao[l][j]; /* determinar a soma dos minimos de todas as casas */ 
						soma_max += matriz_solucao[l][j]; /* determinar a soma dos maximos de todas as casas */
						
						if(matriz_solucao[l][j] == 0){
							for(m=0; m<9; m++){
								if(matriz_estruturas[l][j].casa->valores[m] == 1){
									soma_min += m + 1;
									break;
								}
							}
						} /* somar todos os valores dessa soma, se houver casas nao preenchidas assume-se o valor minimo dentro das hipoteses que elas podem tomar */
						
						if(matriz_solucao[l][j] == 0){
							for(m=8; m>=0; m--){
								if(matriz_estruturas[l][j].casa->valores[m] == 1){
									soma_max += m + 1;
									break;
								}
							}
						} /* somar todos os valores dessa soma, se houver casas nao preenchidas assume-se o valor maximo dentro das hipoteses que elas podem tomar */
						
						if(matriz_solucao[l][j] != 0){
							preenchidas++;
						}
						
						contagem = 0;
						for(m=0; m<9; m++){
							if(matriz_estruturas[l][j].casa->valores[m] == 1){
								contagem++; /* conta quantas hipoteses tem as varias brancas */
							}
						}
						if(contagem == 0){ 
							/* ao testar com o valor k+1 uma das brancas da soma ficou sem hipoteses, é impossivel o valor entao */
							return 0; 
							/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */
						}
					}							
					
					if(preenchidas > 0){	
						if((preenchidas == matriz_estruturas[i][j].triangulo_inferior->num_casas) && (soma_min != matriz_estruturas[i][j].triangulo_inferior->soma)){
							/* as casas estao todas preenchidas mas a sua soma e diferente da soma prentendida, entao essa valor com que se testou ja esta mal */
							return 0;
							/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */ 
						}
						
						if(preenchidas < matriz_estruturas[i][j].triangulo_inferior->num_casas){
							if((matriz_estruturas[i][j].triangulo_inferior->num_casas) - preenchidas > 1){
								nova_soma = matriz_estruturas[i][j].triangulo_inferior->soma;
							
								for(l = (matriz_estruturas[i][j].triangulo_inferior->pos_y)+1; l <= (matriz_estruturas[i][j].triangulo_inferior->pos_y) + (matriz_estruturas[i][j].triangulo_inferior->num_casas); l++){
									if(matriz_solucao[l][j] != 0){
										nova_soma -= matriz_solucao[l][j];
									}
								}				
							
								if(combinacoes_tentativa_erro(nova_soma, (matriz_estruturas[i][j].triangulo_inferior->num_casas) - preenchidas) == 0){
								/* esta funcao devolve 0 quando detecta que uma dada soma ja nao e possivel com um dado numero de casas, e entao o valor testado esta mal */
									return 0;
									/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */ 
								}
							}							
							
							if(soma_min >  matriz_estruturas[i][j].triangulo_inferior->soma){ 
							/* numero de casas preenchidas e menor mas a soma ja e impossivel por a soma dos minimos ser maior que a soma pretendida */
								return 0;
								/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */ 								
							} 
						
							if(soma_max <  matriz_estruturas[i][j].triangulo_inferior->soma){ 
							/* numero de casas preenchidas e menor mas a soma ja e impossivel por a soma dos maximos ser menor que a soma pretendida */
								return 0;
								/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */ 								
							}
						}
					}	

					if(chama_algoritmos(matriz_estruturas[i][j].triangulo_inferior, altura, largura, matriz_estruturas, matriz_solucao) == 1){
						alteracoes = 1;
						/* a chama_algoritmo devolveu 1 logo esta a fazer alteracoes e entao esta funcao muda uma variavel para 1 tambem para manter o chama_algoritmos num ciclo */
						/* quando a chama_algoritmos devolver 0, entao aqui saira do ciclo */
					}
				}
				
										
				/* triangular superior */
				if(matriz_estruturas[i][j].triangulo_superior != NULL){
					soma_min = 0;
					soma_max = 0;
					preenchidas = 0;
					
					for(l = (matriz_estruturas[i][j].triangulo_superior->pos_x)+1; l <= (matriz_estruturas[i][j].triangulo_superior->pos_x) + (matriz_estruturas[i][j].triangulo_superior->num_casas); l++){
							
						soma_min += matriz_solucao[i][l]; /* determinar a soma dos minimos de todas as casas */ 
						soma_max += matriz_solucao[i][l]; /* determinar a soma dos maximos de todas as casas */ 
						
						if(matriz_solucao[i][l] == 0){
							for(m=0; m<9; m++){
								if(matriz_estruturas[i][l].casa->valores[m] == 1){
									soma_min += m + 1;
									break;
								}
							}
						} /* somar todos os valores dessa soma, se houver casas nao preenchidas assume-se o valor minimo dentro das hipoteses que elas podem tomar */
						
						if(matriz_solucao[i][l] == 0){
							for(m=8; m>=0; m--){
								if(matriz_estruturas[i][l].casa->valores[m] == 1){
									soma_max += m + 1;
									break;
								}
							}
						} /* somar todos os valores dessa soma, se houver casas nao preenchidas assume-se o valor maximo dentro das hipoteses que elas podem tomar */
						
						if(matriz_solucao[i][l] != 0){
							preenchidas++;
						}
						
						contagem = 0;
						for(m=0; m<9; m++){
							if(matriz_estruturas[i][l].casa->valores[m] == 1){
								contagem++; /* conta quantas hipoteses tem as varias brancas */
							}
						}
						if(contagem == 0){ 
							/* ao testar com o valor k+1 uma das brancas da soma ficou sem hipoteses, é impossivel */
							return 0;
							/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */ 
						}
					}							
						
					if(preenchidas > 0){	
						if((preenchidas == matriz_estruturas[i][j].triangulo_superior->num_casas) && (soma_min != matriz_estruturas[i][j].triangulo_superior->soma)){
							return 0;
							/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */ 								
						}
						
						if(preenchidas < matriz_estruturas[i][j].triangulo_superior->num_casas){
							if((matriz_estruturas[i][j].triangulo_superior->num_casas) - preenchidas > 1){
								nova_soma = matriz_estruturas[i][j].triangulo_superior->soma;
							
								for(l = (matriz_estruturas[i][j].triangulo_superior->pos_x)+1; l <= (matriz_estruturas[i][j].triangulo_superior->pos_x) + (matriz_estruturas[i][j].triangulo_superior->num_casas); l++){
									if(matriz_solucao[i][l] != 0){
										nova_soma -= matriz_solucao[i][l];
									}
								}
							
								if(combinacoes_tentativa_erro(nova_soma, (matriz_estruturas[i][j].triangulo_superior->num_casas) - preenchidas) == 0){
								/* esta funcao devolve 0 quando detecta que uma dada soma ja nao e possivel com um dado numero de casas, e entao o valor testado esta mal */	
									return 0;
									/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */ 								
								}
							}
							
							if(soma_min >  matriz_estruturas[i][j].triangulo_superior->soma){ 
							/* numero de casas preenchidas e menor mas a soma ja e impossivel por a soma dos minimos ser maior que a soma pretendida */
								return 0;
								/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */ 																
							} 
						
							if(soma_max <  matriz_estruturas[i][j].triangulo_superior->soma){ 
							/* numero de casas preenchidas e menor mas a soma ja e impossivel por a soma dos maximos ser menor que a soma pretendida */
								return 0;
								/* devolve 0 para que a funcao que aplica o metodo de tentativa e erro saiba que o valor que esta a testar numa dada branca esta mal */ 																
							}
						}
					}	

					if(chama_algoritmos(matriz_estruturas[i][j].triangulo_superior, altura, largura, matriz_estruturas, matriz_solucao) == 1){
						alteracoes = 1;
						/* a chama_algoritmo devolveu 1 logo esta a fazer alteracoes e entao esta funcao muda uma variavel para 1 tambem para manter o chama_algoritmos num ciclo */
						/* quando a chama_algoritmos devolver 0, entao aqui saira do ciclo */
					}
				}
			}
		}
	} while(alteracoes == 1); 
	/* tudo isto e feito num ciclo ate se detectar que o valor de alteracoes mudou para 0, ou seja, que o puzzle ou esta resolvido ou encravou na resoluçao usando apenas restriçoes */
	
	return 1;
	/* se devolver 1 entao nao detectou nenhum erro ao longo da solucao do puzzle */	
}

void encontra_casa_teste(int * pos_y, int * pos_x, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]){
	int i,j,k;
	int possibilidades;
	int procurar_n_hipoteses = 2;
	
	do{
		for(i=0; i<altura; i++){
			for(j=0; j<largura; j++){
				if(matriz_solucao[i][j] == 0){ /* significa que casa branca está vazia */							
					possibilidades = 0;
					for(k=0; k<9; k++){
						if(matriz_estruturas[i][j].casa->valores[k] == 1){
							possibilidades++;
						}
					}
					if(possibilidades == procurar_n_hipoteses){
						(*pos_y) = i;
						(*pos_x) = j;						
						return;
					}						
				}	
			}		
		}
		procurar_n_hipoteses++;
	} while(procurar_n_hipoteses <= 9);
	
	return;
}

int testa_hipoteses(int i, int j, fila_casas ** ultima, int altura, int largura, int matriz[altura][largura], celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura], celula_matriz matriz_estruturas_temporaria[altura][largura], int matriz_solucao_temporaria[altura][largura]){
/* funcao que aplica o metodo de tentativa e erro, ou seja, o metodo de tentativa-e-erro para continuar a resolver o puzzle */	
	int x, y, k, l, a, r, m;
	int possibilidades = 0;
	int valores_testados[9] = {0,0,0,0,0,0,0,0,0};
	int incompleto = 0;
	
	/* copiar a matriz */
	for(y=0; y<altura; y++){
		for(x=0; x<largura; x++){
			if(matriz_estruturas[y][x].casa != NULL){
				for(k=0; k<9; k++){
					matriz_estruturas_temporaria[y][x].casa->valores[k] = matriz_estruturas[y][x].casa->valores[k];
				}				
			}
			matriz_solucao_temporaria[y][x] = matriz_solucao[y][x];
		}
	}
	 	
	*ultima = push_coordenadas(i, j, *ultima);
				
	for(k=0; k<9; k++){
		valores_testados[k] = 0; /* inicializa cada valor testado nessa casa a zero */ 
	}
					
	for(k=0; k<9; k++){
		if(matriz_estruturas[i][j].casa->valores[k] == 1 && valores_testados[k] == 0){ /* é um valor possivel nessa casa e ainda não foi testado */

			valores_testados[k] = 1; /* testamos casa com valor k+1 */
			for(l = 0; l<9; l++){
				matriz_estruturas_temporaria[i][j].casa->valores[l] = 0;
			} 
			matriz_estruturas_temporaria[i][j].casa->valores[k] = 1;
			matriz_solucao_temporaria[i][j] = k+1;
			/* na matriz temporaria unico valor no vector de numeros possiveis passa a ser k+1 */
			
			do{
				/* inicio de verificacoes de erros */
				
				/* chama-se a funçao de resolver a casa e verifica-se se deu erros */
				if(resolve_casas_adjacentes(i, j, altura, largura, matriz_estruturas_temporaria, matriz_solucao_temporaria) == 0){
					matriz_estruturas[i][j].casa->valores[k] = 0; /* resolve casas deu erro e o valor é impossivel */
					possibilidades = 0;
					for(l=0; l<9; l++){
						if(matriz_estruturas[i][j].casa->valores[l] == 1){
							possibilidades++;
						}
					}
					if(possibilidades == 1){
						for(l=0; l<9; l++){
							if(matriz_estruturas[i][j].casa->valores[l] == 1){
								matriz_solucao[i][j] = l+1;
							}
						}
						resolve_casas_adjacentes(i, j, altura, largura, matriz_estruturas, matriz_solucao);
						return 1;
					}
					for(r=0; r<altura; r++){
						for(a=0; a<largura; a++){
							if(matriz_estruturas[r][a].casa != NULL){
								for(m=0; m<9; m++){
									matriz_estruturas_temporaria[r][a].casa->valores[m] = matriz_estruturas[r][a].casa->valores[m];
								}
							}
						}
					} /* volta a copiar a matriz de estruturas ja alterada para a temporaria */
							
					for(r=0; r<altura; r++){
						for(a=0; a<largura; a++){
							matriz_solucao_temporaria[r][a] = matriz_solucao[r][a];
						}
					} /* volta a copiar a matriz solucao */	
					
					break;
				}
				
				/* chama-se a funçao de procurar por triangulos e verifica-se se deu erros */	
				if(procura_soma_triangulares(altura, largura, matriz_estruturas_temporaria, matriz_solucao_temporaria) == 0){
					matriz_estruturas[i][j].casa->valores[k] = 0; /* procura de triangulos deu erro e valor é impossivel */
					possibilidades = 0;
					for(l=0; l<9; l++){
						if(matriz_estruturas[i][j].casa->valores[l] == 1){
							possibilidades++;
						}
					}
					if(possibilidades == 1){
						for(l=0; l<9; l++){
							if(matriz_estruturas[i][j].casa->valores[l] == 1){
								matriz_solucao[i][j] = l+1;
							}
						}
						resolve_casas_adjacentes(i, j, altura, largura, matriz_estruturas, matriz_solucao);
						return 1;
					}
					for(r=0; r<altura; r++){
						for(a=0; a<largura; a++){
							if(matriz_estruturas[r][a].casa != NULL){
								for(m=0; m<9; m++){
									matriz_estruturas_temporaria[r][a].casa->valores[m] = matriz_estruturas[r][a].casa->valores[m];
								}
							}
						}
					} /* volta a copiar a matriz de estruturas ja alterada para a temporaria */
							
					for(r=0; r<altura; r++){
						for(a=0; a<largura; a++){
							matriz_solucao_temporaria[r][a] = matriz_solucao[r][a];
						}
					} /* volta a copiar a matriz solucao */	
					
					break;
				}	
				
				/* nao deu erros */
				incompleto = 0;
				
				for(r=0; r<altura; r++){
					for(a=0; a<largura; a++){
						if(matriz_solucao_temporaria[r][a] == 0){				
							incompleto = 1;
						}
					}
				}							
				
				if(incompleto == 1){ 
					int matriz_solucao_temporaria_2[altura][largura];
					celula_matriz matriz_estruturas_temporaria_2[altura][largura];
					cria_matrizes_temporarias(altura, largura, matriz, matriz_estruturas_temporaria, matriz_estruturas_temporaria_2, matriz_solucao_temporaria, matriz_solucao_temporaria_2);
					
					y = i;
					x = j;
					
					int adjacente_vazia = 0;
											
					do{	
						if(*ultima == NULL){
							break;
						}
						
						*ultima = pop_coordenadas(&y, &x, *ultima);
						
						for(r=(matriz_estruturas[y][x].casa->inferior->pos_y) + 1; r <= (matriz_estruturas[y][x].casa->inferior->pos_y) + (matriz_estruturas[y][x].casa->inferior->num_casas); r++){
							if(matriz_solucao[r][x] == 0){
								adjacente_vazia = 1;
								testa_hipoteses(r, x, ultima, altura, largura, matriz, matriz_estruturas_temporaria, matriz_solucao_temporaria, matriz_estruturas_temporaria_2, matriz_solucao_temporaria_2);
								break;
							}
						}
						
						if(adjacente_vazia == 0){
							for(a=(matriz_estruturas[y][x].casa->superior->pos_x) + 1; a <= (matriz_estruturas[y][x].casa->superior->pos_x) + (matriz_estruturas[y][x].casa->superior->num_casas); a++){
								if(matriz_solucao[y][a] == 0){
									adjacente_vazia = 1;
									testa_hipoteses(y, a, ultima, altura, largura, matriz, matriz_estruturas_temporaria, matriz_solucao_temporaria, matriz_estruturas_temporaria_2, matriz_solucao_temporaria_2);
									break;
								}
							}
						}										
										
					}	while(adjacente_vazia == 0);						
					
					if(*ultima == NULL && adjacente_vazia == 0){
						encontra_casa_teste(&y, &x, altura, largura, matriz_estruturas, matriz_solucao);
						testa_hipoteses(y, x, ultima, altura, largura, matriz, matriz_estruturas_temporaria, matriz_solucao_temporaria, matriz_estruturas_temporaria_2, matriz_solucao_temporaria_2);
					}													
					
					limpa_memoria(altura, largura, matriz_estruturas_temporaria_2);								
				} /* com a escoha deste numero encravou-se na resoluçao do puzzle e tem de se recorrer a outra escolha possivel noutra branca */
				
				incompleto = 0;
					
				for(r=0; r<altura; r++){
					for(a=0; a<largura; a++){
						if(matriz_solucao_temporaria[r][a] == 0){				
							incompleto = 1;
						}
					}
				}	
				
				/*ghetto programming!*/
				if (incompleto == 0 && analisa_matriz_solucao(altura, largura, matriz_estruturas_temporaria, matriz_solucao_temporaria) == 0){
					for(i=0; i<altura; i++){
						for(j=0; j<largura; j++){
							if(matriz_estruturas[i][j].casa != NULL){
								for(k=0; k<9; k++){
									matriz_estruturas[i][j].casa->valores[k] = matriz_estruturas_temporaria[i][j].casa->valores[k];					
								}				
							}
							matriz_solucao[i][j] = matriz_solucao_temporaria[i][j];
						}
					}
					return 0;
				}				 						
				/* */					
					
			} while (incompleto == 1);																					
		}
	}			
	
	for(i=0; i<altura; i++){
		for(j=0; j<largura; j++){
			if(matriz_estruturas[i][j].casa != NULL){
				for(k=0; k<9; k++){
					matriz_estruturas[i][j].casa->valores[k] = matriz_estruturas_temporaria[i][j].casa->valores[k];					
				}				
			}
			matriz_solucao[i][j] = matriz_solucao_temporaria[i][j];
		}
	}				
	
	return 0;
}
