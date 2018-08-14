#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "helper.h"


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

/* Populate matrix of shortest paths between all pairs of nodes.
 * Parameters:
 *          graph - the graph in question
 * Return:
 *          A matrix containing the length of the shortest paths
 *          between all nodes.
 */
int** all_pair_shortest_path(Graph* graph) {
    int** dist = malloc(graph->nodeCount * sizeof(int*));
    for (int i = 0; i < graph->nodeCount; i++) {
        dist[i] = malloc(graph->nodeCount * sizeof(int));
        for (int j = 0; j < graph->nodeCount; j++) {
            if (graph->connections[i][j] > 0 || i == j) {
                dist[i][j] = graph->connections[i][j];
            } else {
                dist[i][j] = INT_MAX;
            }
        }
    }
    for (int k = 0; k < graph->nodeCount; k++) {
        for (int i = 0; i < graph->nodeCount; i++) {
            for (int j = 0; j < graph->nodeCount; j++) {
                if (dist[i][j] > dist[i][k] + dist[k][j] 
                        && dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    //print_int_matrix(dist, graph->nodeCount, graph->nodeCount);
    return dist;
}

/* Find the centre of the graph defined by closeness.
 *  That is, find the node(s) such that the sum of shortest paths to every other
 *  node is minimised.
 * Parameters:
 *          graph - the graph in question
 * Return:
 *          The id of the centre node.
 */
ListNode* min_total_centres(Graph* graph) {
    int** dist = all_pair_shortest_path(graph);

    int champ = INT_MAX;
    ListNode* centres = malloc(sizeof(ListNode));
    centres->next = NULL;
    for (int i = 0; i < graph->nodeCount; i++) {
        int sum = 0;
        for (int j = 0; j < graph->nodeCount; j++) {
            sum += dist[i][j];
        } 
        printf("%i: %i\n", i, sum);
        if (sum < champ) {
            champ = sum;
            centres->value = i;
            if (centres->next != NULL) free_list(centres->next);
        } else if (sum == champ) {
            ListNode* next = malloc(sizeof(ListNode));
            next->value = i;
            next->next = centres;
            centres = next;
        }
    }
    printf("total distance : %i\n", champ);
    return centres; 
}

/* Find the centre of the graph defined by minimum maximum distance.
 *  That is, find the node(s) such that the longest distance to any other node 
 *  (along the shortest path) is minimised.
 * Parameters:
 *          graph - the graph in question
 * Return:
 *          The id of the centre node.
 */
ListNode* min_max_centres(Graph* graph) {
    int** dist = all_pair_shortest_path(graph);

    int champ = INT_MAX;
    ListNode* centres = malloc(sizeof(ListNode));
    centres->next = NULL;
    for (int i = 0; i < graph->nodeCount; i++) {
        int maxDistance = max_val(dist[i], graph->nodeCount);
        printf("%i: %i\n", i, maxDistance);
        if (maxDistance < champ) {
            champ = maxDistance;
            centres->value = i;
            if (centres->next != NULL) free_list(centres->next);
        } else if (maxDistance == champ) {
            ListNode* next = malloc(sizeof(ListNode));
            next->value = i;
            next->next = centres;
            centres = next;
        }
    }
    printf("min max distance : %i\n", champ);
    return centres; 
}

int main(int argc, char** argv) {
    Graph* graph = read_file(argv[1]);
    //print_labels(graph);
    //print_graph(graph);

    // For all nodes
    // - Sum shortest path to each other node
    // Choose node with shortest such sum
    // Duplication of work - need to eliminate redundant cals - e.g. both ways
    // Try Dijkstra first to get a sense of worst-case
    // Then Floyd-Warshall
    //
    print_centres(min_total_centres(graph), graph);
    print_centres(min_max_centres(graph), graph);

    //print_int_matrix(graph->connections, graph->nodeCount, graph->nodeCount);

    return 0;
}

