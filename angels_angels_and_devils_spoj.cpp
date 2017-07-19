// Implementa��o em C++ do algoritmo de Hopcroft�Karp para emparelhamento em grafos bipartidos.
#include <vector>
#include <queue>
#include <utility>
#include <stdio.h>

#define MAXCOLS 400
#define MAXROWS 400

using namespace std;

typedef vector<int> vi;
typedef pair<int, int> ii;

const int oo = 0x3F3F3F3F;
const int NIL = 0;

// Uma struct para representar o grafo bipartido.
// Assumimos que os v�rtices de 1 at� n s�o da biparti��o L.
// J� os v�rtices de n + 1 at� n + m s�o da biparti��o R.
// � comum que problemas utilizem v�rtices de 1 at� n para uma biparti��o e de 1 at� m para outra. Cuidado.
struct Bipartite {
    // n � o n�mero de v�rtices da biparti��o L.
    // m � o n�mero de v�rtices da biparti��o R.
    // v � o n�mero total de v�rtices no grafo.
    int v, n, m;

    // Lista de adjac�ncias para a biparti��o L.
    // Nada impede de estender para a biparti��o R, mas n�o foi preciso em nenhum momento.
    vector<vi> graph;

    // Sejam u um v�rtice da biparti��o L e v um v�rtice da biparti��o R.
    // match[u] para u no intervalo [1, n] � o v�rtice v emparelhado ao u.
    // match[v] para v no intervalo [n + 1, n + m] � o v�rtice u emparelhado ao v.
    vi match;

    // Constru�do na BFS e usado na DFS para adicionar o caminho aumentante maximal ao emparelhamento.
    vi dist;

    // A constru��o do grafo s� requer o n�mero de v�rtices de cada biparti��o.
    Bipartite(int n, int m) : v(n + m), n(n), m(m) {
        graph.resize(n + 1);
    }

    // Suponha v no intervalo [1, n] e u no intervalo [1, m].
    // Este � o mais comum para entrada dos problemas.
    void addEdge(int u, int v) {
        graph[u].push_back(n + v);
    }

    // M�todos necess�rios para a implementa��o do Hopcroft�Karp.
    bool bfs();
    bool dfs(int);
    int hopkarp();
    vector<ii> matches();
};

// Precisamos encontrar um caminho aumentante a cada itera��o.
// Fazemos isso atrav�s da busca em largura abaixo.
bool Bipartite::bfs() {
    queue<int> q;

    // Podemos pensar que � a primeira itera��o da BFS, buscando todos os v�rtices u n�o-saturados da biparti��o L.
    for (int u = 1; u <= n; u++) {
        // Se for um v�rtice n�o-saturado, adicione na fila.
        if (match[u] == NIL) {
            dist[u] = 0; // Significa que o v�rtice u n�o est� emparelhado.
            q.push(u);
        }

        // Definimos dist[u] como infinito para ele ser considerado nas itera��es da BFS. Veja linhas mais abaixo.
        else {
            dist[u] = oo;
        }
    }

    // Definimos dist[NIL] como infinito pelo mesmo motivo acima.
    // Queremos um caminho que saia de NIL e volte para NIL alternando arestas. Isso s� � poss�vel se levarmos NIL
    // em considera��o.
    dist[NIL] = oo;

    // Repare que a fila q s� possui v�rtices da biparti��o L.
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u != 0) {
            for (int i = 0; i < (int) graph[u].size(); i++) {
                int v = graph[u][i];

                // Se o match de v n�o foi considerado at� agora, significa que a aresta (v, match[v]) ainda n�o foi
                // explorada.
                if (dist[match[v]] == oo) {
                    dist[match[v]] = dist[u] + 1; // N�o levaremos mais match[v] em conta. Ele acabou de ser saturado.
                    q.push(match[v]);
                }
            }
        }
    }

    // Se conseguirmos voltar ao v�rtice NIL alternando arestas, ent�o existe caminho aumentante.
    return (dist[0] != oo);
}

// Uma vez encontrado o caminho aumentante, precisamos adicion�-lo ao emparelhamento atual.
// Fazemos isso atrav�s da busca em profundidade abaixo.
bool Bipartite::dfs(int u) {
    // Conseguimos chegar ao v�rtice NIL come�ando em u_0 e alterando arestas.
    if (u == NIL) return true;

    for (int i = 0; i < (int) graph[u].size(); i++) {
        int v = graph[u][i]; // V�rtice v da biparti��o R.

        // Se v � o pr�ximo elemento do caminho encontrado pela BFS.
        if (dist[match[v]] == dist[u] + 1) {
            // Se existir caminho alternante de v at� NIL.
            if (dfs(match[v])) {
                match[v] = u;
                match[u] = v;
                return true;
            }
        }
    }

    // N�o � poss�vel ir de u_0 at� NIL.
    dist[u] = oo;
    return false;
}

// Implementa��o do algoritmo de Hopcroft�Karp.
// Retorna o tamanho do emparelhamento m�ximo em um grafo bipartido.
int Bipartite::hopkarp() {
    match.resize(v+1, NIL);
    dist.resize(v+1, 0);
    int matching = 0;

    // Enquanto houver caminho aumentante.
    while (bfs()) {
        // Para cada v�rtice u da biparti��o L.
        for (int u = 1; u <= n; u++) {
            // Se u � um v�rtice n�o-saturado, vejo se h� caminho aumentante come�ando em u.
            if (match[u] == NIL && dfs(u)) {
                matching++;
            }
        }
    }

    return matching;
}

// Fun��o que retorna um vetor de arestas (u, v) indicando que o v�rtice u da biparti��o L
// � par do v�rtice v da biparti��o R.
vector<ii> Bipartite::matches() {
    hopkarp();
    vector<ii> v;

    for (int u = 1; u <= n; u++) {
		if(u != 0 && match[u] != 0) v.push_back({u, match[u]-n});
    }

    return v;
}

int indexRows(int nRows, int nCols, char m[MAXROWS][MAXCOLS], int rowsId[MAXROWS][MAXCOLS]){
	int nodes = 0;
	
	for(int i = 0; i < nRows; i++){
			for(int j = 0; j < nCols; j++){
				if(m[i][j] != 'A'){
					if(j == 0 || m[i][j-1] == 'A'){
						rowsId[i][j] = ++nodes;
					} else {
						rowsId[i][j] = rowsId[i][j-1];
					}
				}
			}
	}
	
	return nodes;
}

int indexCols(int nRows, int nCols, char m[MAXROWS][MAXCOLS], int colsId[MAXROWS][MAXCOLS]){
	int nodes = 0;
	
	for(int i = 0; i < nRows; i++){
			for(int j = 0; j < nCols; j++){
				if(m[i][j] != 'A'){
					if(i == 0 || m[i-1][j] == 'A'){
						colsId[i][j] = ++nodes;
					} else {
						colsId[i][j] = colsId[i-1][j];
					}
				}
			}
	}
	
	return nodes;
}

Bipartite buildBipartiteGraph(int nRows, int nCols, int rowsId[MAXROWS][MAXCOLS],int colsId[MAXROWS][MAXCOLS], int nRowsNodes, int nColsNodes, char m[MAXROWS][MAXCOLS]){
	Bipartite b(nRowsNodes, nColsNodes);
	
	for(int i = 0; i < nRows; i++){
		for(int j = 0; j < nCols; j++){
			if(rowsId[i][j] >= 1 && m[i][j] == 'H'){
				b.addEdge(rowsId[i][j], colsId[i][j]);
			}
		}
	}
	
	return b;
}

int main(){
	int tCases;
	char m[MAXROWS][MAXCOLS];
	int rowsId[MAXROWS][MAXCOLS];
	int colsId[MAXROWS][MAXCOLS];
	
	scanf("%d", &tCases);
	while(tCases--){
		int nRows, nCols;
		scanf("%d %d ", &nRows, &nCols);
		
		for(int i = 0; i < nRows; i++){
			for(int j = 0; j < nCols; j++){
				scanf("%c ", &m[i][j]);
			}
		}
		/*
		for(int i = 0; i < nRows; i++){
			for(int j = 0; j < nCols; j++){
				printf("%c ", m[i][j]);
			}
			puts("");
		}
		puts("");*/
		
		int nRowsNodes = indexRows(nRows, nCols, m, rowsId);
		int nColsNodes = indexCols(nRows, nCols, m, colsId);
		
		/*
		for(int i = 0; i < nRows; i++){
			for(int j = 0; j < nCols; j++){
				printf("%d ", rowsId[i][j]);
			}
			puts("");
		}
		
		puts("");
		for(int i = 0; i < nRows; i++){
			for(int j = 0; j < nCols; j++){
				printf("%d ", colsId[i][j]);
			}
			puts("");
		}*/
		
		
		Bipartite b = buildBipartiteGraph(nRows, nCols, rowsId, colsId, nRowsNodes, nColsNodes, m);
		
		
		
		/*vector<ii> pairs = b.matches();
		for(unsigned i = 0; i < pairs.size();++i){
			printf("%d %d\n", pairs[i].first, pairs[i].second);
		} 
		puts("");*/
		
		
		printf("%d\n", b.hopkarp());
	}
}