#ifndef NATIVEMSG_ERRMACROS
#define NATIVEMSG_ERRMACROS

/*
Macro to make sure correct number of bytes were read

If not, writes the corresponding error to stderr
Then, Frees the given buffer and returns NULL
*/
#define COUNT_CHECK_STDIN(count, length, err, buffer_to_clean)                                     \
        do {                                                                                       \
            if (count != length)                                                                   \
            {                                                                                      \
                if (feof(stdin))                                                                   \
                {                                                                                  \
                    fprintf(stderr, "Unexpectedly encountered EOF while reading stdin\n");         \
                }                                                                                  \
                else if ((err = ferror(stdin)))                                                    \
                {                                                                                  \
                    fprintf(stderr, "An error occured while reading stdin, err code: %d\n", err);  \
                    clearerr(stdin);                                                               \
                }                                                                                  \
                free(buffer_to_clean);                                                             \
                return NULL;                                                                       \
            }                                                                                      \
        } while(0)


/*
Macro to make sure correct number of bytes were written

If not, writes the corresponding error to stderr
Then, returns NULL
*/
#define COUNT_CHECK_STDOUT(count, length, err)                                                         \
        do {                                                                                           \
            if (count != length)                                                                       \
            {                                                                                          \
                if (feof(stdout))                                                                      \
                {                                                                                      \
                    fprintf(stderr, "Unexpectedly encountered EOF while writing to stdout\n");         \
                }                                                                                      \
                else if ((err = ferror(stdout)))                                                       \
                {                                                                                      \
                    fprintf(stderr, "An error occured while writing to stdout, err code: %d\n", err);  \
                    clearerr(stdout);                                                                  \
                }                                                                                      \
                return 0;                                                                              \
            }                                                                                          \
        } while(0)

#endif // !NATIVEMSG_ERRMACROS
