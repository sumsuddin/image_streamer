# image_streamer


    /* ImageStreamer */
    static ImageStreamer image_streamer(8877);

    int main() {

        int input_id = image_streamer.create_new_input();
        Mat src;
        VideoCapture capture(0);
        capture.set(CAP_PROP_FPS, 30);

        while (image_streamer.is_running()) {
            if (!capture.read(src))
                break; // TODO

            image_streamer.set_image(input_id, src);
        }
    }
