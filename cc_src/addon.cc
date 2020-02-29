#include "kinect.h"

KinectDevice *device;

void num_connected_devices(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 0) {
        ERROR(isolate, "Error: no arguments expected");
        return;
    }

    uint32_t num = device->get_num_connected_devices();
    args.GetReturnValue().Set(num);
    return;
}

void open(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 1) {
        ERROR(isolate, "Error: no arguments or one integer expected");
        return;
    }

    Local<Context> context = isolate->GetCurrentContext();
    uint32_t device_number = 0;
    if (args.Length() == 1) {
        MaybeLocal<Uint32> maybe_device_number = args[0]->ToUint32(context);
        if (!maybe_device_number.IsEmpty()) {
            device_number = maybe_device_number.ToLocalChecked()->Value();
        } else {
            ERROR(isolate, "Error: integer expected");
            return;
        }
    }

    k4a_result_t result = device->open(device_number);
    args.GetReturnValue().Set(result);
}

void close(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 0) {
        ERROR(isolate, "Error: no arguments expected");
        return;
    }
    device->close();
}

void get_serial_number(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 0) {
        ERROR(isolate, "Error: no arguments expected");
        return;
    }
    char *serial_number = device->get_serial_number();
    args.GetReturnValue().Set(STR(isolate, serial_number));
    free(serial_number);
}

void configure(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    if (args.Length() != 1) {
        ERROR(isolate, "Error: one object expected");
        return;
    }

    Local<Context> context = isolate->GetCurrentContext();
    MaybeLocal<Object> maybe_config = args[0]->ToObject(context);
    if (!maybe_config.IsEmpty()) {
        device->configure(isolate, maybe_config.ToLocalChecked());
    } else {
        ERROR(isolate, "Error: object expected");
        return;
    }
}

void start_cameras(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 1) {
        ERROR(isolate, "Error: no arguments or one object expected");
        return;
    }

    if (args.Length() == 1) {
        Local<Context> context = isolate->GetCurrentContext();
        MaybeLocal<Object> maybe_config = args[0]->ToObject(context);
        if (!maybe_config.IsEmpty()) {
            device->configure(isolate, maybe_config.ToLocalChecked());
        } else {
            ERROR(isolate, "Error: object expected");
            return;
        }
    }

    k4a_result_t result = device->start_cameras();
    args.GetReturnValue().Set(result);
}

void stop_cameras(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 0) {
        ERROR(isolate, "Error: no arguments expected");
        return;
    }

    device->stop_cameras();
}

void capture(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 1) {
        ERROR(isolate, "Error: no arguments or one integer expected");
        return;
    }

    Local<Context> context = isolate->GetCurrentContext();
    int32_t timeout = K4A_WAIT_INFINITE;
    if (args.Length() == 1) {
        MaybeLocal<Uint32> maybe_timeout = args[0]->ToUint32(context);
        if (!maybe_timeout.IsEmpty()) {
            timeout = maybe_timeout.ToLocalChecked()->Value();
        } else {
            ERROR(isolate, "Error: integer expected");
            return;
        }
    }

    k4a_wait_result_t result = device->get_capture(timeout);
    args.GetReturnValue().Set(result);
}

void get_color_image(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 0) {
        ERROR(isolate, "Error: no arguments expected");
        return;
    }

    k4a_image_t image = device->get_color_image();
    if (image)
        args.GetReturnValue().Set(true);
    else
        args.GetReturnValue().Set(false);
}

void get_depth_image(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 0) {
        ERROR(isolate, "Error: no arguments expected");
        return;
    }

    k4a_image_t image = device->get_depth_image();
    if (image)
        args.GetReturnValue().Set(true);
    else
        args.GetReturnValue().Set(false);
}

void get_ir_image(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 0) {
        ERROR(isolate, "Error: no arguments expected");
        return;
    }

    k4a_image_t image = device->get_ir_image();
    if (image)
        args.GetReturnValue().Set(true);
    else
        args.GetReturnValue().Set(false);
}

void release_images_and_capture(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() > 0) {
        ERROR(isolate, "Error: no arguments expected");
        return;
    }

    device->release_images_and_capture();
}

void get_image_buffer(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() != 1) {
        ERROR(isolate, "Error: one image type expected");
        return;
    }

    Local<Context> context = isolate->GetCurrentContext();
    MaybeLocal<Uint32> maybe_image_type = args[0]->ToUint32(context);
    if (maybe_image_type.IsEmpty()) {
        ERROR(isolate, "Error: image type expected");
        return;
    }
    KinectDevice::image_type image_type =
        (KinectDevice::image_type)maybe_image_type.ToLocalChecked()->Value();
    uint8_t *buffer = device->get_image_buffer(image_type);
    size_t size = device->get_image_size(image_type);
    Local<ArrayBuffer> array = ArrayBuffer::New(isolate, (void *)buffer, size);
    Local<Int8Array> data = Int8Array::New(array, 0, size);
    args.GetReturnValue().Set(data);
}

void initialize(Local<Object> exports) {
    device = new KinectDevice();

    NODE_SET_METHOD(exports, "getNumConnectedDevices", num_connected_devices);
    NODE_SET_METHOD(exports, "open", open);
    NODE_SET_METHOD(exports, "close", close);
    NODE_SET_METHOD(exports, "configure", configure);
    NODE_SET_METHOD(exports, "getSerialNumber", get_serial_number);
    NODE_SET_METHOD(exports, "startCameras", start_cameras);
    NODE_SET_METHOD(exports, "stopCameras", stop_cameras);
    NODE_SET_METHOD(exports, "capture", capture);
    NODE_SET_METHOD(exports, "getColorImage", get_color_image);
    NODE_SET_METHOD(exports, "getDepthImage", get_depth_image);
    NODE_SET_METHOD(exports, "getIRImage", get_ir_image);
    NODE_SET_METHOD(exports, "releaseImagesAndCapture",
                    release_images_and_capture);
    NODE_SET_METHOD(exports, "getImageBuffer", get_image_buffer);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, initialize);
