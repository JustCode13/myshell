#include "../include/redirect.h"

#include <fcntl.h>
#include <unistd.h>

static int open_redirect_target(const Redirect *redir) {
    if (redir == NULL) {
        return -1;
    }

    if (redir->target == NULL || redir->target[0] == '\0') {
        return -1;
    }

    switch (redir->type) {
    case REDIR_INPUT:
        return open(redir->target, O_RDONLY);
        break;

    case REDIR_OUTPUT:
        return open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        break;

    case REDIR_APPEND:
        return open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
        break;

    case REDIR_HEREDOC:
        return -1;

    default:
        return -1;
    }

    return redir->fd;
}

static int save_descriptor(int fd) {
    if (fd < 0) {
        return -1;
    }

    int saved_fd = dup(fd);

    return saved_fd;
}

int redirect_open_file(const Redirect *redir) {
    if (redir == NULL) {
        return -1;
    }

    int fd = open_redirect_target(redir);

    return fd;
}

int redirect_apply(Redirect *list) {
    if (list == NULL) {
        return 0;
    }

    Redirect *current = list;

    while (current != NULL) {

        int saved_fd = save_descriptor(current->fd);

        if (saved_fd == -1) {
            close(saved_fd);

            return -1;
        }

        int opened_fd = redirect_open_file(current);

        if (opened_fd == -1) {
            close(saved_fd);

            return -1;
        }

        if (dup2(opened_fd, current->fd) < 0) {
            close(opened_fd);
            close(saved_fd);

            return -1;
        }

        if (close(opened_fd) < 0) {
            return -1;
        }

        current = current->next;
    }

    return 0;
}
