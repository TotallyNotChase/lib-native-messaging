#include "nativemsg.h"
#include "errmacros.h"

// Reads the message sent by the browser extension from stdin
uint8_t* nativemsg_read_message(uint32_t* length)
{
    size_t count;
    int err;
    count = fread(length, sizeof(uint32_t), 1, stdin);
    // Make sure correct number of bytes were read, if not handle errors and return NULL
    COUNT_CHECK_STDIN(count, 1, err, NULL);
    uint8_t* buffer = malloc(*length * sizeof(*buffer));
    if (buffer == NULL)
    {
        fprintf(stderr, "An error occured while allocating memory for buffer");
        return NULL;
    }
    count = fread(buffer, sizeof(*buffer), *length, stdin);
    // Make sure correct number of bytes were read, if not handle errors and return NULL
    COUNT_CHECK_STDIN(count, *length, err, buffer);
    return buffer;
}

// Writes the given message to be read by the browser extension in stdout
size_t nativemsg_write_message(const uint8_t* const buffer, uint32_t length)
{
    size_t count;
    int err;
    if (length > (1024 * 1024)) {
        fprintf(stderr, "Message too large");
        return 0;
    }
    count = fwrite(&length, sizeof(uint32_t), 1, stdout);
    COUNT_CHECK_STDOUT(count, 1, err);
    count = fwrite(buffer, sizeof(char), length, stdout);
    COUNT_CHECK_STDOUT(count, length, err);
    fflush(stdout);
    return length + 4;
}
