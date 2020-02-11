#pragma once
// #pragma comment(lib, "k4a.lib")
#include <k4a/k4a.h>
#include <node.h>

using namespace v8;

#define STR(i, s) String::NewFromUtf8(i, s)
#define ERROR(i, s) i->ThrowException(Exception::Error(STR(i, s)));
#define GET_ENUM_VAL(maybe_val, context) \
    maybe_val.ToLocalChecked()->ToInt32(context).ToLocalChecked()->Value();

class KinectDevice {
   private:
    k4a_device_t device;
    k4a_device_configuration_t config;

   public:
    KinectDevice();
    k4a_result_t open();
    k4a_result_t open(uint32_t device_number);
    void close();
    void configure(Isolate*, Local<Object>);
    char* get_serial_number();
    k4a_result_t start_cameras();
    void stop_cameras();
};
