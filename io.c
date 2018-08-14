#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "helper.h"
#include "io.h"

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

/* Write a file containing centres.
 * Parameters:
 *          filename - the path and name of the file to write to
 *          head - the head of the linked list of centres
 *          graph - the graph in question
 */
void write_file(char* filename, ListNode* head, Graph* graph) {
    FILE* file = fopen(filename, "w");
    fprintf(file, "%i\n", head->length);
    ListNode* node = head;
    while (node != NULL) {
        fprintf(file, "%s %i\n", graph->nodeLabels[node->value], node->value);
        node = node->next;
    }
    fclose(file);
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
            // Init to INT_MAX instead of 0 to represent no connection, since
            // 0 is a valid weight
            for (int j = 0; j < graph->nodeCount; j++) {
                if (i == j) {
                    graph->connections[i][j] = 0;
                } else {
                    graph->connections[i][j] = INT_MAX;
                }
            }
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

