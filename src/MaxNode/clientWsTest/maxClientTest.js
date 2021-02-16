const maxApi = require("max-api");
const io = require('socket.io-client');

let socket;

maxApi.addHandler('connect', (url) => {
    socket = io(url);

    //Incoming socket messages
    socket.on('message', (...msg) => {
        maxApi.outlet("message", ...msg);
    });

    socket.on('tracktimes', (...data) => {
        maxApi.outlet('tracktimes', ...data);
    });

    socket.on('sequence', (...data) => {
        maxApi.outlet('sequence', ...data);
    });

    // Delay Effects
    socket.on('leftdelaytime', (...data) => {
        maxApi.outlet('leftdelaytime', ...data);
    });

    socket.on('leftfeedback', (...data) => {
        maxApi.outlet('leftfeedback', ...data);
    });

    socket.on('rightdelaytime', (...data) => {
        maxApi.outlet('rightdelaytime', ...data);
    });

    socket.on('rightfeedback', (...data) => {
        maxApi.outlet('rightfeedback', ...data);
    });

    //Reverb Effects
    socket.on('reverbsize', (...data) => {
        maxApi.outlet('reverbsize', ...data);
    });

    socket.on('reverbdecay', (...data) => {
        maxApi.outlet('reverbdecay', ...data);
    });

    socket.on('reverbdamping', (...data) => {
        maxApi.outlet('reverbdamping', ...data);
    });

    socket.on('reverbdiffusion', (...data) => {
        maxApi.outlet('reverbdiffusion', ...data);
    });

    //Track Parameters
    socket.on('panning', (...data) => {
        maxApi.outlet('panning', ...data);
    });

    socket.on('mode', (...data) => {
        maxApi.outlet('mode', ...data);
    });

    socket.on('pitch', (...data) => {
        maxApi.outlet('pitch', ...data);
    });

    socket.on('speed', (...data) => {
        maxApi.outlet('speed', ...data);
    });

    socket.on('stretchstate', (...data) => {
        maxApi.outlet('stretchstate', ...data);
    });

    socket.on('bpm', (...data) => {
        maxApi.outlet('bpm', ...data);
    });

    socket.on('delay', (...data) => {
        maxApi.outlet('delay', ...data);
    });

    socket.on('reverb', (...data) => {
        maxApi.outlet('reverb', ...data);
    });

    socket.on('sample', (...data) => {
        maxApi.outlet('sample', ...data);
    });

    //Change all BPM values
    socket.on('globalbpm', (...data) => {
        maxApi.outlet('globalbpm', ...data);
    });

});

maxApi.addHandler('disconnect', () => {
    socket.close();
});

const handlers = {

    tracktimes: (...data) => {
        socket.emit('tracktimes', ...data);
    },

    sequence: (...data) => {
        socket.emit('sequence', ...data);
    },

    message: (...msg) => {
        socket.emit('message', ...msg);
    },

    sendinfo: (...data) => {
        socket.emit('sendinfo', ...data)
    },

    globalbpm: (...data) => {
        socket.emit('globalbpm', ...data);
    }

};

const effectHandlers = {

    leftdelaytime: (...data) => {
        socket.emit('leftdelaytime', ...data);
    },

    leftfeedback: (...data) => {
        socket.emit('leftfeedback', ...data);
    },

    rightdelaytime: (...data) => {
        socket.emit('rightdelaytime', ...data);
    },

    rightfeedback: (...data) => {
        socket.emit('rightfeedback', ...data);
    },

    reverbsize: (...data) => {
        socket.emit('reverbsize', ...data);
    },

    reverbdecay: (...data) => {
        socket.emit('reverbdecay', ...data);
    },

    reverbdamping: (...data) => {
        socket.emit('reverbdamping', ...data);
    },

    reverbdiffusion: (...data) => {
        socket.emit('reverbdiffusion', ...data);
    },
};

const trackHandlers = {

    panning: (...data) => {
        socket.emit('panning', ...data);
    },

    mode: (...data) => {
        socket.emit('mode', ...data);
    },

    pitch: (...data) => {
        socket.emit('pitch', ...data);
    },

    speed: (...data) => {
        socket.emit('speed', ...data);
    },

    stretchstate: (...data) => {
        socket.emit('stretchstate', ...data);
    },

    bpm: (...data) => {
        socket.emit('bpm', ...data);
    },

    delay: (...data) => {
        socket.emit('delay', ...data);
    },

    reverb: (...data) => {
        socket.emit('reverb', ...data);
    },

    sample: (...data) => {
        socket.emit('sample', ...data);
    }

};

maxApi.addHandlers(handlers);
maxApi.addHandlers(effectHandlers);
maxApi.addHandlers(trackHandlers);

// maxApi.addHandler('message', (...msg) => {
//     socket.emit('message', ...msg);
// });

// //Sample start and end times
// maxApi.addHandler('trackstate', (...data) => {
//     socket.emit('trackstate', ...data);
// });