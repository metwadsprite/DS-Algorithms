#include <fstream>
#include <list>
#include <vector>

using namespace std;

#define INF 5000

typedef pair<int, int> intPair;

class Graph {
public:
	int noVert;
	list<intPair> *adjList;

	Graph(int noVert) {
		this->noVert = noVert;
		this->adjList = new list<intPair>[noVert];
	}

	void addEdge(int startVert, int endVert, int weight);
};

void Graph::addEdge(int startVert, int endVert, int weight) {
	adjList[startVert].push_back(make_pair(endVert, weight));
	adjList[endVert].push_back(make_pair(startVert, weight));
}

void outputDist(Graph graph, vector<vector<int>> dist) {
	ofstream fileOut("output.txt");

	fileOut << "\t";
	for (int i = 0; i < graph.noVert; i++) {
		fileOut << i << "\t";
	}
	fileOut << "\n\n";

	for (int i = 0; i < graph.noVert; i++) {
		fileOut << i << "\t";

		for (int j = 0; j < graph.noVert; j++) {
			fileOut << dist[i][j] << "\t";
		}

		fileOut << "\n\n";
	}

	fileOut.close();
}

void royFloyd(Graph graph) {
	vector<vector<int>> dist(graph.noVert);
	for (int i = 0; i < graph.noVert; i++) {
		dist[i].resize(graph.noVert);
	}

	for (int i = 0; i < graph.noVert; i++) {
		for (int j = 0; j < graph.noVert; j++) {
			dist[i][j] = INF;
		}
	}

	for (int i = 0; i < graph.noVert; i++) {
		for (intPair pair : graph.adjList[i]) {
			dist[i][pair.first] = pair.second;
		}

		dist[i][i] = 0;
	}

	for (int k = 0; k < graph.noVert; k++) {
		for (int i = 0; i < graph.noVert; i++) {
			for (int j = 0; j < graph.noVert; j++) {
				if (dist[i][j] > dist[i][k] + dist[k][j]) {
					dist[i][j] = dist[i][k] + dist[k][j];
				}
			}
		}
	}

	outputDist(graph, dist);
}

int main() {
	ifstream fileIn("graph.txt");

	int noVert;
	int startVert;
	int endVert;
	int weight;

	fileIn >> noVert;

	Graph graph(noVert);

	while (true) {
		fileIn >> startVert >> endVert >> weight;
		graph.addEdge(startVert, endVert, weight);
		if (fileIn.eof()) break;
	}

	royFloyd(graph);

	fileIn.close();

	return 0;
}