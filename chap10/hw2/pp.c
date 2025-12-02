#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

void addq(struct node **head, struct node **tail, int data);
int delete(struct node **head, struct node **tail);
void print_queue(struct node *head);

int main() {
    struct node *head = NULL;
    struct node *tail = NULL;
    int input_data;
    int scan_result;

    while ((scan_result = scanf("%d", &input_data)) == 1) {
        if (input_data > 0) {
            addq(&head, &tail, input_data);
        }
    }

    print_queue(head);

    while (head != NULL) {
        delete(&head, &tail);
    }

    return 0;
}

void addq(struct node **head, struct node **tail, int data) {
    struct node *new_node;
    new_node = (struct node *)malloc(sizeof(struct node));

    if (new_node == NULL) {
        perror("addq malloc failed");
        return;
    }

    new_node->data = data;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
        *tail = new_node;
    } else {
        (*tail)->next = new_node;
        *tail = new_node;
    }
}

int delete(struct node **head, struct node **tail) {
    if (*head == NULL) {
        return -1;
    }

    struct node *temp;
    int data_out;

    temp = *head;
    data_out = temp->data;

    *head = (*head)->next;

    if (*head == NULL) {
        *tail = NULL;
    }
    
    free(temp);

    return data_out;
}

void print_queue(struct node *head) {
    struct node *current = head;
    printf("Print queue\n");
    while (current != NULL) {
        printf("%d\n", current->data);
        current = current->next;
    }
}

