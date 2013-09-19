#include <vector>
#include <set>
#include <fstream>
#include "prim.hpp"

using namespace std;

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::seconds segundos;

unsigned primAVL(Grafo grafo, int M[81][81], int res[81][81]){
    ///Lista de Adjacência
    vector<std::vector<aresta> > adj = grafo.listaAdj;
    vector<int> adjTree(grafo.qtdNos+1,-1); ///árvore resultante

    unsigned tamMST = 0; ///qtd de Nós na MST
    unsigned custoMST = 0; ///Custo atual da MST

    /// Distância de todos os nós para a MST
    vector<unsigned> prioridadeAtual(adj.size(),INFINITO);
    prioridadeAtual[0] = 0; ///Nó inicial (0), custo 0

    /// AVL - Árvore Rubro-Negra
    set< pair<unsigned,unsigned> > AVL;

    ///Ponteiros para os nós da ávore
    vector< set< pair<unsigned,unsigned> >::iterator > ponteiro(adj.size());

    ///Insere na árvore o nó 0 com distância 0
    ponteiro[0] = AVL.insert( make_pair(0,0) ).first;

    ///Demais nós com distância infinita
    for (unsigned i=1;i<adj.size();i++) {
        ponteiro[i] = (AVL.insert( make_pair(INFINITO,i) )).first;
    }

    ///Verifica se o nó já faz parte da MST
    vector<bool> inTree(adj.size(),false);


    while(tamMST!=adj.size()){

        tamMST++;

        /// Pega o nó com maior prioridade
        pair<unsigned,unsigned> node;
        node = *AVL.begin();
        AVL.erase(AVL.begin());

        /// Marca como visitado
        inTree[node.second] = true;

        custoMST += node.first;///Incrementa o custo

        /// Verifica a adjacência do nó atual
        for (vector<aresta>::iterator i = adj[node.second].begin();i != adj[node.second].end(); i++) {
            if (inTree[(*i).destino] == false && (*i).peso < prioridadeAtual[(*i).destino]) {
                if(!M[(*i).origem][(*i).destino])
                    continue;
                /// Atualiza na árvore a prioridade do nó
                AVL.erase( ponteiro[(*i).destino] );
                prioridadeAtual[(*i).destino] = (*i).peso;
                ponteiro[(*i).destino] = AVL.insert( make_pair((*i).peso,(*i).destino) ).first;
                adjTree[(*i).destino] =  (*i).origem;
            }
        }
    }

    ///Preenche a matriz de resposta
    for(int i=0;i<81;i++)
        for(int j=0;j<81;j++)
            res[i][j] = 0;

    for(unsigned i=0;i<adjTree.size();i++)
    {
        if(adjTree[i]==-1)
            continue;
        res[i][adjTree[i]] = 1;
        res[adjTree[i]][i] = 1;
    }

    return custoMST;
}

unsigned primVetor(Grafo grafo, int M[81][81],int res[81][81]){
    ///Lista de Adjacência
    vector<std::vector<aresta> > adj = grafo.listaAdj;
    vector<int> adjTree(grafo.qtdNos+1,-1); ///árvore resultante

    unsigned tamMST = 0; ///qtd de Nós na MST
    unsigned custoMST = 0; ///Custo atual da MST

    /// Distância de todos os nós para a MST
    vector<unsigned> prioridadeAtual(adj.size(),INFINITO);
    prioridadeAtual[0] = 0; ///Nó inicial (0), custo 0

    ///Verifica se o nó já faz parte da MST
    vector<bool> inTree(adj.size(),false);

    while(tamMST!=adj.size()){
        tamMST++;

        ///Remove o de maior prioridade
        unsigned menor = INFINITO;
        int posMenor = -1;

        for(unsigned i=0;i<prioridadeAtual.size();i++){
            if(inTree[i]==false && prioridadeAtual[i]<menor){
                menor = prioridadeAtual[i];
                posMenor = i;
            }
        }

        inTree[posMenor] = true;
        custoMST += prioridadeAtual[posMenor];

        /// Verifica a adjacência do nó atual
        for (vector<aresta>::iterator i = adj[posMenor].begin();i != adj[posMenor].end(); i++) {
	    /// Verifica se a adjacência é válida
	    if(!M[(*i).origem][(*i).destino])
            continue;
            /// Atualiza na árvore a prioridade do nó
            if (inTree[(*i).destino] == false && (*i).peso < prioridadeAtual[(*i).destino]) {
                prioridadeAtual[(*i).destino] = (*i).peso;
                adjTree[(*i).destino] =  (*i).origem;
            }
        }

    }

    ///Preenche a matriz de resposta
    for(int i=0;i<81;i++)
        for(int j=0;j<81;j++)
            res[i][j] = 0;

    for(unsigned i=0;i<adjTree.size();i++)
    {
        if(adjTree[i]==-1)
            continue;
        res[i][adjTree[i]] = 1;
        res[adjTree[i]][i] = 1;
    }

    return custoMST;
}

void dfsVisit(Grafo &grafo,vector<unsigned> &cor,unsigned no){
    for(vector<aresta>::iterator i = grafo.listaAdj[no].begin();i != grafo.listaAdj[no].end(); i++){
        if(cor[(*i).destino] == 0){
            cor[(*i).destino] = 1;
            dfsVisit(grafo,cor,(*i).destino);
        }
    }
}

unsigned dfs(Grafo grafo){
    vector<unsigned> cor(grafo.listaAdj.size(),0); //td mundo branco
    unsigned qtd=0;
    for(unsigned i=0;i<grafo.listaAdj.size();i++){
        if(cor[i] == 0){
            qtd++;
            dfsVisit(grafo,cor,i);
        }
    }
    return qtd;
}
