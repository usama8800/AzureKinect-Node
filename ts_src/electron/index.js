const { ipcRenderer } = require('electron');
const $ = require('jquery');
ipcRenderer.send('kinect', 'getDeviceS/Ns');

const states = {
    open: {
        show: ['#openView'],
        hide: ['#notOpenView'],
    },
    close: {
        show: ['#notOpenView'],
        hide: ['#openView'],
    },
    start: {
        show: ['#stopButton', '#colorImage'],
        hide: ['#startButton'],
    },
    stop: {
        show: ['#startButton'],
        hide: ['#stopButton', '#colorImage'],
    },
    only720: {
        disable: ['#1080p', '#1440p', '#2160p', '#1536p', '#3072p'],
        check: ['#720p'],
        enable: ['#30fps'],
    },
    allRes: {
        enable: ['#1080p', '#1440p', '#2160p', '#1536p', '#3072p'],
    },
    no30FPS: {
        disable: ['#30fps'],
        uncheck: ['#30fps']
    },
    allFPS: {
        enable: ['#30fps'],
    }
};

const defaults = {
    click: ['#depthCamera', '#nv12', '#720p', '#15fps'],
}

function setDefaults() {
    defaults.click.forEach(s => $(s).trigger('click'));
}

function setState(stateName) {
    const state = states[stateName];
    state.show?.forEach(s => $(s).show());
    state.hide?.forEach(s => $(s).hide());
    state.disable?.forEach(s => $(s).prop('disabled', true));
    state.enable?.forEach(s => $(s).prop('disabled', false));
    state.check?.forEach(s => $(s).prop('checked', true));
    state.uncheck?.forEach(s => $(s).prop('checked', false));
}

$(document).ready(() => {
    $('#refreshDevices').click(e => {
        ipcRenderer.send('kinect', 'getDeviceS/Ns');
    });
    $('#openDevice').click(e => {
        $('#openDevice').prop('disabled', true);
        const selected = $('#devices').children('option:selected');
        $('#sn').html(selected.text());
        ipcRenderer.send('kinect', 'open', +selected.attr('val'));
    });
    $('#closeDevice').click(e => {
        $('#closeDevice').prop('disabled', true);
        ipcRenderer.send('kinect', 'close');
    });
    $('#depthCamera').change(() => {
        const checked = $('#depthCamera').is(':checked');
        const other = $('#colorCamera').is(':checked');
        if (checked) {
            $('#depthConfig').show();
            $('#synchronizedImagesOnly').prop('disabled', !other);
        } else {
            $('#depthConfig').hide();
            $('#synchronizedImagesOnly').prop('disabled', true);
        }
    });
    $('#colorCamera').change(() => {
        const checked = $('#colorCamera').is(':checked');
        const other = $('#depthCamera').is(':checked');
        if (checked) {
            $('#colorConfig').show();
            $('#synchronizedImagesOnly').prop('disabled', !other);
        } else {
            $('#colorConfig').hide();
            $('#synchronizedImagesOnly').prop('disabled', true);
        }
    });
    $('#startButton').click(e => {
        const form = new FormData($('#config')[0]);
        const config = {
            colorFormat: +(form.get('colorFormat') ?? 3),
            colorResolution: 0,
            depthMode: 0,
            cameraFPS: +(form.get('cameraFPS') ?? 2),
            synchronizedImagesOnly: $('#synchronizedImagesOnly').is(':enabled:checked'),
            depthDelayOffColorUsec: 0,
            wiredSyncMode: 0,
            subordinateDelayOffMasterUsec: 0,
            disableStreamingIndicator: $('#disableStreamingIndicator').is(':checked'),
        };
        if ($('#colorCamera').is(':checked')) {
            config.colorResolution = +(form.get('colorResolution') ?? 0);
        }
        if ($('#depthCamera').is(':checked')) {
            config.depthMode = +(form.get('depthMode') ?? 0);
        }
        ipcRenderer.send('kinect', 'start', config);
    });
    $('#stopButton').click(e => {
        ipcRenderer.send('kinect', 'stop');
    });
    $('[name=colorFormat]').change(e => {
        if (['nv12', 'yuy2'].includes($('[name=colorFormat]:checked').attr('id'))) {
            setState('only720');
        } else {
            setState('allRes');
        }
    });
    $('[name=colorResolution]').change(e => {
        if ($('[name=colorResolution]:checked').attr('id') === '3072p') {
            setState('no30FPS');
        } else {
            setState('allFPS');
        }
    });
    setDefaults();
});

ipcRenderer.on('deviceS/Ns', (e, val) => {
    $('#devices').html(val.map(v => `<option val='${v.i}'>${v.sn}</option>`).join());
    if ($('#devices').html() === '') {
        $('#devices').html('<option selected disabled>(No available devices)</option>');
        $('#openDevice').prop('disabled', true);
    } else {
        $('#openDevice').prop('disabled', false);
    }
});

ipcRenderer.on('open', (e, val) => {
    $('#openDevice').prop('disabled', false);
    if (val) {
        setState('open');
    } else {
        alert('Could not open device');
    }
});
ipcRenderer.on('close', (e, val) => {
    $('#closeDevice').prop('disabled', false);
    setState('close');
});

ipcRenderer.on('start', (e, val) => {
    $('#startButton').prop('disabled', false);
    if (val) {
        setState('start');
    } else {
        alert('Could not start cameras');
    }
});
ipcRenderer.on('stop', (e, val) => {
    $('#stopButton').prop('disabled', false);
    setState('stop');
});

ipcRenderer.on('colorImage', (e, image) => {
    $('#colorImage').attr('src', `data:image/jpeg;base64,${image}`);
});
