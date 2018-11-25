#include "include/input.h"
#include "include/image_streamer.h"
using namespace std;


int input::init()
{
    /* this mutex and the conditional variable are used to synchronize access to the global picture buffer */
    if(pthread_mutex_init(&param.global->in[param.id].db, NULL) != 0) {
        return -1;
    }
    if(pthread_cond_init(&param.global->in[param.id].db_update, NULL) != 0) {
        return -1;
    }

    param.global->in[param.id].buf       = NULL;
    param.global->in[param.id].size      = 0;

    return 0;
}

int input::set_image(unsigned char *buffer, int buffer_size) {
    if (!param.global->stop) {
        input * in = &param.global->in[param.id];

        /* copy JPG picture to global buffer */
        pthread_mutex_lock(&in->db);


        // TODO: what to do if imencode returns an error?

        // std::vector is guaranteed to be contiguous
        in->buf = buffer;
        in->size = buffer_size;

        /* signal fresh_frame */
        pthread_cond_broadcast(&in->db_update);
        pthread_mutex_unlock(&in->db);
        return 0;
    }
    return -1;
}