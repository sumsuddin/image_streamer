#ifndef IMAGE_STREAMER_OUTPUT_H
#define IMAGE_STREAMER_OUTPUT_H

#define OUTPUT_PLUGIN_PREFIX " o: "
#define OPRINT(...) { char _bf[1024] = {0}; snprintf(_bf, sizeof(_bf)-1, __VA_ARGS__); fprintf(stderr, "%s", OUTPUT_PLUGIN_PREFIX); fprintf(stderr, "%s", _bf); syslog(LOG_INFO, "%s", _bf); }

/* parameters for output plugin */
typedef struct _output_parameter output_parameter;
struct _output_parameter {
    struct _globals *global;
};



/* structure to store variables/functions for output plugin */
typedef struct _output output;
struct _output {

    output_parameter param;


    int init(output_parameter *param);
    int stop();
    int run();
};



#endif //IMAGE_STREAMER_OUTPUT_H
