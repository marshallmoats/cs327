#include <stdlib.h>

#include "queue.h"

int queue_init(queue_t* q) {
    q->front = NULL;
    q->back = NULL;
    q->length = 0;
}
int queue_destroy(queue_t *q) {
    queue_node_t *tmp;
    while ((tmp = q->front)) {
        q->front = q->front->next;
        free(tmp);
    }
    q->back = NULL;
    q->length = 0;
    return 0;
}
int queue_enqueue(queue_t *q, int i) {
    queue_node_t *n;
    if (!(n = malloc(sizeof(*n)))) {
        return 1;
    }

    n->data = i;
    n->next = NULL;

    q->length++;

    if (q->back) {
        q->back->next = n;
    } else {
        q->front = n;
    }
    q->back = n;
}
int queue_dequeue(queue_t *q, int *i) {
    queue_node_t *tmp;
    if (!q->length) {
        return 1;
    }
    *i = q->front->data;
    tmp = q->front;
    q->front = q->front->next;
    free(tmp);
    if (!--(q->length)) {
        q->back = NULL;
    }
    return 0;
}
int queue_front(queue_t *q, int *i) {
    if (!q->length) {
        return 1;
    }
    *i = q->front->data;
    return 0;
}
int queue_length(queue_t *q) {
    return q->length;
}
int queue_is_empty(queue_t *q) {
    return !q->length;
}



int main