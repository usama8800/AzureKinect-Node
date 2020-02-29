import { app, BrowserWindow, ipcMain } from 'electron';
import path from 'path';
import { ColorFormat, Config, Result } from '../defs';
import { Kinect } from '../kinect';

const k = new Kinect();

function createWindow() {
    const width = 1200;
    const win = new BrowserWindow({
        width,
        height: width * 9 / 16,
        webPreferences: {
            nodeIntegration: true,
        },
    });

    win.loadFile(path.join(__dirname, '..', '..', 'ts_src', 'electron', 'index.html'));
    win.webContents.openDevTools();
    win.on('close', e => {
        k.close();
    });
}

function startCameras(e: Electron.IpcMainEvent, args: Config) {
    if (args.colorFormat === ColorFormat.ColorMJPG) {
        k.capture();
        const image = k.getColorImage();
        if (image) {
            const imageBuffer = k.getImageBuffer(image);
            e.reply('colorImage', imageBuffer.toString('base64'));
        }
        k.releaseImagesAndCapture();
    }
}

let startCamerasInterval: number;
ipcMain.on('kinect', (e, func, args) => {
    let res: Result;
    switch (func) {
        case 'getDeviceS/Ns':
            const sns: { sn: string, i: number }[] = [];
            for (let i = 0; i < k.getNumConnectedDevices(); i++) {
                res = k.open(i);
                if (res === Result.FAILED) { continue; }
                sns.push({ sn: k.getSerialNumber(), i });
                k.close();
            }
            e.reply('deviceS/Ns', sns);
            break;
        case 'open':
            res = k.open(args);
            e.reply('open', res === Result.SUCCEEDED);
            break;
        case 'close':
            k.close();
            e.reply('close', true);
            break;
        case 'configure':
            k.configure(args);
            break;
        case 'start':
            res = k.startCameras(args);
            e.reply('start', res === Result.SUCCEEDED);
            const twiceFPS = ((args.cameraFPS * 15) || 5) * 2;
            startCamerasInterval = setInterval(startCameras, 1000 / twiceFPS, e, args);
            startCameras(e, args);
            break;
        case 'stop':
            clearInterval(startCamerasInterval);
            k.stopCameras();
            e.reply('stop', true);
            break;
        default:
            e.reply('err', 'Function not implemented');
            break;
    }
});

app.allowRendererProcessReuse = true;
app.whenReady().then(createWindow);
