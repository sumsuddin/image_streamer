#include <input.h>
using namespace std;

int input::init()
{
    /* this mutex and the conditional variable are used to synchronize access to the image_streamer picture buffer */
    if(pthread_mutex_init(&db, NULL) != 0) {
        return -1;
    }
    if(pthread_cond_init(&db_update, NULL) != 0) {
        return -1;
    }

    return 0;
}

int input::set_image(cv::Mat &image) {
    pthread_mutex_lock(&db);
    // take whatever Mat it returns, and write it to jpeg buffer
    imencode(".jpg", image, buffer);
    // TODO: what to do if imencode returns an error?

    /* signal fresh_frame */
    pthread_cond_broadcast(&db_update);
    pthread_mutex_unlock(&db);
    return 0;
}