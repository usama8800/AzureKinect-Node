import {Kinect} from './kinect';
import { setTimeout } from 'timers';

const k = new Kinect();
try {
    k.open();

    k.getSerialNumber();
    k.startCameras();


    setTimeout(() => {
        k.stopCameras();
        k.close();
    }, 5000);
} catch (e) {
    if (e instanceof Error) {
        console.error(e.message);
    }
    process.exit();
}
