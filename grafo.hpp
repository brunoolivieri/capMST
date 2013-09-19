#ifndef GRAFO_HPP_INCLUDED
#define GRAFO_HPP_INCLUDED

#include<vector>
#include<iostream>
#include<chrono>

typedef std::chrono::milliseconds milliseconds;

typedef struct aresta{
    unsigned origem;
    unsigned destino;
    unsigned peso;
    unsigned visitado; //Flag para verificar se uma dada aresta foi visitada
    aresta(unsigned a,unsigned b,unsigned c) : origem(a), destino(b), peso(c),visitado(0){};
}aresta;

class Grafo{
private:

public:
    Grafo(const char * fileName);
    Grafo();
    ~Grafo();
    void addVertice();
    void addAresta(unsigned origem, unsigned destino, unsigned peso);
    void imprimeLista();
    void imprimeMatriz();
    void writeOutput(const char * estrategia,const char * nome, milliseconds time,unsigned iteracoes,unsigned custo);

    std::vector<std::vector<aresta> > listaAdj;
    std::vector<std::vector<int> > matrizAdj;

    const char * nomeArquivo;
    unsigned qtdNos;
 };

#endif // GRAFO_HPP_INCLUDED
