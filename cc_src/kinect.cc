#include "kinect.h"

KinectDevice::KinectDevice() {
    device = NULL;
    config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.camera_fps = K4A_FRAMES_PER_SECOND_15;
    config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
    config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
}

k4a_result_t KinectDevice::open() { return open(K4A_DEVICE_DEFAULT); }

k4a_result_t KinectDevice::open(uint32_t device_number) {
    return k4a_device_open(device_number, &device);
}

void KinectDevice::close() { k4a_device_close(device); }

void KinectDevice::configure(Isolate *i, Local<Object> my_config) {
    Local<Context> c = i->GetCurrentContext();
    MaybeLocal<Value> mv;

    mv = my_config->Get(c, STR(i, "colorFormat"));
    if (!mv.IsEmpty()) {
        config.color_format = (k4a_image_format_t)GET_ENUM_VAL(mv, c);
    }
    mv = my_config->Get(c, STR(i, "colorResolution"));
    if (!mv.IsEmpty()) {
        config.color_resolution = (k4a_color_resolution_t)GET_ENUM_VAL(mv, c);
    }
    mv = my_config->Get(c, STR(i, "depthMode"));
    if (!mv.IsEmpty()) {
        config.depth_mode = (k4a_depth_mode_t)GET_ENUM_VAL(mv, c);
    }
    mv = my_config->Get(c, STR(i, "cameraFPS"));
    if (!mv.IsEmpty()) {
        config.camera_fps = (k4a_fps_t)GET_ENUM_VAL(mv, c);
    }
    mv = my_config->Get(c, STR(i, "synchronizedImagesOnly"));
    if (!mv.IsEmpty()) {
        config.synchronized_images_only = GET_ENUM_VAL(mv, c);
    }
    mv = my_config->Get(c, STR(i, "depthDelayOffColorUsec"));
    if (!mv.IsEmpty()) {
        config.depth_delay_off_color_usec = GET_ENUM_VAL(mv, c);
    }
    mv = my_config->Get(c, STR(i, "wiredSyncMode"));
    if (!mv.IsEmpty()) {
        config.wired_sync_mode = (k4a_wired_sync_mode_t)GET_ENUM_VAL(mv, c);
    }
    mv = my_config->Get(c, STR(i, "subordinateDelayOffMasterUsec"));
    if (!mv.IsEmpty()) {
        config.subordinate_delay_off_master_usec = GET_ENUM_VAL(mv, c);
    }
    mv = my_config->Get(c, STR(i, "disableStreamingIndicator"));
    if (!mv.IsEmpty()) {
        config.disable_streaming_indicator = (bool)GET_ENUM_VAL(mv, c);
    }
}

char *KinectDevice::get_serial_number() {
    // Get the size of the serial number
    size_t serial_size = 0;
    k4a_device_get_serialnum(device, NULL, &serial_size);

    // Allocate memory for the serial, then acquire it
    char *serial = (char *)(malloc(serial_size));
    k4a_device_get_serialnum(device, serial, &serial_size);
    return serial;
}

k4a_result_t KinectDevice::start_cameras() {
    return k4a_device_start_cameras(device, &config);
}

void KinectDevice::stop_cameras() { k4a_device_stop_cameras(device); }
