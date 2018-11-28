# image_streamer

**Build**

    $ mkdir build && cd build
    $ cmake ..
    $ ./demo

**Code Sample**

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

**Usage**

The library can be used to set multiple input streams.

    int input_id = image_streamer.create_new_input();

Calling the above method creates a new input stream and returns the id of the stream.

    image_streamer.set_image(input_id, src);

Image can be set to a specific stream using the above method.

**Output**

On the browser go to http://localhost:8888/?stream=0

Here 0 is the 1st input stream output for multiple streams replace the specific id instead of 0.