#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <functional>

using namespace std;

#define INF 5000

typedef pair<int, int> intPair;

class Graph {
public:
	int nodes;
	list<intPair> *adj;

	Graph(int nodes) {
		this->nodes = nodes;
		this->adj = new list<intPair>[nodes];
	}

	void addEdge(int startNode, int endNode, int dist);
};

void Graph::addEdge(int startNode, int endNode, int dist) {
	adj[startNode].push_back(make_pair(endNode, dist));
	adj[endNode].push_back(make_pair(startNode, dist));
}

void Dijkstra(Graph graph, int source) {
	list<int> Q;
	vector<int> dist(graph.nodes);
	vector<int> prev(graph.nodes);
	list<int>::iterator qi;
	list<intPair>::iterator ni;

	for (int v = 0; v < graph.nodes; v++) {		// Initializare
		dist[v] = INF;							// Nu stim distanta de la sursa spre v
		prev[v] = INF;							// Nodul precedent in calea optima
		Q.push_back(v);							// In Q avem toate nodurile nevizitate
	}

	dist[source] = 0;							// Distanta de la sursa la sursa

	while (!Q.empty()) {
		int u = 0;
		int minDist = INF;

		for (qi = Q.begin(); qi != Q.end(); qi++) {		// Gasim nodul cu cea mai mica distanta
			if (dist[*qi] < minDist) {
				minDist = dist[*qi];
				u = *qi;
			}
		}

		for (qi = Q.begin(); qi != Q.end(); qi++) {		// Il stergem din lista de noduri nevizitate
			if (*qi == u) {
				Q.erase(qi);
				break;
			}
		}

		for (ni = graph.adj[u].begin(); ni != graph.adj[u].end(); ni++) {	// Fiecare vecin al nodului u din Q
			int alt = dist[u] + (*ni).second;
			if (alt < dist[(*ni).first]) {									// Am gasit o cale mai scurta spre v
				dist[(*ni).first] = alt;
				prev[(*ni).first] = alt;
			}
		}
	}

	for (int i = 0; i < graph.nodes; i++) {
		std::cout << i << "\t" << dist[i] << "\n";
	}
}

int main() {
	ifstream file("graph.txt");

	int nodes;
	int start;
	int end;
	int dist;

	file >> nodes;

	Graph graph(nodes);

	while (true) {
		file >> start >> end >> dist;
		if (file.eof()) break;
		graph.addEdge(start, end, dist);
	}

	Dijkstra(graph, 0);

	return 0;
}