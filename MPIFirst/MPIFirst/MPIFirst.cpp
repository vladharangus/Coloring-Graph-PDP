// MPIFirst.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "mpi.h"
#include "Graph.h"

using namespace std;
#define V 10
#define numberOfColors 2

Graph graph = NULL;
int color[V];
int colorSlave[V];
int ok1;
int ok2;
void printGraph() {
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
            cout << graph.matrix[i][j] << " ";
        cout << "\n";
    }
}
bool isSafe(int vertex, int c) {
    for (int i = 0; i < V; i++)
        if (graph.matrix[vertex][i] && c == colorSlave[i])
            return false;
    return true;
}
bool backtracking(int pos) {
    if (pos == V)
        return true;

    for (int c = 1; c <= numberOfColors; c++) {
        if (isSafe(pos, c)) {
            colorSlave[pos] = c;
            if (backtracking(pos + 1))
                return true;
            colorSlave[pos] = 0;
        }
    }
    return false;
}
int main(int argc, char **argv)
{
   
    int commsize;
    int rank = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Status status;
    if (rank == 0) {
        MPI_Send(&color, V, MPI_INT, 1, 7, MPI_COMM_WORLD);
        MPI_Recv(&ok1, 1, MPI_INT, 1, 10, MPI_COMM_WORLD, &status);
        MPI_Recv(&color, V, MPI_INT, 1, 11, MPI_COMM_WORLD, &status);
        if (ok1 == 1) {
            cout << "We have solution!" << "\n";
            for (int i = 0; i < V; i++)
                cout << color[i] << " ";
        }
        else cout << "No solution :(";
    }
    if (rank == 1) {
        graph = Graph(V);
        cout << "Graph:\n";
        printGraph();
        MPI_Recv(&colorSlave, V, MPI_INT, 0, 7, MPI_COMM_WORLD, &status);
        if (backtracking(0)) ok2 = 1;
        MPI_Send(&ok2, 1, MPI_INT, 0, 10, MPI_COMM_WORLD);
        MPI_Send(&colorSlave, V, MPI_INT, 0, 11, MPI_COMM_WORLD);
        
    }

    MPI_Finalize();
    return 0;
}