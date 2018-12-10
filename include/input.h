//
// Created by shojib on 11/24/18.
//

#ifndef IMAGE_STREAMER_INPUT_H
#define IMAGE_STREAMER_INPUT_H
#include <syslog.h>
#include <pthread.h>

#include <image_streamer.h>
#include <opencv2/opencv.hpp>

struct ImageStreamer;

/* structure to store variables/functions for input plugin */
struct input {

    /* signal fresh frames */
    pthread_mutex_t db;
    pthread_cond_t  db_update;

    /* image_streamer JPG frame, this is more or less the "database" */
    unsigned char *buf;
    int size;

    int init();
    int set_image(cv::Mat &image);
    std::vector<uchar> buffer;
};
#endif //IMAGE_STREAMER_INPUT_H
