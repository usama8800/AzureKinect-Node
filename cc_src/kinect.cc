#include "kinect.h"

KinectDevice::KinectDevice() {
    device = NULL;
    config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.camera_fps = K4A_FRAMES_PER_SECOND_15;
    config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
    config.color_resolution = K4A_COLOR_RESOLUTION_720P;
}

uint32_t KinectDevice::get_num_connected_devices() {
    return k4a_device_get_installed_count();
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
        config.color_format = (k4a_image_format_t)OBJ_GET_INT(mv, c);
    }
    mv = my_config->Get(c, STR(i, "colorResolution"));
    if (!mv.IsEmpty()) {
        config.color_resolution = (k4a_color_resolution_t)OBJ_GET_INT(mv, c);
    }
    mv = my_config->Get(c, STR(i, "depthMode"));
    if (!mv.IsEmpty()) {
        config.depth_mode = (k4a_depth_mode_t)OBJ_GET_INT(mv, c);
    }
    mv = my_config->Get(c, STR(i, "cameraFPS"));
    if (!mv.IsEmpty()) {
        config.camera_fps = (k4a_fps_t)OBJ_GET_INT(mv, c);
    }
    mv = my_config->Get(c, STR(i, "synchronizedImagesOnly"));
    if (!mv.IsEmpty()) {
        config.synchronized_images_only = OBJ_GET_INT(mv, c);
    }
    mv = my_config->Get(c, STR(i, "depthDelayOffColorUsec"));
    if (!mv.IsEmpty()) {
        config.depth_delay_off_color_usec = OBJ_GET_INT(mv, c);
    }
    mv = my_config->Get(c, STR(i, "wiredSyncMode"));
    if (!mv.IsEmpty()) {
        config.wired_sync_mode = (k4a_wired_sync_mode_t)OBJ_GET_INT(mv, c);
    }
    mv = my_config->Get(c, STR(i, "subordinateDelayOffMasterUsec"));
    if (!mv.IsEmpty()) {
        config.subordinate_delay_off_master_usec = OBJ_GET_INT(mv, c);
    }
    mv = my_config->Get(c, STR(i, "disableStreamingIndicator"));
    if (!mv.IsEmpty()) {
        config.disable_streaming_indicator = (bool)OBJ_GET_INT(mv, c);
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

k4a_wait_result_t KinectDevice::get_capture(int32_t timeout) {
    images_set.capture = 1;
    return k4a_device_get_capture(device, &capture, timeout);
}

k4a_image_t KinectDevice::get_color_image() {
    color_image = k4a_capture_get_color_image(capture);
    images_set.color = color_image != 0;
    images_set.capture |= images_set.color;
    return color_image;
}

k4a_image_t KinectDevice::get_depth_image() {
    depth_image = k4a_capture_get_depth_image(capture);
    images_set.depth = depth_image != 0;
    images_set.capture |= images_set.depth;
    return depth_image;
}

k4a_image_t KinectDevice::get_ir_image() {
    ir_image = k4a_capture_get_ir_image(capture);
    images_set.ir = ir_image != 0;
    images_set.capture |= images_set.ir;
    return ir_image;
}

void KinectDevice::release_images_and_capture() {
    if (images_set.color) {
        k4a_image_release(color_image);
        images_set.color = 0;
    }
    if (images_set.depth) {
        k4a_image_release(depth_image);
        images_set.depth = 0;
    }
    if (images_set.ir) {
        k4a_image_release(ir_image);
        images_set.ir = 0;
    }
    if (images_set.capture) {
        k4a_capture_release(capture);
        images_set.capture = 0;
    }
}

uint8_t *KinectDevice::get_image_buffer(image_type type) {
    if (type == DEPTH_IMAGE) return k4a_image_get_buffer(depth_image);
    if (type == IR_IMAGE) return k4a_image_get_buffer(ir_image);
    return k4a_image_get_buffer(color_image);
}

size_t KinectDevice::get_image_size(image_type type) {
    if (type == DEPTH_IMAGE) return k4a_image_get_size(depth_image);
    if (type == IR_IMAGE) return k4a_image_get_size(ir_image);
    return k4a_image_get_size(color_image);
}

int KinectDevice::get_stride_bytes(image_type type) {
    if (type == COLOR_IMAGE &&
        config.color_format == K4A_IMAGE_FORMAT_COLOR_NV12) {
        return k4a_image_get_stride_bytes(color_image);
    }
    return -1;
}

int KinectDevice::get_image_width(image_type type) {
    if (type == COLOR_IMAGE) return k4a_image_get_width_pixels(color_image);
    return -1;
}

int KinectDevice::get_image_height(image_type type) {
    if (type == COLOR_IMAGE) return k4a_image_get_height_pixels(color_image);
    return -1;
}
