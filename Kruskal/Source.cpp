#include "Graph.h"

void kruskal(Graph &graph) {
	std::list<Edge> edgeList;

	Graph MST(graph.noVert);

	for (int it = 0; it < graph.noVert - 1; ++it) {
		for (intPair pair : graph.adjList[it]) {
			if (it < pair.first) {
				edgeList.push_back(std::make_tuple(it, pair.first, pair.second));
			}
		}
	}

	edgeList.sort(edgeSort);

	while (!edgeList.empty()) {
		Edge minEdge = edgeList.front();
		edgeList.pop_front();

		if (!formCycle(MST, minEdge)) {
			MST.addEdge(std::get<0>(minEdge), std::get<1>(minEdge), std::get<2>(minEdge));
		}
	}

	outputEdges(MST);
}

int main() {
	std::ifstream inFile("input.txt");

	int noVert;
	int startVert;
	int endVert;
	int weight;

	inFile >> noVert;

	Graph graph(noVert);

	while (true) {
		inFile >> startVert >> endVert >> weight;
		graph.addEdge(startVert, endVert, weight);
		if (inFile.eof()) break;
	}
	inFile.close();

	kruskal(graph);

	return 0;
}