#include "../include/redirect.h"

#include <fcntl.h>
#include <unistd.h>

static int open_redirect_target(Redirect *redir) {
    if (redir == NULL) {
        return -1;
    }

    if (redir->target == NULL || redir->target[0] == '\0') {
        return -1;
    }

    switch (redir->type) {
    case REDIR_INPUT:
        redir->fd = open(redir->target, O_RDONLY);
        break;

    case REDIR_OUTPUT:
        redir->fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        break;

    case REDIR_APPEND:
        redir->fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
        break;

    case REDIR_HEREDOC:
        return -1;

    default:
        return -1;
    }

    return redir->fd;
}

static int save_discriptor(int fd) {
    if (fd < 0) {
        return -1;
    }

    int saved_fd = dup(fd);

    return saved_fd;
}
