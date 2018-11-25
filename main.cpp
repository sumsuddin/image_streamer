#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <pthread.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <syslog.h>
#include <linux/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <dlfcn.h>
#include <pthread.h>


#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

#include "include/utils.h"
#include "include/image_streamer.h"

/* globals */
static globals global;

void *worker_thread(void *arg)
{
    input * in = (input*)arg;
    Mat src;
    VideoCapture capture(in->param.id );
    capture.set(CAP_PROP_FPS, 30);

    while (!global.stop) {
        if (!capture.read(src))
            break; // TODO

        vector<uchar> jpeg_buffer;
        // take whatever Mat it returns, and write it to jpeg buffer
        imencode(".jpg", src, jpeg_buffer);

        // TODO: what to do if imencode returns an error?

        in->set_image(&jpeg_buffer[0], jpeg_buffer.size());
        //usleep(100);
    }


    return NULL;
}

void add_input() {
    global.in[global.incnt].param.id = global.incnt;
    global.in[global.incnt].param.global = &global;
    if(global.in[global.incnt].init() == -1) {
        LOG("input_init() return value signals to exit\n");
        closelog();
        return;
    }

    pthread_t  worker;
    if(pthread_create(&worker, 0, worker_thread, &global.in[global.incnt]) != 0) {
        fprintf(stderr, "could not start worker thread\n");
        return;
    }
    pthread_detach(worker);
    global.incnt++;
}


/******************************************************************************
Description.:
Input Value.:
Return Value:
******************************************************************************/
int main(int argc, char *argv[])
{
    global.incnt = 0;

    /* open input plugin */
    add_input();
    add_input();

    /* open output plugin */
    global.out.param.global = &global;
    if(global.out.init(&global.out.param)) {
        LOG("output_init() return value signals to exit\n");
        closelog();
        exit(EXIT_FAILURE);
    }

    DBG("starting %d output plugin(s)\n", global.outcnt);
    global.out.run();

    /* wait for signals */
    pause();

    return 0;
}