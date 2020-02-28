import bindings from 'bindings';
import { Config, ImageType, Result, WaitResult } from './defs';
import { KinectImage } from './kinectImage';
const kinect = bindings('azurekinect_node');

const colorImage = new KinectImage(ImageType.COLOR);

export class Kinect {

    getNumConnectedDevices(): number {
        return kinect.getNumConnectedDevices();
    }

    open(deviceNumber?: number): Result {
        let res: Result;
        if (deviceNumber) { res = kinect.open(deviceNumber); } else { res = kinect.open(); }
        return res;
    }

    close() {
        kinect.close();
    }

    configure(config: Config) {
        kinect.configure(config);
    }

    getSerialNumber(): string {
        return kinect.getSerialNumber();
    }

    startCameras(config?: Config): Result {
        let res: Result;
        if (config) { res = kinect.startCameras(config); } else { res = kinect.startCameras(); }
        return res;
    }

    stopCameras() {
        kinect.stopCameras();
    }

    capture(timeout?: number): WaitResult {
        let res: WaitResult;
        if (timeout) { res = kinect.capture(timeout); } else { res = kinect.capture(); }
        return res;
    }

    getColorImage(): KinectImage | undefined {
        const res = kinect.getColorImage();
        if (res) { return colorImage; }
        return undefined;
    }

    releaseImagesAndCapture() {
        kinect.releaseImagesAndCapture();
    }

    getImageBuffer(image: KinectImage): Int8Array {
        const data = kinect.getImageBuffer(image.type);
        return data;
    }
}
