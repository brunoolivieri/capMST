#include "grafo.hpp"
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

/* OBS: Todos os nós são identificados na entrada de 1 à N
*  e na aplicação de 0 à N-1. Nó(N) = N-1;
*/

using namespace std;

Grafo::Grafo(const char * fileName){
    nomeArquivo = fileName;
    ifstream in( fileName );
    string palavra;

    if(!in){
        printf("Nao foi possivel abrir o arquivo %s\n",fileName);
        exit(EXIT_FAILURE);
    }

    in >> qtdNos;

    listaAdj.resize(qtdNos+1);
    matrizAdj.resize(qtdNos+1);

    for(unsigned i=0;i<qtdNos+1;i++){
	matrizAdj[i].resize(qtdNos+1);
    }
    
    unsigned peso;

    for(unsigned i=0;i<qtdNos+1;i++){
       for(unsigned j=0;j<qtdNos+1;j++){
	  in >> peso;
  	  matrizAdj[i][j] = peso;
	  addAresta(i,j,peso);
       }
    }
    
    in.close();
}

Grafo::Grafo(){

    cout << "Insira a quantidade de vertices" << endl;
    unsigned tam;
    cin >> tam;

    listaAdj.resize(tam);
    unsigned peso;

    for(unsigned i=0;i<tam+1;i++){
	for(unsigned j=i;j<tam+1;j++){
	  cout << "Digite o peso do no " << i << " para o no " << j << endl;
	  cin >> peso;
          matrizAdj[i][j] = peso;
	  matrizAdj[j][i] = peso;
	  addAresta(i,j,peso);
	  addAresta(j,i,peso);
        }
    }
}

void Grafo::addVertice(){

}

void Grafo::writeOutput(const char * estrategia,const char * nomeArquivo, milliseconds time,unsigned iteracoes,unsigned custo){
    ofstream out;
    out.open("resultados.txt",ofstream::out | ofstream::app);
    out << "Futuro Log" << endl;
    out.close();
}

void Grafo::addAresta(unsigned origem,unsigned destino,unsigned peso){
    aresta novaAresta(origem,destino,peso);
    listaAdj[origem].push_back(novaAresta);
}

void Grafo::imprimeLista(){
    for(unsigned i=0;i<listaAdj.size();i++){
        for(unsigned j=0;j<listaAdj[i].size();j++)
            cout << listaAdj[i][j].origem << " " << listaAdj[i][j].destino << " " << listaAdj[i][j].peso << endl;
    }
}

void Grafo::imprimeMatriz(){
    for(unsigned i=0;i<qtdNos+1;i++){
	for(unsigned j=0;j<qtdNos+1;j++){
	  cout << matrizAdj[i][j] << " ";
        }
	cout << endl;
    }
    cout << endl;
}

Grafo::~Grafo(){

}
