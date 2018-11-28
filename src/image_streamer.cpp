
#include <image_streamer.h>

ImageStreamer::ImageStreamer(int port) {
    /* ignore SIGPIPE (send by OS if transmitting to closed TCP sockets) */
    signal(SIGPIPE, SIG_IGN);

    start();
    /* open output plugin */
    if(out.init(this, port)) {
        LOG("output_init() return value signals to exit\n");
        closelog();
        exit(EXIT_FAILURE);
    }

    DBG("starting output plugin(s)\n");
    out.run();
}

int ImageStreamer::get_num_inputs() {
    return in.size();
}

void ImageStreamer::start() {
    running = true;
}

void ImageStreamer::set_image(int input_id, cv::Mat &image) {
    if (in.size() > input_id) {
        in[input_id]->set_image(image);
    }
}

bool ImageStreamer::is_running() {
    return running;
}

int ImageStreamer::create_new_input() {
    input *inp = new input();
    if(inp->init() == -1) {
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

void ImageStreamer::stop() {
    running = false;
}