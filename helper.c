#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

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

/* Print a matrix with tab-seperation. 
 *  N.B. Very large matrices will not fit on screen.
 * Parameters:
 *          matrix - the matrix being printed
 *          rows - the number of rows
 *          cols - the number of columns
 */
void print_int_matrix(int** matrix, int rows, int cols) {
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
    while (node != NULL) {
        printf("centre : %s @ %i\n", graph->nodeLabels[node->value], node->value);

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

/* Get the indices of all elements in an array with the largest value.
 * Parameters:
 *          arr - the array being examined
 *          size - the size of the array
 * Return:
 *          Linked list of indices sharing max value. 
 *          If unique max value, still return a list of length 1.
 */
ListNode* max_ids(int* arr, int size) {
    int champ = 0;
    ListNode* head = malloc(sizeof(ListNode));
    // Go backwards so that list is in appearance order
    for (int i = size - 1; i >= 0; i--) {
        if (arr[i] > champ) {
            champ = arr[i];
            head->value = i;
            if (head->next != NULL) free_list(head->next);
        } else if (arr[i] == champ) {
            ListNode* next = malloc(sizeof(ListNode));
            next->value = i;
            next->next = head;
            head = next;
        }
    }
    return head;
}

