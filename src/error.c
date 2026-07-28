#include "../include/error.h"
#include <string.h>

static ErrorCode current_error; // stores the current error code

static char error_buffer[256]; // stores the current error message

void set_error(ErrorCode code, const char *message) {
    if (message == NULL) {
        error_buffer[0] = '\0';
        return;
    }

    current_error = code;

    strcpy(error_buffer, message);

    return;
}

ErrorCode error_code(void) { return current_error; }

const char *error_message(void) { return error_buffer; }
