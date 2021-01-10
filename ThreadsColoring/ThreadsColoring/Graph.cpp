#include "Graph.h"
#include <random>
#include <ctime>
Graph::Graph(int V) {
	this->V = V;
	init();
}
void Graph::init() {
	srand((int)time(0));
	for (int i = 0; i < V; i++)
		for (int j = 0; j < V; j++)
			if (i == j) matrix[i][j] = 0;
			else if (i < j) {
				matrix[i][j] = rand() % 2;
				matrix[j][i] = matrix[i][j];
			}
}
