// Implementação em C++ do algoritmo de Hopcroft–Karp para emparelhamento em grafos bipartidos.
#include <vector>
#include <queue>
#include <utility>
#include <map>
#include <stdio.h>
#include <iostream>

#define MAX_BOLTS 500
#define MAX_NUTS 500

using namespace std;

typedef vector<int> vi;
typedef pair<int, int> ii;

const int oo = 0x3F3F3F3F;
const int NIL = 0;

// Uma struct para representar o grafo bipartido.
// Assumimos que os vértices de 1 até n são da bipartição L.
// Já os vértices de n + 1 até n + m são da bipartição R.
// É comum que problemas utilizem vértices de 1 até n para uma bipartição e de 1 até m para outra. Cuidado.
struct Bipartite {
    // n é o número de vértices da bipartição L.
    // m é o número de vértices da bipartição R.
    // v é o número total de vértices no grafo.
    int v, n, m;

    // Lista de adjacências para a bipartição L.
    // Nada impede de estender para a bipartição R, mas não foi preciso em nenhum momento.
    vector<vi> graph;

    // Sejam u um vértice da bipartição L e v um vértice da bipartição R.
    // match[u] para u no intervalo [1, n] é o vértice v emparelhado ao u.
    // match[v] para v no intervalo [n + 1, n + m] é o vértice u emparelhado ao v.
    vi match;

    // Construído na BFS e usado na DFS para adicionar o caminho aumentante maximal ao emparelhamento.
    vi dist;

    // A construção do grafo só requer o número de vértices de cada bipartição.
    Bipartite(int n, int m) : v(n + m), n(n), m(m) {
        graph.resize(n + 1);
    }

    // Suponha v no intervalo [1, n] e u no intervalo [1, m].
    // Este é o mais comum para entrada dos problemas.
    void addEdge(int u, int v) {
        graph[u].push_back(n + v);
    }

    // Métodos necessários para a implementação do Hopcroft–Karp.
    bool bfs();
    bool dfs(int);
    int hopkarp();
    vector<ii> matches();
};

// Precisamos encontrar um caminho aumentante a cada iteração.
// Fazemos isso através da busca em largura abaixo.
bool Bipartite::bfs() {
    queue<int> q;

    // Podemos pensar que é a primeira iteração da BFS, buscando todos os vértices u não-saturados da bipartição L.
    for (int u = 1; u <= n; u++) {
        // Se for um vértice não-saturado, adicione na fila.
        if (match[u] == NIL) {
            dist[u] = 0; // Significa que o vértice u não está emparelhado.
            q.push(u);
        }

        // Definimos dist[u] como infinito para ele ser considerado nas iterações da BFS. Veja linhas mais abaixo.
        else {
            dist[u] = oo;
        }
    }

    // Definimos dist[NIL] como infinito pelo mesmo motivo acima.
    // Queremos um caminho que saia de NIL e volte para NIL alternando arestas. Isso só é possível se levarmos NIL
    // em consideração.
    dist[NIL] = oo;

    // Repare que a fila q só possui vértices da bipartição L.
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u != 0) {
            for (int i = 0; i < (int) graph[u].size(); i++) {
                int v = graph[u][i];

                // Se o match de v não foi considerado até agora, significa que a aresta (v, match[v]) ainda não foi
                // explorada.
                if (dist[match[v]] == oo) {
                    dist[match[v]] = dist[u] + 1; // Não levaremos mais match[v] em conta. Ele acabou de ser saturado.
                    q.push(match[v]);
                }
            }
        }
    }

    // Se conseguirmos voltar ao vértice NIL alternando arestas, então existe caminho aumentante.
    return (dist[0] != oo);
}

// Uma vez encontrado o caminho aumentante, precisamos adicioná-lo ao emparelhamento atual.
// Fazemos isso através da busca em profundidade abaixo.
bool Bipartite::dfs(int u) {
    // Conseguimos chegar ao vértice NIL começando em u_0 e alterando arestas.
    if (u == NIL) return true;

    for (int i = 0; i < (int) graph[u].size(); i++) {
        int v = graph[u][i]; // Vértice v da bipartição R.

        // Se v é o próximo elemento do caminho encontrado pela BFS.
        if (dist[match[v]] == dist[u] + 1) {
            // Se existir caminho alternante de v até NIL.
            if (dfs(match[v])) {
                match[v] = u;
                match[u] = v;
                return true;
            }
        }
    }

    // Não é possível ir de u_0 até NIL.
    dist[u] = oo;
    return false;
}

// Implementação do algoritmo de Hopcroft–Karp.
// Retorna o tamanho do emparelhamento máximo em um grafo bipartido.
int Bipartite::hopkarp() {
    match.resize(v+1, NIL);
    dist.resize(v+1, 0);
    int matching = 0;

    // Enquanto houver caminho aumentante.
    while (bfs()) {
        // Para cada vértice u da bipartição L.
        for (int u = 1; u <= n; u++) {
            // Se u é um vértice não-saturado, vejo se há caminho aumentante começando em u.
            if (match[u] == NIL && dfs(u)) {
                matching++;
            }
        }
    }

    return matching;
}

// Função que retorna um vetor de arestas (u, v) indicando que o vértice u da bipartição L
// é par do vértice v da bipartição R.
vector<ii> Bipartite::matches() {
    hopkarp();
    vector<ii> v;

    for (int u = 1; u <= n; u++) {
		if(u != 0 && match[u] != 0) v.push_back({u, match[u]-n});
    }

    return v;
}

int main(){
	
	int tCases;
	
	scanf("%d", &tCases);
	
	for(int i = 1; i <= tCases; ++i){
		int nBolts, nNuts;
		
		cin >> nBolts >> nNuts;
		
		int fits;
		Bipartite partition(MAX_BOLTS, MAX_NUTS);
		for(int b = 1; b <= nBolts; b++){
			for(int n = 1; n <= nNuts; n++){
				cin >> fits;
				if(fits)
					partition.addEdge(b,n);
			}
		}
		
		cout << "Case " << i << ": a maximum of " << partition.hopkarp() << " nuts and bolts can be fitted together\n";
	}
}