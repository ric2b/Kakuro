#ifndef __RESOLVE_H_
#define __RESOLVE_H_

#include <stdio.h>
#include <stdlib.h>
#include "kakuro.h"
#include "combinacoes.h"

int resolve_casas_adjacentes(int y, int x, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int cruza_somas(int y, int x, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
void completar_soma(triangular * celula, int valores_descobertos[9], int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int combinacoes_pares_reais(triangular * celula, int valores_descobertos[9], int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int eliminar_maximos_minimos(triangular * celula, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int n_possibilidades_n_casas(triangular * celula, int novas_possibilidades[9], int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int resolver_como_soma_menor(triangular * celula, int novas_possibilidades[9], int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int possibilidades_exclusivas(triangular * celula, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int valores_max_min(triangular * celula, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int chama_algoritmos(triangular * celula, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int procura_soma_triangulares(int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
void encontra_casa_teste(int * pos_y, int * pos_x, int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
int testa_hipoteses(int pos_y, int pos_x, fila_casas ** ultima, int altura, int largura,  int matriz[altura][largura], celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura], celula_matriz matriz_estruturas_temporaria[altura][largura], int matriz_solucao_temporaria[altura][largura]); 

#endif
