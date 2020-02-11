import bindings from 'bindings';
import { Config } from './defs';
const kinect = bindings('myaddon')

export class Kinect {

    open(deviceNumber = 0) {
        const res = kinect.open(deviceNumber);
        if (res === 1) {
            throw new Error('Could not connect to device');
        }
        return true;
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

    startCameras(config?: Config) {
        if (config) kinect.startCameras(config);
        else kinect.startCameras();
    }

    stopCameras() {
        kinect.stopCameras();
    }
}
