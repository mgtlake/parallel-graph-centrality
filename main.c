#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "helper.h"
#include "io.h"


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
            dist[i][j] = graph->connections[i][j];
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
    print_int_matrix(dist, graph->nodeCount, graph->nodeCount);
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
    // Iterate backwards so that list is in appearance order
    for (int i = graph->nodeCount - 1; i >= 0; i--) {
        int sum = 0;
        for (int j = 0; j < graph->nodeCount; j++) {
            sum += dist[i][j];
        } 
        printf("%i: %i\n", i, sum);
        if (sum < champ) {
            champ = sum;
            centres->value = i;
            centres->length = 1;
            if (centres->next != NULL) {
                free_list(centres->next);
                centres->next = NULL;
            }
        } else if (sum == champ) {
            ListNode* next = malloc(sizeof(ListNode));
            next->value = i;
            next->next = centres;
            next->length = centres->length + 1;
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
    // Iterate backwards so that list is in appearance order
    for (int i = graph->nodeCount - 1; i >= 0; i--) {
        int maxDistance = max_val(dist[i], graph->nodeCount);
        printf("%i: %i\n", i, maxDistance);
        if (maxDistance < champ) {
            champ = maxDistance;
            centres->value = i;
            centres->length = 1;
            if (centres->next != NULL) {
                free_list(centres->next);
                centres->next = NULL;
            }
        } else if (maxDistance == champ) {
            ListNode* next = malloc(sizeof(ListNode));
            next->value = i;
            next->next = centres;
            next->length = centres->length + 1;
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
    
    ListNode* minTotalCentres = min_total_centres(graph);
    print_centres(minTotalCentres, graph);
    char* minTotalFilename = malloc((strlen(argv[1]) + 20) * sizeof(char));
    sprintf(minTotalFilename, "%s-min_total", argv[1]);
    write_file(minTotalFilename, minTotalCentres, graph);
    //print_centres(min_max_centres(graph), graph);
    
    ListNode* minMaxCentres = min_max_centres(graph);
    print_centres(minMaxCentres, graph);
    char* minMaxFilename = malloc((strlen(argv[1]) + 20) * sizeof(char));
    sprintf(minMaxFilename, "%s-min_max", argv[1]);
    write_file(minMaxFilename, minMaxCentres, graph);

    //print_int_matrix(graph->connections, graph->nodeCount, graph->nodeCount);

    return 0;
}

