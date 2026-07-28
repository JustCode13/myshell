#ifndef ERROR_H
#define ERROR_H

typedef enum ErrorCode {
    ERR_NONE,
    ERR_MEMORY,
    ERR_PARSE,
    ERR_PROCESS,
    ERR_SIGNAL,
    ERR_IO,
} ErrorCode;

void error_set(ErrorCode code, const char *message);

void error_code(ErrorCode code);

const char *error_message(void);

void error_clean(void);

#endif // !ERROR_H
