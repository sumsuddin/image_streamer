
#include "include/image_streamer.h"

globals::globals() {
    start();
    /* open output plugin */
    if(out.init(this)) {
        LOG("output_init() return value signals to exit\n");
        closelog();
        exit(EXIT_FAILURE);
    }

    DBG("starting %d output plugin(s)\n", global.outcnt);
    out.run();
}

void globals::start() {
    running = true;
}

void globals::set_image(int input_id, cv::Mat &image) {
    if (in.size() > input_id) {
        in[input_id]->set_image(image);
    }
}

bool globals::is_running() {
    return running;
}

int globals::get_new_input() {
    input *inp = new input();
    if(inp->init(this) == -1) {
        LOG("input_init() return value signals to exit\n");
        closelog();
        return -1;
    }
    input_locker.lock();
    int id = in.size();
    in.push_back(inp);
    input_locker.unlock();
    return id;
}

void globals::stop() {
    running = false;
}