#pragma once
#include <k4a/k4a.h>
#include <malloc.h>
#include <node.h>
#include <stdlib.h>

using namespace v8;

#define STR(i, s) \
    String::NewFromUtf8(i, s, NewStringType::kNormal).ToLocalChecked()
#define ERROR(i, s) i->ThrowException(Exception::Error(STR(i, s)))
#define OBJ_GET_INT(maybe_val, context) \
    maybe_val.ToLocalChecked()->ToInt32(context).ToLocalChecked()->Value()

class KinectDevice {
   private:
    k4a_device_t device;
    k4a_device_configuration_t config;
    k4a_capture_t capture;
    k4a_image_t color_image;
    k4a_image_t depth_image;
    k4a_image_t ir_image;
    struct images_set_t {
        bool color : 1;
        bool depth : 1;
        bool ir : 1;
        bool capture : 1;
    } images_set;

   public:
    KinectDevice();
    typedef enum { COLOR_IMAGE = 0, DEPTH_IMAGE, IR_IMAGE } image_type;
    uint32_t get_num_connected_devices();
    k4a_result_t open();
    k4a_result_t open(uint32_t device_number);
    void close();
    void configure(Isolate*, Local<Object>);
    char* get_serial_number();
    k4a_result_t start_cameras();
    void stop_cameras();
    k4a_wait_result_t get_capture(int32_t timeout);
    k4a_image_t get_color_image();
    k4a_image_t get_depth_image();
    k4a_image_t get_ir_image();
    void release_images_and_capture();
    uint8_t* get_image_buffer(image_type);
    size_t get_image_size(image_type);
    int get_stride_bytes(image_type);
    int get_image_width(image_type);
    int get_image_height(image_type);
};
