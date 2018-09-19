/* =============================================================================
 * helper.c     Set of helper functions, mostly printing and 
 *              array and list operations
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "helper.h"

/* Print node labels for a graph.
 * Parameters: 
 *          graph - the graph in question
 */
void print_labels(Graph* graph) {
    for (int i = 0; i < graph->nodeCount; i++) {
        printf("%i : %s ", i, graph->nodeLabels[i]);
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

/* Print a graph in DOT format for easy visualisation.
 * Parameters:
 *          graph - the graph in question
 */ 
void print_graph_DOT(Graph* graph) {
    printf("digraph graphname {\n");
    for (int i = 0; i < graph->nodeCount; i++) {
        printf("\t%s\n", graph->nodeLabels[i]);
        for (int j = 0; j < graph->nodeCount; j++) {
            if (graph->connections[i][j] != 0 
                    && graph->connections[i][j] != INT_MAX) {
                printf("\t\"%s\" -> \"%s\" [label=%i]\n", graph->nodeLabels[i],
                        graph->nodeLabels[j], graph->connections[i][j]);
            }
        }
    }
    printf("}\n");
}

/* Print a matrix with tab-seperation. 
 *  N.B. Very large matrices will not fit on screen.
 * Parameters:
 *          matrix - the matrix being printed
 *          rows - the number of rows
 *          cols - the number of columns
 */
void print_matrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%i\t", matrix[i][j]);
        }
        printf("\n");
    }
}

/* Print a list of centres.
 * Parameters:
 *          head - the head of the list of centres
 *          graph - the graph in question
 */
void print_centres(ListNode* head, Graph* graph) {
    ListNode* node = head;
    printf("Number of centres : %i\n", head->length);
    while (node != NULL) {
        int centreID = node->value;
        printf("centre : %s @ %i\n", graph->nodeLabels[centreID], centreID);

        node = node->next;
    }
}

/* Free all elements of linked-list, including head.
 * Parameters:
 *          head - the head of the list to be freed
 */
void free_list(ListNode* head) {
    ListNode* node = head;
    while (node != NULL) {
        ListNode* tmp = node;
        node = node->next;
        free(tmp);
    }
}

/* Get the maximum value in an array.
 * Parameters:
 *          arr - the array being examined
 *          size - the size of the array
 * Return:
 *          The largest value.
 */
int max_val(int* arr, int size) {
    int champ = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] > champ) champ = arr[i];
    }
    return champ;
}

