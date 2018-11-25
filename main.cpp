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

#include "include/utils.h"
#include "include/image_streamer.h"

using namespace cv;
using namespace std;

/* globals */
static globals global;

struct worker_arg {
    int input_id;
    int capture_device;
};

void *worker_thread(void *arg)
{
    worker_arg *worker = (worker_arg*)arg;
    int input_id = worker->input_id;
    Mat src;
    VideoCapture capture(worker->capture_device);
    capture.set(CAP_PROP_FPS, 30);
    delete worker;

    while (global.is_running()) {
        if (!capture.read(src))
            break; // TODO

        global.set_image(input_id, src);
        //usleep(100);
    }
}

void add_input(int device) {

    int input_id = global.get_new_input();
    if (input_id != -1) {
        pthread_t worker;
        worker_arg *arg = new worker_arg();
        arg->input_id = input_id;
        arg->capture_device = device;
        if (pthread_create(&worker, 0, worker_thread, arg) != 0) {
            fprintf(stderr, "could not start worker thread\n");
            return;
        }
        pthread_detach(worker);
    }
}


/******************************************************************************
Description.:
Input Value.:
Return Value:
******************************************************************************/
int main(int argc, char *argv[])
{
    /* open input plugin */
    add_input(0);
    //add_input(1);

    /* wait for signals */
    pause();

    return 0;
}