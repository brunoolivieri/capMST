
Descri��o das Inst�ncias

- Abaixo s�o descritos os arquivos de inst�ncias para o
problema da �rvore Geradora com Restri��es de Capacidade

Em todas as inst�ncias, todos os v�rtices t�m demanda igual a 1.

Conforme visto abaixo,  as capacidades das �rvores ligadas ao
n� raiz (o �ltimo nos arquivos, n+1) s�o todas iguais.

As inst�ncias correspondem a capacidade 3, 5 e 10.
Portanto ser�o no total 39 inst�ncias, visto que est�o em anexo
os arquivos te16.txt, tc40-x.txt  e  te40-x.txt para x=1,2,3,4 e 5,  
tc80-1.txt e te80-1.txt.

Neste zip,  as inst�ncias pequenas te16  s�o para testar a corretude do algoritmo.

Todos os arquivos de inst�ncias s�o .txt. 
O arquivo te16.crd cont�m as coordenadas dos v�rtices
no plano. Isto permite desenhar a inst�ncia para visualizar
a solu��o obtida.

Segue a descri��o original das inst�ncias.
------------------------------------------------------------------------

This file contains tc (central depot) and te data (end depot)
for the CMST with unit demands.

L. Gouveia, A comparison of directed formulations for the
capacitated minimal spanning tree problem, Telecommunication
Systems vol 1, 1993, 51-76.

Each vertex i (i=1,...n) has unit weight and each subtree connected to the 
depot N (N=n+1) in the final solution should have a total weight of less than 
or equal to Q.

For each problem type (tc,te), there are 10 problems each, 5 of size n=40
and 5 of size n=80.  Note that the depot is vertex number 41 and 81
respectively.

For size n=40 (N=41) : tc40-*.txt and te40-*.txt (*=1,...,5)
capacity Q can take three values: Q=3, 5, or 10.
Total number of problems = 2*5*3 = 30 (15 type tc and 15 type te).

For size n=80 (N=81) : tc80-*.txt and te80-*.txt (*=1,...,5)
capacity Q can take three values: Q=5, 10, or 20.
Total number of problems = 2*5*3 = 30 (15 type tc and 15 type te).

The format of the file is as follows:

problem name
problem size n
edge costs c(i,j), i=1,N, j=1,N

(Note c(i,i) is taken as a large number)


