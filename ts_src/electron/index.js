const { ipcRenderer } = require('electron');
const $ = require('jquery');
ipcRenderer.send('kinect', 'getDeviceS/Ns');

$(document).ready(function () {
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
            config.colorResolution = +form.get('colorResolution');
        }
        if ($('#depthCamera').is(':checked')) {
            config.depthMode = +form.get('depthMode');
        }
        ipcRenderer.send('kinect', 'start', config);
    });
    $('#stopButton').click(e => {
        ipcRenderer.send('kinect', 'stop');
    });

    $('#depthCamera').trigger('click');
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
        $('#notOpenView').hide();
        $('#openView').show();
    } else {
        alert('Could not open device');
    }
});
ipcRenderer.on('close', (e, val) => {
    $('#closeDevice').prop('disabled', false);
    $('#openView').hide();
    $('#notOpenView').show();
});

ipcRenderer.on('start', (e, val) => {
    $('#startButton').prop('disabled', false);
    if (val) {
        $('#startButton').hide();
        $('#stopButton').show();
        $('#colorImage').show();
    } else {
        alert('Could not start cameras');
    }
});
ipcRenderer.on('stop', (e, val) => {
    $('#stopButton').prop('disabled', false);
    $('#startButton').show();
    $('#stopButton').hide();
    $('#colorImage').hide();
});
ipcRenderer.on('colorImage', (e, image) => {
    $('#colorImage').attr('src', `data:image/jpeg;base64,${image}`);
});
