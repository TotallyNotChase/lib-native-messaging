#ifndef NATIVEMSG
#define NATIVEMSG
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

/*
Reads the message sent the by browser extension and returns a uint8_t pointer to it
The pointer should be freed using `free()`

@param: length - address of a `uint32_t` variable, to store the length of returned buffer

Returns NULL on failure
The corresponding error is written to stderr

**NOTE**: The returned pointer does not include a null terminator
*/
uint8_t* nativemsg_read(uint32_t* length);

/*
Writes given message for the browser extension to read

@param: buffer - pointer to the array of bytes to be written (should not include null terminator)
@param: length - length of the array - should not be more than (1024 * 1024)

Returns number of bytes written to stdout
The number of bytes written is always length + 4 on success
The 4 extra bytes being the int value: length

Returns 0 on failure
The corresponding error is written to stderr
*/
size_t nativemsg_write(const uint8_t* const buffer, uint32_t length);

#endif // NATIVEMSG

