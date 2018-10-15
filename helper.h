/* =============================================================================
 * helper.h     Set of helper functions, mostly printing and 
 *              array and list operations, as well as core structs
 * =============================================================================
 */

/* Store a graph with weighted edges and labelled nodes.
*/
typedef struct {
    int nodeCount;
    int edgeCount;
    char** nodeLabels;
    int** connections;
} Graph;

/* A linked-list node.
 */
typedef struct listNode {
    int value;
    struct listNode* next;
    int length;
} ListNode;

/* Print node labels for a graph.
 * Parameters: 
 *          graph - the graph in question
 */
void print_labels(Graph* graph);

/* Print a graph.
 * Parameters:
 *          graph - the graph in question
 */
void print_graph(Graph* graph);

/* Print a graph in DOT format for easy visualisation.
 * Parameters:
 *          graph - the graph in question
 */ 
void print_graph_DOT(Graph* graph);

/* Print a matrix with tab-seperation. 
 *  N.B. Very large matrices will not fit on screen.
 * Parameters:
 *          matrix - the matrix being printed
 *          rows - the number of rows
 *          cols - the number of columns
 */
void print_matrix(int** matrix, int rows, int cols);

/* Print a list of centres.
 * Parameters:
 *          head - the head of the list of centres
 *          graph - the graph in question
 */
void print_centres(ListNode* head, Graph* graph);

/* Free all elements of linked-list, including head.
 * Parameters:
 *          head - the head of the list to be freed
 */
void free_list(ListNode* head);

/* Get the maximum value in an array.
 * Parameters:
 *          arr - the array being examined
 *          size - the size of the array
 * Return:
 *          The largest value.
 */
int max_val(int* arr, int size);



void set_partition(int rank, int size, int dataSize, int* start, int* end);

int get_partition(int i, int size, int dataSize);
