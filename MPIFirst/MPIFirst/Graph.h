#pragma once
class Graph
{
public:
	int V;
	int matrix[4][4] = {
        { 0, 1, 1, 1 },
        { 1, 0, 1, 0 },
        { 1, 1, 0, 1 },
        { 1, 0, 1, 0 },
    };;
	Graph(int V);
	void init();
};

