#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

struct node *push(struct node *top, int data);
int pop(struct node **top);
void print_stack(struct node *top);

int main() {
    struct node *head = NULL;
    int input_data;
    int scan_result;

    printf("Enter positive integers to push (Non-integer input to print stack):\n");

    while ((scan_result = scanf("%d", &input_data)) == 1) {
        if (input_data > 0) {
            head = push(head, input_data);
        } else {
            printf("Non-positive number (%d) entered. Only positive integers are pushed.\n", input_data);
        }
    }

    if (scan_result != 1) {
        printf("\nNon-integer input detected. Printing stack...\n");
        print_stack(head);
    }
    
    printf("\nPopping elements...\n");
    while (head != NULL) {
        int popped = pop(&head);
        printf("Popped: %d\n", popped);
    }
    
    return 0;
}

struct node *push(struct node *top, int data) {
    struct node *new_node;
    new_node = (struct node *)malloc(sizeof(struct node));

    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        return top;
    }

    new_node->data = data;
    new_node->next = top;
    return new_node;
}

int pop(struct node **top) { 
    if (*top == NULL) {
        printf("Stack is empty.\n");
        return -1;
    }

    struct node *temp;
    int data_out;
    
    temp = *top; 
    data_out = temp->data;
    *top = (*top)->next;
    free(temp);

    return data_out;
}

void print_stack(struct node *top) {
    struct node *current = top;
    printf("--- Stack Content (Top to Bottom) ---\n");
    while (current != NULL) {
        printf("%d\n", current->data);
        current = current->next;
    }
    printf("-----------------------------------\n");
}
