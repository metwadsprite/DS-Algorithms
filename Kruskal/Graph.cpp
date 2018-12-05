#include "Graph.h"

Graph::Graph(int noVert) {
	this->noVert = noVert;
	this->adjList = new std::list<intPair>[noVert];
}
Graph::~Graph() {
		delete[] this->adjList;
}

void Graph::addEdge(int startVert, int endVert, int weight) {
	this->adjList[startVert].push_back(std::make_pair(endVert, weight));
	this->adjList[endVert].push_back(std::make_pair(startVert, weight));
}

bool edgeSort(Edge edge1, Edge edge2) {
	return std::get<2>(edge1) < std::get<2>(edge2);
}

bool formCycle(Graph &graph, Edge edge) {
	std::vector<bool> visited(graph.noVert, false);
	std::list<int> queue;

	int start = std::get<0>(edge);
	int dest = std::get<1>(edge);

	visited[start] = true;
	queue.push_back(start);

	while (!queue.empty()) {
		start = queue.front();
		queue.pop_front();

		for (intPair pair : graph.adjList[start]) {
			if (pair.first == dest) {
				return true;
			}

			if (!visited[pair.first]) {
				visited[pair.first] = true;
				queue.push_back(pair.first);
			}
		}
	}

	return false;
}

void outputEdges(Graph &graph) {
	std::ofstream outFile("output.txt");

	for (int it = 0; it < graph.noVert - 1; ++it) {
		for (intPair pair : graph.adjList[it]) {
			if (it < pair.first) {
				outFile << it << "-" << pair.first << " weight: " << pair.second << "\n";
			}
		}
	}

	outFile.close();
}