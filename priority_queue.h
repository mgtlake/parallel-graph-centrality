typedef struct {
    int node;
    int priority;
} Node;

typedef struct {
    Node* nodes;
    int len;
    int size;
} PQ;

PQ* initPQ(int* nodes, int nodeCount);
void push(int node, int priority);
int pop();
void decrease(int node, int priority);
