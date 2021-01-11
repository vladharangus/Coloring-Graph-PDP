// ThreadsColoring.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Graph.h"
#include <thread>
#include <mutex>

using namespace std;

mutex m;
Graph graph = NULL;
int V = 5;
int numberOfColors = 3;
int colors[10];
thread threads[10];
void exec(int color[], int c);

void printGraph() {
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
            cout << graph.matrix[i][j] << " ";
        cout << "\n";
    }
}
bool isSafe(int vertex, int c, int color[]) {
    for (int i = 0; i < V; i++)
        if (graph.matrix[vertex][i] && c == color[i])
            return false;
    return true;
}

bool backtracking(int pos, int color[]) {
    if (pos == V)
        return true;

    for (int c = 1; c <= numberOfColors; c++) {
        if (pos == 0) {
            int color1[5];
            threads[c] = thread(exec, color1, c);
        }
        else {
            if (isSafe(pos, c, color)) {
                color[pos] = c;
                if (backtracking(pos + 1, color))
                    return true;
                color[pos] = 0;
            }
        }
    }
    return false;
}
void exec(int color[], int c) {
    color[0] = c;
    
    if (backtracking(1, color))
    {
      
        m.lock();
        for (int i = 0; i < V; i++) {
            
            
            colors[i] = color[i];
        }
        m.unlock();
       
    }
    
}

int main()
{
    graph = Graph(V);
    cout << "Graph:\n";
    printGraph();
    int color[5];
    for (int i = 0; i < V; i++)
        color[i] = 0;
    
    backtracking(0, color);
    for (int i = 1; i <= numberOfColors; i++) {
        threads[i].join();
    }
    for (int i = 0; i < V; i++) {
        cout << colors[i] << " ";
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
