#ifndef TREE_QUEUE_H_
#define TREE_QUEUE_H_

#include "tree.h"

typedef enum Result {
    SUCCESS,
    ERR_INVAL,
    ERR_NOMEM
} Result;

typedef struct QueueNode {
    Node* data;
    struct QueueNode *next;
} QueueNode;

typedef struct TreeQueue {
    QueueNode *front;
    QueueNode *back;
} TreeQueue;

QueueNode* create_queue_node(Node* data);
TreeQueue* create_queue();
bool is_queue_empty(TreeQueue *queue);
Result queue_destroy(TreeQueue *queue);
Node* queue_dequeue(TreeQueue *queue);
int queue_enqueue(TreeQueue *queue, Node* data);
void print_queue(TreeQueue *queue);

#endif // TREE_QUEUE_H_
