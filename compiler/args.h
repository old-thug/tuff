#ifndef __TUFF_ARGS_H_
#define __TUFF_ARGS_H_

typedef struct {
    const char *input_file;
} Args;

Args parse_args (int argc, char **argv);

#endif // __TUFF_ARGS_H_
