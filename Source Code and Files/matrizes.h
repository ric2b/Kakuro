#ifndef __MATRIZES_H_
#define __MATRIZES_H_

#include <stdio.h>
#include <stdlib.h>
#include "kakuro.h"
#include "resolve.h"
#include "combinacoes.h"

branca * cria_branca(int pos_y, int pos_x, int altura, int largura, celula_matriz matriz_estruturas[altura][largura]);
triangular * cria_triangulo(int valor, char orientacao, int pos_y, int pos_x, int altura, int largura, int matriz[altura][largura]);
void cria_matriz_estruturas(int altura, int largura, int matriz[altura][largura], celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
void cria_matriz_solucao(int altura, int largura, int matriz[altura][largura], int matriz_solucao[altura][largura]);
int analisa_matriz_solucao(int altura, int largura, celula_matriz matriz_estruturas[altura][largura], int matriz_solucao[altura][largura]);
void cria_matrizes_temporarias(int altura, int largura, int matriz[altura][largura], celula_matriz matriz_estruturas[altura][largura], celula_matriz matriz_estruturas_temporaria[altura][largura], int matriz_solucao[altura][largura], int matriz_solucao_temporaria[altura][largura]);
fila_casas * push_coordenadas(int i, int j, fila_casas * ultima);
fila_casas * pop_coordenadas(int * i, int *j, fila_casas * ultima);
void limpa_memoria(int altura, int largura, celula_matriz matriz_estruturas[altura][largura]);

#endif
