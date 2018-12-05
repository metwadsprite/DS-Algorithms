#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <queue>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
////////////////////////// Class definitions begin /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Edge {
public:
	int dest;
	int flow;
	int capacity;

	Edge(int dest, int cap) {
		this->dest = dest;
		this->flow = 0;
		this->capacity = cap;
	}
};

class Graph {
public:
	int noVert;
	std::vector<std::list<Edge>> *adjList;

	Graph(int noVert) {
		this->noVert = noVert;
		this->adjList = new std::vector<std::list<Edge>>(noVert);
	}
	void addEdge(int start, int end, int cap) {
		this->adjList->at(start).push_back(Edge(end, cap));
	}
};

class Network : public Graph {
public:
	int source;
	int sink;

	Network(int noVert, int source, int sink) : Graph(noVert) {
		this->source = source;
		this->sink = sink;
	}
};

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Class definitions end //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool isPathBFS(Network &G, int start, int stop, std::vector<int> &path) {
	std::queue<int> Q;
	std::vector<bool> visited(G.noVert, false);

	Q.push(start);
	visited[start] = true;

	path[start] = -1;

	while (!Q.empty()) {
		start = Q.front();
		Q.pop();

		for (auto edge : G.adjList->at(start)) {
			if (!visited[edge.dest] && edge.capacity - edge.flow > 0) {
				Q.push(edge.dest);
				path[edge.dest] = start;
				visited[edge.dest] = true;
			}
		}
	}

	return (visited[stop]);
}

int FordFulkerson(Network &G) {
	std::vector<int> path(G.noVert);
	int maxFlow = 0;
	int actVert;
	int preVert;
	
	while (isPathBFS(G, G.source, G.sink, path)) {
		int pathFlow = 9999;

		for (actVert = G.sink; actVert != G.source; actVert = path[actVert]) {
			preVert = path[actVert];

			for (auto &edge : G.adjList->at(preVert)) {
				if (edge.dest == actVert) {
					pathFlow = std::min(pathFlow, edge.capacity - edge.flow);
				}
			}
		}

		for (actVert = G.sink; actVert != G.source; actVert = path[actVert]) {
			preVert = path[actVert];

			for (auto &edge : G.adjList->at(preVert)) {
				if (edge.dest == actVert) {
					edge.flow += pathFlow;
				}
			}
		}

		maxFlow += pathFlow;
	}

	return maxFlow;
}

int main() {
	std::ifstream file("input.txt");
	int noVert;
	int source, sink;
	int start, end, cap;

	file >> noVert >> source >> sink;
	Network NT(noVert, source, sink);

	while (file >> start) {
		file >> end >> cap;
		NT.addEdge(start, end, cap);
	}
	file.close();

	std::cout << FordFulkerson(NT) << "\n";

	return 0;
}