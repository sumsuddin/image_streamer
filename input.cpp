#include "include/input.h"
#include "include/image_streamer.h"
using namespace std;

/* private functions and variables to this plugin */
static globals     *pglobal;


int input::init(input_parameter *param)
{
    pglobal = param->global;
    input * in = &pglobal->in[param->id];

    in->buf = NULL;
    in->size = 0;

    return 0;
}

int input::set_image(unsigned char *buffer, int buffer_size, int id) {
    if (!pglobal->stop) {
        input * in = &pglobal->in[id];

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