//
// Created by shojib on 11/24/18.
//

#ifndef IMAGE_STREAMER_IMAGE_STREAMER_H
#define IMAGE_STREAMER_IMAGE_STREAMER_H
/* FIXME take a look to the output_http clients thread marked with fixme if you want to set more then 10 plugins */
#define MAX_INPUT_PLUGINS 10
#define MAX_OUTPUT_PLUGINS 10
#define MAX_PLUGIN_ARGUMENTS 32

#include <linux/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <pthread.h>

#ifdef DEBUG
#define DBG(...) fprintf(stderr, " DBG(%s, %s(), %d): ", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

#define LOG(...) { char _bf[1024] = {0}; snprintf(_bf, sizeof(_bf)-1, __VA_ARGS__); fprintf(stderr, "%s", _bf); syslog(LOG_INFO, "%s", _bf); }

#include "input.h"
#include "output.h"

/* global variables that are accessed by all plugins */
typedef struct _globals globals;


struct _globals {
    int stop;

    /* input plugin */
    input in[MAX_INPUT_PLUGINS];
    int incnt;

    /* output plugin */
    output out;
};
#endif //IMAGE_STREAMER_IMAGE_STREAMER_H
