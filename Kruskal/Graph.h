#pragma once

#ifndef Graph_H
#define Graph_H

#include <list>
#include <tuple>
#include <vector>
#include <fstream>

typedef std::pair<int, int> intPair;
typedef std::tuple<int, int, int> Edge;

class Graph {
public:
	int noVert;
	std::list<intPair> *adjList;

	Graph(int noVert);
	~Graph();

	void addEdge(int startVert, int endVert, int weight);
};

bool edgeSort(Edge edge1, Edge edge2);
bool formCycle(Graph &graph, Edge edge);
void outputEdges(Graph &graph);

#endif