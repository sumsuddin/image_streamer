//
// Created by shojib on 11/24/18.
//

#ifndef IMAGE_STREAMER_IMAGE_STREAMER_H
#define IMAGE_STREAMER_IMAGE_STREAMER_H
/* FIXME take a look to the output_http clients thread marked with fixme if you want to set more then 10 plugins */
#define MAX_INPUT_PLUGINS 10

#include <linux/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <pthread.h>
#include <vector>
#include <mutex>

#include "httpd.h"
#include "opencv2/opencv.hpp"

#ifdef DEBUG
#define DBG(...) fprintf(stderr, " DBG(%s, %s(), %d): ", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

#define LOG(...) { char _bf[1024] = {0}; snprintf(_bf, sizeof(_bf)-1, __VA_ARGS__); fprintf(stderr, "%s", _bf); syslog(LOG_INFO, "%s", _bf); }

#include "input.h"
#include "output.h"

struct input;

/* image_streamer variables that are accessed by all plugins */

class ImageStreamer {

public:

    ImageStreamer();

    bool is_running();

    // creates and returns the new input id
    int get_new_input();

    void set_image(int input_id, cv::Mat &image);

    void stop();

private:
    std::mutex input_locker;
    bool running = false;

    /* input plugin */
    std::vector<input*> in;

    /* output plugin */
    output out;

    void start();

    friend void send_stream(cfd *context_fd, int input_number);
    friend void *client_thread(void *arg);
};
#endif //IMAGE_STREAMER_IMAGE_STREAMER_H
