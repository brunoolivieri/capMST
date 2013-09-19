#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <chrono> ///g++ -std=gnu++0x
#include "grafo.hpp"
#include "prim.hpp"
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>


using namespace std;

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::seconds segundos;

std::chrono::seconds refTime(3600);   // Rodar pelo menos 5 segundos
Clock::time_point t0,t1;
segundos tempo;

#define N 81

int solucao = 999999;
int n;
int M[N][N];
int arvorePrim[N][N];
int melhorArvorePrim[N][N];
long unsigned  podas_por_dfs,cont,podas_por_upper_bound,podas_por_grau_minimo, podas_por_lower_bound;
int VetorQ[3] = {3,5,10};
int Q;
char arq_resultados[80];
char arq_respostas[80];
char str_temp[50];
char nome_solucao[50];
char nome_instancia[50];
char tipo_UBond[50];


vector<vector<int>> custos;

///Função que dada uma profundidade P, retornar a quantidade de nós filhos + 1
long unsigned qtdNosArvore(int profundidadeAtual){
    ///Profundidade Total - Profundidade Atual
    long unsigned profundidade = ((n+1)*n)/2 - profundidadeAtual;
    return (unsigned)pow(2,profundidade+1)-1;
}


///Escreve arquivo de Log
void escreveArquivo(segundos seg){
    ofstream out;
    out.open(arq_resultados,ofstream::out | ofstream::app);
    //Nome qtNos - Capacidade - Custo - podasDFS - podasGrauR - podasUpper - podasLower
    out << n << ";" << Q << ";" << solucao << ";" << seg.count() << ";" << podas_por_dfs << ";"
    << podas_por_grau_minimo << ";" << podas_por_upper_bound << ";" << podas_por_lower_bound << endl;
    out.close();
}

// Imprime a matriz de arestas habilitadas na tela e no arquivo de respostas
void imprime_matriz(int M[][N]) {

	// modificada para mostrar na tela e salvar em arquivo tb...
	ofstream out;
	out.open(arq_respostas,ofstream::out | ofstream::app);

	printf("\n\n Solução de custo %d encontrada:\n\n", solucao);
	out << "Solução de custo " << solucao << " encontrada: " << endl;
	out << endl;

	// mostra na tela e taca no arquivo a matriz de arestas habilitadas
	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= n; j++){
			printf("%d ", M[i][j]);
			out << M[i][j] << " ";
		}
		printf("\n");
		out << endl;
	}
	printf("\n");
	out << endl;

	// mostra na tela e taca no arquivo AS ARESTAS habilitadas w(u,v, peso)
	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= n; j++){
			if (M[i][j]) {
			   printf("(%d, %d, %d) ", i,j, custos[i][j]);
			   out << "("<< i << "," << j <<  ","<<  custos[i][j] <<  ");";
			}
		}
	}
	printf("\n\n\n");
	out << endl << endl << endl;

	out.close();

}

bool prox_aresta_a_remover(int M[][N], int &novo_i, int &novo_j) {
	for (int j = novo_j+1; j <= n; j++) {
		if (M[novo_i][j]) {
			novo_j = j;
			return true;
		}
	}
	for (int i = novo_i+1; i <= n; i++) {
		for (int j = 0; j <= n; j++) {
			if (M[i][j]) {
				novo_i = i;
				novo_j = j;
				return true;
			}
		}
	}
	return false;
}

int custo_da_arvore(int M[][N], vector<vector<int>> custos) {
	int total = 0;
	for (int i = 0; i <= n; i++)
		for (int j = i+1; j <= n; j++)
			if (M[i][j])
				total += custos[i][j];
	return total;
}

int contaAresta(int M[][N]) {
	int count = 0;
	for (int i = 0; i <= n; i++)
		for (int j = i+1; j <= n; j++)
			if (M[i][j])
				count++;
	return count;
}

int dfs_visit_demanda(int M[][N], int u, bool visitado[N], int contador) {
	visitado[u] = true;
	for (int v = 0; v <= n; v++)
		if (M[u][v] && !visitado[v]){
			contador = dfs_visit_demanda(M, v, visitado, contador);
		}
	return contador + 1;
}

bool valida_demanda(int M[][N]) {
	bool visitado[n+1];
	for (int i = 0; i < n; i++)
		visitado[i] = false;

	visitado[n] = true;

	int contador = 0;
	for (int i = 0; i < n; i++){
		if ((M[n][i]) && (visitado[i]==false)){
			 contador = dfs_visit_demanda(M, i, visitado, 0);
			 if(contador >= Q){
				return false;
			}
		}
	}
	return true;
}

bool grau_minimo(int M[][N]) {
	int count = 0;
	for (int i = 0; i < n; i++){
		if (M[i][n])
			count++;
	}
	if(count >= ceil((float)n/(float)Q))
		return true;
	else
		return false;
}

void dfs_visit(int M[][N], int u, bool visitado[N]) {
	visitado[u] = true;
	for (int v = 0; v <= n; v++)
		if (M[u][v] && !visitado[v])
			dfs_visit(M, v, visitado);
}

int eh_conectado(int M[][N], int u) {
	bool visitado[N];
	for (int v = 0; v <= n; v++)
		visitado[v] = false;

	dfs_visit(M, u, visitado);

	for (int v = 0; v <= n; v++)
		if (!visitado[v])
			return false;
	return true;
}

void cmst_prim(int M[][N], int profundidade, int i, int j, Grafo grafo, int tipoprim) {

	t1 = Clock::now(); ///Marca Fim de Tempo
	tempo = std::chrono::duration_cast<std::chrono::seconds>(t1 - t0);

	if(tempo>refTime){ ///Já rodou mais de uma hora
        	escreveArquivo(tempo);
        	printf("Rodou mais de uma hora... abortando\n");
       	 	exit(1);
	}

        //verifica se a recursão chegou na folha
	if (profundidade == (((n+1)*n)/2)+1) {
	    return;
	}

	///Incrementa a quantidade de nos da árvores de reultados (do B&B) visitados
	cont++;

	// poda de conectado
	if (!eh_conectado(M, 0)) {
	   podas_por_dfs = podas_por_dfs + qtdNosArvore(profundidade) - 1;
	   return;
	}

    // corte do galho em que R possui grau menor que (n/Q) arestas
    if(!grau_minimo(M)){
        podas_por_grau_minimo = podas_por_grau_minimo + qtdNosArvore(profundidade) - 1;
        return;
    }

    int valorPrim;
    if(tipoprim==0)
        valorPrim = primVetor(grafo, M,arvorePrim);
    else if(tipoprim==1)
        valorPrim = primAVL(grafo,M,arvorePrim);

	if(valorPrim>=solucao){
		podas_por_lower_bound = podas_por_lower_bound + qtdNosArvore(profundidade) - 1;
		return;
	}

    ///Se válido, nenhum filho tem custo menor.Portanto, pode retornar.
	if(valida_demanda(M)){
		solucao = valorPrim;
			escreveArquivo(tempo);
			printf("\n\n=> Novo Upperbound encontrado: %d\n", solucao);
			imprime_matriz(arvorePrim);
			printf("\n\n\n");
			for(int i=0;i<=n;i++){
				for(int j=0;j<=n;j++){
					melhorArvorePrim[i][j] = arvorePrim[i][j];
				}
		 	}
	     	podas_por_upper_bound = podas_por_upper_bound + qtdNosArvore(profundidade) - 1;
	     	return;	
	 }

    int novo_i, novo_j;

    // remove_uma_aresta(M);
    novo_i = i;
    novo_j = j;
    if (!prox_aresta_a_remover(M, novo_i, novo_j)) {
        return;
    }

    M[i][j] = M[j][i] = 0;

    // sem a aresta
    cmst_prim(M, profundidade+1, novo_i, novo_j, grafo,tipoprim);

    // recupera_aresta(M);
    M[i][j] = M[j][i] = 1;

    // com a aresta
    cmst_prim(M, profundidade+1, novo_i, novo_j, grafo,tipoprim);
}

void limpaVariaveis(){
    podas_por_dfs = 0;
    cont = 0;
    podas_por_upper_bound = 0;
    podas_por_grau_minimo = 0;
    podas_por_lower_bound = 0;
}




////////////////////////////////// KRUSKAL ///////////////////////////////////

void cmst(int id_aresta, int num_arestas_na_raiz, int num_arestas_no_grafo);

struct aresta_kruskal {
	int u, v, w;
	aresta_kruskal(int a, int b, int c) {
		u = a, v = b; w = c;
	}
	
	bool operator<(const aresta_kruskal &o) const {
		return w < o.w;
	}
	
	friend ostream& operator<<(ostream& os, const aresta_kruskal& a) {
		os << '(' << a.u << ',' << a.v << ',' << a.w << ')';
		return os;
	}
};

int raiz, minimo_de_arestas_ligadas_a_raiz;

// grafo como lista de arestas:
vector<aresta_kruskal> E, arestas_da_raiz, arestas_do_resto;

// arestas que estao, nesse momento, válidas no grafo E
vector<int> e_validas;

void print_grafo_atual() {
	printf("grafo: {");
	for (int i = 0; i < E.size(); i++)
		if (e_validas[i])
			cout << E[i] << ", ";
	cout << "}" << endl;
}

void print_arvore(vector<aresta_kruskal> &arv) {
	printf("arvore: {");
	for (int i = 0; i < arv.size(); i++)
		cout << arv[i] << ", ";
	cout << endl;
}

int find(int u, vector<int> &grupo) {
	while (grupo[u] != u) {
		u = grupo[u];
	}
	return u;
}

void unionn(int u, int v, vector<int> &altura, vector<int> &grupo, vector<int> &num_filhos) {
	int grupo_u = find(u, grupo), grupo_v = find(v, grupo);
	if (altura[grupo_u] == altura[grupo_v]) {
		altura[grupo_u]++;
	}
	if (altura[grupo_u] >= altura[grupo_v]) {
		num_filhos[grupo_v] += num_filhos[grupo_u];
		grupo[grupo_v] = grupo_u;
	}
	else {
		num_filhos[grupo_u] += num_filhos[grupo_v];
		grupo[grupo_u] = grupo_v;
	}
}

vector<aresta_kruskal> kruskal(int &custo_mst) {
	vector<aresta_kruskal> mst;
	vector<int> altura(n+1), grupo(n+1), num_filhos(n+1);
	
	for (int i = 0; i <= n; i++) {
		altura[i] = 0;
		grupo[i] = i;
		num_filhos[i] = 1;
	}
	
	custo_mst = 0;
	for (int i = 0; i < E.size(); i++) {
		if (e_validas[i]) {
			mst.push_back(E[i]);
		}
	}
	sort(mst.begin(), mst.end());
	
	for (int i = 0; i < mst.size(); i++) {
		aresta_kruskal a = mst[i];
		int u = a.u, v = a.v, w = a.w;
		if (find(u, grupo) != find(v, grupo)) {
			unionn(u, v, altura, grupo, num_filhos);
			custo_mst += w;
		}
	}
	
	return mst;
}

int custo_total_do_grafo() {
	int total = 0;
	for (int i = 0; i < E.size(); i++) {
		if (e_validas[i]) {
			total += E[i].w;
		}
	}
	return total;
}

bool visitado[N+1];
vector<int> adj[N];

int dfs_visit(int u) {
	visitado[u] = true;
	int nessa_subarvore = 0;
	for (int i = 0; i < adj[u].size(); i++) {
		int v = adj[u][i];
		if (!visitado[v]) {
			nessa_subarvore += dfs_visit(v);
		}
	}
	return nessa_subarvore + 1;
}

bool dfs_count() {
	for (int i = 0; i <= n; i++) {
			visitado[i] = false;		
	}
	visitado[n] = true;

	for (int i = 0; i <= adj[n].size(); i++) {
		int v = adj[n][i];
		if (dfs_visit(v) > Q) {
			return false;
		}
	}
	
	for (int i = 0; i <= n; i++) {
		if (!visitado[i]){
			//printf("não é árvore\n");
			return false; // pois não é árvore
		}
	}	
	return true;
}

bool eh_uma_cmst(vector<aresta_kruskal> &mst) {
	for (int i = 0; i <= n; i++) {
		adj[i].clear();
	}
	
	for (int i = 0; i < mst.size(); i++) {
		adj[mst[i].u].push_back(mst[i].v);
		adj[mst[i].v].push_back(mst[i].u);
	}
	//printf("dfs_count\n");
	return dfs_count();
}

void merge_arestas() {
	E.clear();
	for (int i = 0; i < minimo_de_arestas_ligadas_a_raiz; i++) {
		E.push_back(arestas_da_raiz[i]);
	}
	
	int j = 0, i = minimo_de_arestas_ligadas_a_raiz;
	while (i != arestas_da_raiz.size() && j != arestas_do_resto.size()) {
		if (arestas_da_raiz[i] < arestas_do_resto[j]) {
			E.push_back(arestas_da_raiz[i]);
			i++;
		}
		else {
			E.push_back(arestas_do_resto[j]);
			j++;
		}
	}
	
	while (i != arestas_da_raiz.size()) {
		E.push_back(arestas_da_raiz[i]);
		i++;
	}
	
	while (j != arestas_do_resto.size()) {
		E.push_back(arestas_do_resto[j]);
		j++;
	}
}

void cmst() {
	// teto de n/Q é o número mínimo de arestas ligadas ao vértice raiz
	minimo_de_arestas_ligadas_a_raiz = n/Q + (n%Q != 0);
	
	// prepara a ordem das arestas pro kruskal como relaxacao otimizada:
	merge_arestas();
	
	// raiz é definida como o n-ésimo vértice
	raiz = n;
	
	// de início, todas as arestas do grafo são válidas
	e_validas = vector<int>(E.size(), 1);
	
	//print_grafo_atual();
	
	cmst(E.size()-1, n, E.size());
}


void init() {
	// somente 1 ordenacao para executar todos os kruskals
	// também facilita na hora de remover a maior aresta a cada nivel de profundidade na execução
	sort(E.begin(), E.end());
	sort(arestas_da_raiz.begin(), arestas_da_raiz.end());
	sort(arestas_do_resto.begin(), arestas_do_resto.end());
}


void input(vector<vector<int>> custos) {
	arestas_do_resto.clear();
	int k;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j <= n; j++) {
			k = custos[i][j];
			// o grafo sempre possuirá arestas do menor vértice pro maior
			if (i < j && j != n) {
				arestas_do_resto.push_back(aresta_kruskal(i,j,k));
			}
		}
	}
	arestas_da_raiz.clear();
	for (int j = 0; j < n; j++) {
		k = custos[n][j];
		arestas_da_raiz.push_back(aresta_kruskal(j,n,k));
	}
}

void atualiza_matriz(vector<aresta_kruskal> mst) {
	for (int i = 0; i <= n; i++)
		for (int j = 0; j <= n; j++)
			M[i][j] = 0;
	
	for (int i = 0; i < mst.size(); i++) {
		int u = mst[i].u, v = mst[i].v;
		M[u][v] = M[v][u] = 1;
	}	
}

void cmst(int id_aresta, int num_arestas_na_raiz, int num_arestas_no_grafo) {
	t1 = Clock::now(); ///Marca Fim de Tempo
	tempo = std::chrono::duration_cast<std::chrono::seconds>(t1 - t0);

	if(tempo>refTime){ ///Já rodou mais de uma hora
        	escreveArquivo(tempo);
        	printf("Rodou mais de uma hora... abortando\n");
       	 	exit(1);
	}
      
	// chegou na folha da árvore de execução:
	if (id_aresta < 0) {
		return;
	}
	
	// corte num grafo que possui o numero minimo_de_arestas_ligadas_a_raiz
	if (num_arestas_na_raiz < minimo_de_arestas_ligadas_a_raiz) {
		return;
	}
	
	// corte num grafo com menos arestas que a arvore:
	if (num_arestas_no_grafo < n) {
		return;
	}
	
	int custo_mst;
	vector<aresta_kruskal> mst = kruskal(custo_mst);

	// poda a partir desse nó de execução usando a relaxação:
	if (custo_mst > solucao) {
		return;
	}
	
	// se a mst já for uma cmst, então corta todas as folhas 
	// abaixo, pois elas não poderão melhorar esse prim

	
	if (eh_uma_cmst(mst)) {
		if (solucao > custo_mst) {
			solucao = custo_mst;			
			escreveArquivo(tempo);
			printf("\n\n=> Novo Upperbound encontrado: %d\n", solucao);
			atualiza_matriz(mst);
			imprime_matriz(M);
			printf("\n\n\n");		
		}
		return;
	}
	
	e_validas[id_aresta] = 0;
	if (E[id_aresta].v == raiz)
		num_arestas_na_raiz--;
		
	cmst(id_aresta-1, num_arestas_na_raiz, num_arestas_no_grafo-1);
	
	e_validas[id_aresta] = 1;
	if (E[id_aresta].v == raiz)
		num_arestas_na_raiz++;
	
	cmst(id_aresta-1, num_arestas_na_raiz, num_arestas_no_grafo);
}

void cmst_kruskal(vector<vector<int>> custos) {
	input(custos);
	init();
	cmst();
}


////////////////////////////////// FIM KRUSKAL /////////////////////////////////


int main(int argc,char ** argv){

  if(argc<4){
   cout << "./exec [tamanho do Q] [algoritmo CMST] [tipo upperbound 0/1 (1=Essàu)] [instância]" << endl;
   exit(0);
  }

  Grafo grafo(argv[4]);

  custos = grafo.matrizAdj;

  n = grafo.qtdNos;

  for (int i = 0; i <= n; i++)
     for (int j = 0; j <= n; j++)
			M[i][j] = i!=j;

      int algoritmo = (int)(argv[2][0] - '0');

      Q = VetorQ[(unsigned)(argv[1][0] - '0')];
      printf("Arquivo %s e capacidade %d\n",argv[4],Q);
	

	// zerando strings
	memset(&arq_resultados[0], 0, sizeof(arq_resultados));
	memset(&arq_respostas[0], 0, sizeof(arq_respostas));
	memset(&nome_solucao[0], 0, sizeof(nome_solucao));
	memset(&nome_instancia[0], 0, sizeof(nome_instancia));

	// tirando o barra
	strcat(nome_instancia, argv[4]);
	char * pch;
	pch = strstr (nome_instancia,"/");
  	strncpy (pch,"_",1);

	//// tipo de upperbound
	memset(&tipo_UBond[0], 0, sizeof(tipo_UBond));
	if ((unsigned)(argv[3][0] - '0') == 1) {	
		strcat(tipo_UBond, "UB_ESSAU");
	} else {
		strcat(tipo_UBond, "UB_PEDESTRE");
		};


	if (algoritmo == 0) {
		// é prim n²
		strcat(nome_solucao,"prim_n2");
	} else {
		if (algoritmo == 1) {
		// é prim AVL
		strcat(nome_solucao,"prim_AVL");
		}else {
			if (algoritmo == 2) {
			// é Kruskal adaptado
			strcat(nome_solucao,"kruskal");
			}
		}
	}


	// criando o nome do arquivo de resultados
	strcat(arq_resultados, "z_result_");
	strcat(arq_resultados, nome_instancia); 
	strcat(arq_resultados, "_Q");

	memset(&str_temp[0], 0, sizeof(str_temp));
	snprintf(str_temp, sizeof(str_temp), "%d", VetorQ[(unsigned)(argv[1][0] - '0')]);
	strcat(arq_resultados, str_temp);  		strcat(arq_resultados, "_");

	strcat(arq_resultados, nome_solucao);		strcat(arq_resultados, "_");
	strcat(arq_resultados, tipo_UBond);		strcat(arq_resultados, "_.txt");



	// criando o nome do arquivo de respostas
	strcat(arq_respostas, "z_resp_");
	strcat(arq_respostas, nome_instancia); 
	strcat(arq_respostas, "_Q");

	memset(&str_temp[0], 0, sizeof(str_temp));
	snprintf(str_temp, sizeof(str_temp), "%d", VetorQ[(unsigned)(argv[1][0] - '0')]);
	strcat(arq_respostas, str_temp);  		strcat(arq_respostas, "_");

	strcat(arq_respostas, nome_solucao);		strcat(arq_respostas, "_");
	strcat(arq_respostas, tipo_UBond);		strcat(arq_respostas, "_.txt");

	
	//// trecho de uso do algoritmo de Essaú que o Igor implementou ou Todos ligados à raiz////
	if ((unsigned)(argv[3][0] - '0') == 1) {	
		solucao = 0;	
		char buffer[100];
		sprintf(buffer,"java -jar upperbound.jar %d %s %s" ,Q , argv[4], arq_respostas);
		printf("\nBuffer de UpperBound: %s\n", buffer);
		system(buffer);
		//usleep(5000);
		ifstream in(arq_respostas);
		if(!in){
		        exit(EXIT_FAILURE);
		}
		in >> solucao;
		in.close();
	} else {
		///Gerando uma solucao inicial basica com todos ligados á raiz
		solucao = 0;

		for(int i=0;i<n;i++)
			solucao += custos[n][i];
		};
	cout << "Limite Superior do B&B em uso: " << solucao << endl;

      printf("\n\n Começando a rodar... \n\n");

      limpaVariaveis();
      t0 = Clock::now(); ///Marca Início de Tempo
      if (algoritmo == 0) {
        // é prim n²
        cmst_prim(M, 0, 0, 1, grafo, 0);
      } else {
        if (algoritmo == 1) {
        // é prim AVL
        cmst_prim(M, 0, 0, 1, grafo, 1);
        }else {
          if (algoritmo == 2) {
          // é Kruskal
          cmst_kruskal(custos);
          }
	}
      }
      t1 = Clock::now();
      imprime_matriz(M);
      tempo = std::chrono::duration_cast<std::chrono::seconds>(t1 - t0);
      escreveArquivo(tempo);

      long unsigned total_de_possibilidades = pow(2,((n+1)*(n))/2);
		cout << "Total de Possibilidades: " << pow(2,((n+1)*(n))/2) << endl;
		cout << "Qntd de nos na Arvore de Possibilidade: " << qtdNosArvore(0) << endl;
		cout << "Podas por DFS: " << podas_por_dfs << endl;
		cout << "Podas por UpperBound: " << podas_por_upper_bound << endl;
		cout << "Podas por LowerBound: " << podas_por_lower_bound << endl;
		cout << "Podas por grau minimo de R: " << podas_por_grau_minimo << endl;
		cout << "Numero de nos que foram analisados: " << cont << " ou seja:" << (cont/(float)qtdNosArvore(0))*100 << "% do total" << endl;


	// salvando estatisticas tb:
	ofstream out;
	out.open(arq_resultados,ofstream::out | ofstream::app);
	out << endl << endl;

	out << "Total de Possibilidades: " << total_de_possibilidades << endl;
	out << "Qntd de nos na Arvore de Possibilidade: " << qtdNosArvore(0) << endl;
	out << "Podas por DFS: " <<  podas_por_dfs << endl;
	out << "Podas por UpperBound: " <<   podas_por_upper_bound << endl;
	out << "Podas por LowerBound: " <<   podas_por_lower_bound << endl;
	out << "Podas por grau minimo de R: " <<   podas_por_grau_minimo << endl;
	out << "Numero de nos que foram analisados: " <<   cont << ", ou seja, " <<   (cont/(float)qtdNosArvore(0))*100 << " do total." <<endl;

	out.close();

}





