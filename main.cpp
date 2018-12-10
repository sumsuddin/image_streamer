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

#include <utils.h>
#include <image_streamer.h>

using namespace cv;
using namespace std;

/* ImageStreamer */
static ImageStreamer image_streamer(8877);

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

    while (image_streamer.is_running()) {
        if (!capture.read(src))
            break; // TODO

        image_streamer.set_image(input_id, src);
        //usleep(100);
    }
}

void add_input(int device) {

    int input_id = image_streamer.create_new_input();
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

#if true // simplified
    int input_id = image_streamer.create_new_input();
    Mat src;
    VideoCapture capture("rtsp://service:12Tigerit%2B%2B@192.168.105.5");
    capture.set(CAP_PROP_FPS, 30);

    while (image_streamer.is_running()) {
        if (!capture.read(src))
            break; // TODO

        image_streamer.set_image(input_id, src);
        //usleep(100);
    }

#else
    //threaded
    /* open input plugin */
    add_input(0);
    //add_input(1);

    /* wait for signals */
    pause();
#endif
    return 0;

}