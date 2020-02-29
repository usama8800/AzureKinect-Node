import bindings from 'bindings';
import { Config, ImageType, Result, WaitResult } from './defs';
import { KinectImage } from './kinectImage';
const kinect = bindings('azurekinect_node');

const colorImage = new KinectImage(ImageType.COLOR);
const depthImage = new KinectImage(ImageType.DEPTH);
const irImage = new KinectImage(ImageType.IR);

export class Kinect {

    private config: Config = {};

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
        this.config = config;
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
        kinect.releaseImagesAndCapture();
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

    getDepthImage(): KinectImage | undefined {
        const res = kinect.getDepthImage();
        if (res) { return depthImage; }
        return undefined;
    }

    getIRImage(): KinectImage | undefined {
        const res = kinect.getIRImage();
        if (res) { return irImage; }
        return undefined;
    }

    releaseImagesAndCapture() {
        kinect.releaseImagesAndCapture();
    }

    getImageBuffer(image: KinectImage): Buffer {
        const data = kinect.getImageBuffer(image.type);
        return Buffer.from(data);
    }
}
