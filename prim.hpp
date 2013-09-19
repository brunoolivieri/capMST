#ifndef PRIM_HPP
#define PRIM_HPP

#include "grafo.hpp"

const unsigned INFINITO = 100000000;

unsigned primAVL(Grafo grafo, int M[81][81], int res[81][81]);

unsigned primVetor(Grafo grafo, int M[81][81],int res[81][81]);

unsigned dfs(Grafo grafo);

#endif // PRIM_HPP
