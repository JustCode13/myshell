#ifndef REDIRECT_H
#define REDIRECT_H

#include "shell_types.h"

int redirect_apply(Redirect *list);

int redirect_restore(void);

int redirect_open_file(const Redirect *redir);

void redirect_close_saved(void);

#endif // !REDIRECT_H
