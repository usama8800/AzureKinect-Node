#include "kinect.h"

KinectDevice *device;

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

void initialize(Local<Object> exports) {
    device = new KinectDevice();
    NODE_SET_METHOD(exports, "open", open);
    NODE_SET_METHOD(exports, "close", close);
    NODE_SET_METHOD(exports, "configure", configure);
    NODE_SET_METHOD(exports, "getSerialNumber", get_serial_number);
    NODE_SET_METHOD(exports, "startCameras", start_cameras);
    NODE_SET_METHOD(exports, "stopCameras", stop_cameras);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, initialize)
