//
// Created by shojib on 11/24/18.
//

#ifndef IMAGE_STREAMER_INPUT_H
#define IMAGE_STREAMER_INPUT_H
#include <syslog.h>
#include <pthread.h>

/* parameters for input plugin */
typedef struct _input_parameter input_parameter;
struct _input_parameter {
    struct _globals *global;
};

/* structure to store variables/functions for input plugin */
struct input {

    input_parameter param; // this holds the command line arguments

    /* signal fresh frames */
    pthread_mutex_t db;
    pthread_cond_t  db_update;

    /* global JPG frame, this is more or less the "database" */
    unsigned char *buf;
    int size;

    int init();
    int set_image(unsigned char *buffer, int buffer_size);
};
#endif //IMAGE_STREAMER_INPUT_H
