const maxApi = require("max-api");
const io = require('socket.io-client');

let socket;

maxApi.addHandler('connect', (url) => {
    socket = io(url);

    //Incoming socket messages
    socket.on('message', (...msg) => {
        maxApi.outlet("message", ...msg);
    });

    socket.on('trackstate', (...data) => {
        maxApi.outlet('trackstate', ...data);
    });

    socket.on('tracktimes', (...data) => {
        maxApi.outlet('tracktimes', ...data);
    });

    socket.on('sequence', (...data) => {
        maxApi.outlet('sequence', ...data);
    });

    socket.on('sendinfo', (...data) => {
        maxApi.outlet('sendinfo', ...data);
    });

    socket.on('globalbpm', (...data) => {
        maxApi.outlet('globalbpm', ...data);
    });

});

maxApi.addHandler('disconnect', () => {
    socket.close();
});

const handlers = {

    trackstate: (...data) => {
        socket.emit('trackstate', ...data);
    },

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

maxApi.addHandlers(handlers);

// maxApi.addHandler('message', (...msg) => {
//     socket.emit('message', ...msg);
// });

// //Sample start and end times
// maxApi.addHandler('trackstate', (...data) => {
//     socket.emit('trackstate', ...data);
// });