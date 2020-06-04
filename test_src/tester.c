#include "nativemsg.h"
#include<string.h>

int main()
{
    uint32_t len;
    // Read the message
    uint8_t* msg = NULL;
    msg = nativemsg_read(&len);
    if (msg == NULL)
    {
        exit(EXIT_FAILURE);
    }
    // Assign a null terminator in the end to make it usable as a string
    msg[len] = '\0';
    if (!strcmp((char*)msg, "{\"text\":\"ping\"}"))
    {
        // Message was 'ping', wrappned inside a json
        // Valid
        if (nativemsg_write((uint8_t*)"{\"msg\":\"pong\"}", 14) != 18)
        {
            // Wrong number of bytes written - not (14 + 4)
            free(msg);
            exit(EXIT_FAILURE);
        }
    }
    // Use `msg` for stuff here

    // free `msg` when done
    free(msg);
    return 0;
}