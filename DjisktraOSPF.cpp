#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

class OSPF {
private:
    int vertices;
    std::vector<std::vector<int>> graph;
    std::vector<int> parent;

public:
    OSPF(int vertices) : vertices(vertices) {
        graph.resize(vertices, std::vector<int>(vertices, 0));
        parent.resize(vertices, -1);
    }

    void addEdge(int source, int destination, int weight) {
        graph[source][destination] = weight;
        graph[destination][source] = weight;
    }

    void dijkstra(int source) {
        std::vector<int> dist(vertices, std::numeric_limits<int>::max());
        std::vector<bool> sptSet(vertices, false);

        for (int i = 0; i < vertices; i++) {
            dist[i] = (i == source) ? 0 : std::numeric_limits<int>::max();
            parent[i] = -1;
            sptSet[i] = false;
        }

        for (int count = 0; count < vertices - 1; count++) {
            int u = minDistance(dist, sptSet);
            sptSet[u] = true;

            for (int v = 0; v < vertices; v++) {
                if (!sptSet[v] && graph[u][v] != 0 && dist[u] != std::numeric_limits<int>::max()
                    && dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                    parent[v] = u;
                }
            }
        }

        printSolution(dist, source);
    }

    int getShortestDistance(int destination) {
        return parent[destination] == -1 ? std::numeric_limits<int>::max() : parent[destination];
    }

    void printPath(int destination) {
        if (destination == -1)
            return;

        printPath(parent[destination]);
        std::cout << destination << " ";
    }

private:
    int minDistance(const std::vector<int>& dist, const std::vector<bool>& sptSet) {
        int min = std::numeric_limits<int>::max();
        int minIndex = -1;

        for (int v = 0; v < vertices; v++) {
            if (!sptSet[v] && dist[v] <= min) {
                min = dist[v];
                minIndex = v;
            }
        }

        return minIndex;
    }

    void printSolution(const std::vector<int>& dist, int source) {
        std::cout << "Vertex\tDistance from Source\tPath" << std::endl;
        for (int i = 0; i < vertices; i++) {
            std::cout << i << "\t" << dist[i] << "\t\t\t";
            printPath(i);
            std::cout << std::endl;
        }
    }
};

int main() {
    int vertices, edges, sourceVertex, destinationVertex;
    std::cout << "Enter the number of vertices: ";
    std::cin >> vertices;

    OSPF graph(vertices);

    std::cout << "Enter the number of edges: ";
    std::cin >> edges;

    for (int i = 0; i < edges; i++) {
        int source, destination, weight;
        std::cout << "Enter source, destination, and weight for edge " << (i + 1) << ": ";
        std::cin >> source >> destination >> weight;
        graph.addEdge(source, destination, weight);
    }

    std::cout << "Enter the source vertex: ";
    std::cin >> sourceVertex;

    std::cout << "Enter the destination vertex: ";
    std::cin >> destinationVertex;

    graph.dijkstra(sourceVertex);

    int shortestDistance = graph.getShortestDistance(destinationVertex);

    if (shortestDistance == std::numeric_limits<int>::max()) {
        std::cout << "There is no path from source to destination." << std::endl;
    } else {
        std::cout << "Shortest path: ";
        graph.printPath(destinationVertex);
        std::cout << std::endl;
    }

    return 0;
}