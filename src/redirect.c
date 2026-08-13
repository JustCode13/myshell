#include "../include/redirect.h"

#include <fcntl.h>
#include <unistd.h>

static int saved_stdin = -1;
static int saved_stdout = -1;

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

    case REDIR_OUTPUT:
        return open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    case REDIR_APPEND:
        return open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);

    case REDIR_HEREDOC:
        return -1;

    default:
        return -1;
    }
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

        if (current->type == REDIR_INPUT) {
            if (saved_stdin < 0) {
                saved_stdin = save_descriptor(STDIN_FILENO);

                if (saved_stdin == -1) {

                    return -1;
                }
            }

        } else {
            if (saved_stdout < 0) {
                saved_stdout = save_descriptor(STDOUT_FILENO);

                if (saved_stdout == -1) {

                    return -1;
                }
            }
        }

        int opened_fd = redirect_open_file(current);

        if (opened_fd == -1) {
            if (current->type == REDIR_INPUT) {
                close(saved_stdin);
                saved_stdin = -1;
            } else {
                close(saved_stdout);
                saved_stdout = -1;
            }

            return -1;
        }

        if (dup2(opened_fd, current->fd) < 0) {
            close(opened_fd);
            if (current->type == REDIR_INPUT) {
                close(saved_stdin);
                saved_stdin = -1;
            } else {
                close(saved_stdout);
                saved_stdout = -1;
            }

            return -1;
        }

        if (close(opened_fd) < 0) {
            return -1;
        }

        current = current->next;
    }

    return 0;
}

int redirect_restore(void) {
    if (saved_stdin >= 0) {
        if (dup2(saved_stdin, STDIN_FILENO) < 0) {
            return -1;
        }
    }

    if (saved_stdout >= 0) {
        if (dup2(saved_stdout, STDOUT_FILENO) < 0) {
            return -1;
        }
    }

    if (close(saved_stdin) < 0) {
        return -1;
    }

    saved_stdin = -1;

    if (close(saved_stdout) < 0) {
        return -1;
    }

    saved_stdout = -1;

    return 0;
}

void redirect_close_saved(void) {
    if (saved_stdin >= 0) {
        if (close(saved_stdin) < 0) {
            return;
        }

        saved_stdin = -1;
    }

    if (saved_stdout >= 0) {
        if (close(saved_stdout) < 0) {
            return;
        }

        saved_stdout = -1;
    }

    return;
}
