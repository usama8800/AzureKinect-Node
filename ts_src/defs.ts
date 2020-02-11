export interface Config {
    colorFormat?: ColorFormat;
    colorResolution?: ColorResolution;
    depthMode?: DepthMode;
    cameraFPS?: CameraFPS;
    synchronizedImagesOnly?: boolean;
    depthDelayOffColorUsec?: number;
    wiredSyncMode?: WiredSyncMode;
    subordinateDelayOffMasterUsec?: number;
    disableStreamingIndicator?: boolean
}

export enum WiredSyncMode {
    Standalone = 0, Master, Subordinate
}

export enum CameraFPS {
    _5 = 0, _15, _30
}

export enum DepthMode {
    Off = 0, NFOV_2X2Binned, NFOV_Unbinned, WFOV_2X2Binned, WFOV_Unbinned, PassiveIR
}

export enum ColorResolution {
    Off = 0, _720, _1080, _1440, _1536, _2160, _3072 
}

export enum ColorFormat {
    ColorMJPG = 0, ColorNV12, ColorYUY2, ColorBGRA32, Depth16, IR16, Custom8, Custom16, Custom
}