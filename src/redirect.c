#include "../include/redirect.h"

#include <fcntl.h>
#include <unistd.h>

static int open_redirect_target(Redirect *redir) {
    if (redir == NULL) {
        return -1;
    }
}
