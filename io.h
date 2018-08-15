/* =============================================================================
 * io.c         Set of functions for reading from and writing to files, as well 
 *              as initialising the graph
 * =============================================================================
 */

/* Read an input file describing a graph and construct that graph.
 * Parameters:
 *          filename - the path and name of the file
 * Return:
 *          A fully constructed graph.
 */
Graph* read_file(char* filename);

/* Write a file containing centres.
 * Parameters:
 *          filename - the path and name of the file to write to
 *          head - the head of the linked list of centres
 *          graph - the graph in question
 */
void write_file(char* filename, ListNode* head, Graph* graph);

/* Parse and process a line read from the input file.
 * Parameters:
 *          lineBuffer - the line in question
 *          line - the line number, 1-indexed
 *          bufferPos - the length of the line
 *          graph - the graph being read in
 */
void process_line(char* lineBuffer, int line, int length, Graph* graph);
