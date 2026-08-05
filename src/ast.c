#include "../include/ast.h"

#include <stdio.h>
#include <stdlib.h>

static void destroy_redirects(Redirect *head) {
    if (head == NULL) {
        return;
    }

    Redirect *current = head;

    while (current != NULL) {
        Redirect *next = current->next;

        free(current->target);
        free(current);

        current = next;
    }
}
