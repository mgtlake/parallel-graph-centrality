#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "priority_queue.h"

/* Store a graph with weighted edges and labelled nodes.
*/
typedef struct {
    int nodeCount;
    int edgeCount;
    char** nodeLabels;
    int** connections;
} Graph;

/* Print node labels for a graph.
 * Parameters: 
 *          graph - the graph in question
 */
void print_labels(Graph* graph) {
    for (int i = 0; i < graph->nodeCount; i++) {
        printf("%i : %s\n", i, graph->nodeLabels[i]);
    }
    printf("---\n");
}

/* Print a graph.
 * Parameters:
 *          graph - the graph in question
 */
void print_graph(Graph* graph) {
    for (int i = 0; i < graph->nodeCount; i++) {
        for (int j = 0; j < graph->nodeCount; j++) {
            if (graph->connections[i][j] != 0) {
                printf("%s <---> %s weight %i\n", graph->nodeLabels[i], 
                        graph->nodeLabels[j], graph->connections[i][j]);
            }
        } 
    }
}

void print_int_matrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%i\t", matrix[i][j]);
        }
        printf("\n");
    }
}

/* Parse and process a line read from the input file.
 * Parameters:
 *          lineBuffer - the line in question
 *          line - the line number, 1-indexed
 *          bufferPos - the length of the line
 *          graph - the graph being read in
 */
void process_line(char* lineBuffer, int line, int length, Graph* graph) {
    if (line == 1) {
        graph->nodeCount = atoi(lineBuffer);
        graph->nodeLabels = malloc(graph->nodeCount * sizeof(char*));
        // TODO we can cut this in half
        graph->connections = malloc(graph->nodeCount * sizeof(int*));
        for (int i = 0; i < graph->nodeCount; i++) {
            graph->connections[i] = malloc(graph->nodeCount * sizeof(int));
        }

    } else if (line == 2) {
        graph->edgeCount = atoi(lineBuffer);
    } else if (line <= 2 + graph->nodeCount) {
        int node_id = line - 2 - 1;
        graph->nodeLabels[node_id] = malloc(sizeof(char) * length + 1);
        strcpy(graph->nodeLabels[node_id], lineBuffer);

    } else {
        int from = atoi(strtok(lineBuffer, " "));
        int to = atoi(strtok(NULL, " "));
        int weight = atoi(strtok(NULL, " "));
        graph->connections[from][to] = weight;
        graph->connections[to][from] = weight;
    }
}

/* Read an input file describing a graph.
 * Parameters:
 *          filename - the path and name of the file
 * Return:
 *          A fully constructed graph.
 */
Graph* read_file(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File %s could not be opened.\n", filename);
        exit(1);
    }

    Graph* graph = malloc(sizeof(Graph));

    int bufferSize = sizeof(char) * 80;
    char* lineBuffer = malloc(sizeof(char) * bufferSize);
    int bufferPos = 0;
    char c = 0;
    int line = 0;

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            line++;

            process_line(lineBuffer, line, bufferPos, graph);

            bufferPos = 0;
            bufferSize = sizeof(char) * 80;
            memset(&lineBuffer[0], 0, bufferSize);
        } else {
            lineBuffer[bufferPos] = c;
            bufferPos++;
            if (bufferPos == bufferSize) {
                bufferSize *= 2;
                lineBuffer = realloc(lineBuffer, sizeof(char) * bufferSize);
            }
        }
    }

    free(lineBuffer);
    fclose(file);

    return graph;
}

/* Find the centre of the graph
 * Parameters:
 *          graph - the graph in question
 * Return:
 *          The id of the centre node.
 */
int closeness_centre(Graph* graph) {
    int** dist = malloc(graph->nodeCount * sizeof(int*));
    for (int i = 0; i < graph->nodeCount; i++) {
        dist[i] = malloc(graph->nodeCount * sizeof(int));
        for (int j = 0; j < graph->nodeCount; j++) {
            dist[i][j] = graph->connections[i][j];
        }
    }
    for (int k = 0; k < graph->nodeCount; k++) {
        for (int i = 0; i < graph->nodeCount; i++) {
            for (int j = 0; j < graph->nodeCount; j++) {
                if (dist[i][j] > dist[i][k] + dist[k][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
    print_int_matrix(dist, graph->nodeCount, graph->nodeCount);
    int centre = -1;
    int minWeight = INT_MAX;
    for (int i = 0; i < graph->nodeCount; i++) {
        int sum = 0;
        for (int j = 0; j < graph->nodeCount; j++) {
            sum += dist[i][j];
        } 
        if (sum < minWeight) {
            minWeight = sum;
            centre = i;
        }
    }
    return centre; 
}

int main(int argc, char** argv) {
    Graph* graph = read_file(argv[1]);
    print_labels(graph);
    print_graph(graph);

    // For all nodes
    // - Sum shortest path to each other node
    // Choose node with shortest such sum
    // Duplication of work - need to eliminate redundant cals - e.g. both ways
    // Try Dijkstra first to get a sense of worst-case
    // Then Floyd-Warshall
    //
    int centreId = closeness_centre(graph);
    printf("centre: %s @ %i\n", graph->nodeLabels[centreId], centreId);

    print_int_matrix(graph->connections, graph->nodeCount, graph->nodeCount);

    return 0;
}

