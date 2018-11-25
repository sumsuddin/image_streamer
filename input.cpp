#include "include/input.h"
#include "include/image_streamer.h"
using namespace std;


int input::init()
{
    /* this mutex and the conditional variable are used to synchronize access to the global picture buffer */
    if(pthread_mutex_init(&db, NULL) != 0) {
        return -1;
    }
    if(pthread_cond_init(&db_update, NULL) != 0) {
        return -1;
    }

    buf       = NULL;
    size      = 0;

    return 0;
}

int input::set_image(unsigned char *buffer, int buffer_size) {
    if (!param.global->stop) {
        /* copy JPG picture to global buffer */
        pthread_mutex_lock(&db);


        // TODO: what to do if imencode returns an error?

        // std::vector is guaranteed to be contiguous
        buf = buffer;
        size = buffer_size;

        /* signal fresh_frame */
        pthread_cond_broadcast(&db_update);
        pthread_mutex_unlock(&db);
        return 0;
    }
    return -1;
}