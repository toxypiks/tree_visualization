#include <stdio.h>
#include <stdlib.h>

#include "tree_queue.h"
#include "tree.h"

// singly-linked list queue

QueueNode* create_queue_node(Node* data)
{
    QueueNode *new_queue_node = malloc(sizeof(QueueNode));
    if (new_queue_node == NULL) {
        return NULL;
    }
    new_queue_node->next = NULL;
    new_queue_node->data = data;
    return new_queue_node;
}

TreeQueue* create_queue()
{
    TreeQueue *new_queue = malloc(sizeof(TreeQueue));
    if (new_queue == NULL) {
        return NULL;
    }
    new_queue->front = NULL;
    new_queue->back = NULL;
    return new_queue;
}

bool is_queue_empty(TreeQueue *queue)
{
    return queue->front == NULL|| queue->back == NULL ? true : false;
}

Result queue_destroy(TreeQueue *queue)
{
    if (is_queue_empty(queue)) {
        return ERR_INVAL;
    }
    while (queue->front != NULL) {
        QueueNode *tmp = queue->front;
        queue->front = tmp->next;
        free(tmp);
    }
    free(queue);
    return SUCCESS;
}

Node* queue_dequeue(TreeQueue *queue)
{
    if (queue == NULL || queue->front == NULL) {
        return NULL;
    }
    QueueNode *tmp = queue->front;
    queue->front = tmp->next;
    if (queue->front == NULL) {
        queue->back = NULL;
    }
    Node* tmp_node = tmp->data;
    free(tmp);
    return tmp_node;
}

int queue_enqueue(TreeQueue *queue, Node* data)
{
    if(queue == NULL) {
        return ERR_INVAL;
    }
    QueueNode *node = create_queue_node(data);
    if (queue->back == NULL) {
        queue->front = queue->back = node;
    } else {
        queue->back->next = node;
        queue->back = node;
    }
    return SUCCESS;
}

void print_queue(TreeQueue *queue)
{
    if (queue == NULL) {
        return;
    }
    QueueNode *tmp = queue->front;
    while (tmp != NULL) {
        // TODO use getter or print_tree_node()
        printf("%d\n", tmp->data->data);
        tmp = tmp->next;
    }
}
