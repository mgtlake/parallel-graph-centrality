/* =============================================================================
 * ass1.c     Main file, contains entry point and core calculation functions
 * =============================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "helper.h"
#include "io.h"

/* Populate matrix of shortest paths between all pairs of nodes.
 *  Use Floyd-Warshall algorithm for O(|V|^3) time with simple implementation.
 * Parameters:
 *          graph - the graph in question
 * Return:
 *          A matrix containing the length of the shortest paths
 *          between all nodes.
 */
int** all_pair_shortest_path(Graph* graph) {
    // Use full matrix instead of half for simpler access
    // RAM use isn't the bottleneck in any case
    int** dist = malloc(graph->nodeCount * sizeof(int*));
    for (int i = 0; i < graph->nodeCount; i++) {
        dist[i] = malloc(graph->nodeCount * sizeof(int));
        for (int j = 0; j < graph->nodeCount; j++) {
            dist[i][j] = graph->connections[i][j];
        }
    }

    // Loop over each intermediate node
    // runtime: |V| * (|V| * |V| + |V|)/2
    for (int k = 0; k < graph->nodeCount; k++) {
        // Loop over each pair of nodes
        for (int i = 0; i < graph->nodeCount; i++) {
            // Extract loop invariant access
            int distIK = dist[i][k];
            // Skip lower half and diagonal since matrix is symetrical since 
            // graph is undirected, and since diagonal = 0 always
            // Don't skip diagonal any more b/c it's needed for detecting 
            // negative cycles
            for (int j = i; j < graph->nodeCount; j++) {
                // If path through intermediate node exists and is shorter than 
                // direct path or if direct path doesn't exist, use path 
                // through intermediate node
                // Check if path exists and short-circuit otherwise
                if (distIK != INT_MAX && dist[k][j] != INT_MAX 
                        && dist[i][j] > distIK + dist[k][j]) {
                    if (i == j && distIK + dist[k][j] < 0) {
                        printf("Negative cycle detected, min path undefined\n");
                        exit(1); 
                    }
                    dist[i][j] = distIK + dist[k][j];
                }
            }
        }
    }

    // Copy top half into lower half - this is valid because an undirected graph
    // will always have a symetrical distance matrix (path(u, v) == path(v, u))
    // runtime: (|V| * |V| - |V|)/2
    for (int i = 0; i < graph->nodeCount; i++) {
        for (int j = 0; j < i; j++) {
           dist[i][j] = dist[j][i]; 
        }
    }

    // total runtime: (|V|^3)/2 + |V|^2 - |V|/2 => O(|V|^3)

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
ListNode* min_total_centres(int** dist, Graph* graph) {
    int champ = INT_MAX;
    ListNode* centres = malloc(sizeof(ListNode));
    centres->next = NULL;
    // Iterate backwards so that list is in appearance order
    for (int i = graph->nodeCount - 1; i >= 0; i--) {
        int sum = 0;
        // Sum over row of pairwise distance matrix to get total distance
        for (int j = 0; j < graph->nodeCount; j++) {
            sum += dist[i][j];
        } 
        if (sum < champ) {
            champ = sum;
            centres->value = i;
            centres->length = 1;
            // Clear linked list
            if (centres->next != NULL) {
                free_list(centres->next);
                centres->next = NULL;
            }
        } else if (sum == champ) {
            // Append at head of linked list
            ListNode* next = malloc(sizeof(ListNode));
            next->value = i;
            next->next = centres;
            next->length = centres->length + 1;
            centres = next;
        }
    }
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
ListNode* min_max_centres(int** dist, Graph* graph) {
    int champ = INT_MAX;
    ListNode* centres = malloc(sizeof(ListNode));
    centres->next = NULL;
    // Iterate backwards so that list is in appearance order
    for (int i = graph->nodeCount - 1; i >= 0; i--) {
        // Take max of row of pairwise distance matrix to get largest distance
        int maxDistance = max_val(dist[i], graph->nodeCount);
        if (maxDistance < champ) {
            champ = maxDistance;
            centres->value = i;
            centres->length = 1;
            // Clear linked list
            if (centres->next != NULL) {
                free_list(centres->next);
                centres->next = NULL;
            }
        } else if (maxDistance == champ) {
            // Append at head of linked list
            ListNode* next = malloc(sizeof(ListNode));
            next->value = i;
            next->next = centres;
            next->length = centres->length + 1;
            centres = next;
        }
    }
    return centres; 
}

int main(int argc, char** argv) {
    Graph* graph = read_file(argv[1]);

    int** shortestPathsMatrix = all_pair_shortest_path(graph);
    
    // Find centres by minimum total distance
    ListNode* minTotalCentres = min_total_centres(shortestPathsMatrix, graph);
    char* minTotalFilename = malloc((strlen(argv[1]) + 20) * sizeof(char));
    sprintf(minTotalFilename, "%s-min_total", argv[1]);
    write_file(minTotalFilename, minTotalCentres, graph);
    
    // Find centres by minimum maximum distance
    ListNode* minMaxCentres = min_max_centres(shortestPathsMatrix, graph);
    char* minMaxFilename = malloc((strlen(argv[1]) + 20) * sizeof(char));
    sprintf(minMaxFilename, "%s-min_max", argv[1]);
    write_file(minMaxFilename, minMaxCentres, graph);

    return 0;
}

