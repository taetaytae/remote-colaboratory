//Import libraries and store inside variables
var express = require('express');
var socket = require('socket.io');

//Initiate the server on port 8080
var app = express();
var server = app.listen(8080, () => {
    console.log('Server listening on port 8080');
});

module.parent

//Connect the socket to this particular server
var io = socket(server);

//Handle socket events
io.on('connection', (socket) => {
    console.log("new connection: " + socket.id);


    socket.on('message', (...msg) => {
        socket.broadcast.emit('message', ...msg);
    });

    //Sample start and end times
    socket.on('tracktimes', (...data) => {
        console.log("Sample times from: " + socket.id);
        socket.broadcast.emit('tracktimes', ...data);
    });

    //Send delay info
    socket.on('leftdelaytime', (...data) => {
        console.log("Left delay time from: " + socket.id);
        socket.broadcast.emit('leftdelaytime', ...data);
    });

    socket.on('leftfeedback', (...data) => {
        console.log("Left feedback from: " + socket.id);
        socket.broadcast.emit('leftfeedback', ...data);
    });

    socket.on('rightdelaytime', (...data) => {
        console.log("Right delay time from: " + socket.id);
        socket.broadcast.emit('rightdelaytime', ...data);
    });

    socket.on('rightfeedback', (...data) => {
        console.log("Right feedback from: " + socket.id);
        socket.broadcast.emit('rightfeedback', ...data);
    });

    //Send reverb info
    socket.on('reverbsize', (...data) => {
        console.log("Reverb size time from: " + socket.id);
        socket.broadcast.emit('reverbsize', ...data);
    });

    socket.on('reverbdecay', (...data) => {
        console.log("Reverb decay from: " + socket.id);
        socket.broadcast.emit('reverbdecay', ...data);
    });

    socket.on('reverbdamping', (...data) => {
        console.log("Reverb damping time from: " + socket.id);
        socket.broadcast.emit('reverbdamping', ...data);
    });

    socket.on('reverbdiffusion', (...data) => {
        console.log("Reverb diffusion from: " + socket.id);
        socket.broadcast.emit('reverbdiffusion', ...data);
    });

    //Track parameter data
    socket.on('panning', (...data) => {
        console.log("Pannning data from: " + socket.id);
        socket.broadcast.emit('panning', ...data);
    });

    socket.on('mode', (...data) => {
        console.log("Track mode change from: " + socket.id);
        socket.broadcast.emit('mode', ...data);
    });

    socket.on('pitch', (...data) => {
        console.log("Track pitch data from: " + socket.id);
        socket.broadcast.emit('pitch', ...data);
    });

    socket.on('speed', (...data) => {
        console.log("Track speed data from: " + socket.id);
        socket.broadcast.emit('speed', ...data);
    });

    socket.on('stretchstate', (...data) => {
        console.log("Track tretch state change from: " + socket.id);
        socket.broadcast.emit('stretchstate', ...data);
    });

    socket.on('bpm', (...data) => {
        console.log("BPM data from: " + socket.id);
        socket.broadcast.emit('bpm', ...data);
    });

    socket.on('delay', (...data) => {
        console.log("Track delay data from: " + socket.id);
        socket.broadcast.emit('delay', ...data);
    });

    socket.on('reverb', (...data) => {
        console.log("Track reverb data from: " + socket.id);
        socket.broadcast.emit('reverb', ...data);
    });

    socket.on('sample', (...data) => {
        console.log("Sample change data from: " + socket.id);
        socket.broadcast.emit('sample', ...data);
    });


    //Sequence data
    socket.on('sequence', (...data) => {
        console.log("Sequence data from: " + socket.id);
        socket.broadcast.emit('sequence', ...data);
    });

    //Global bpm number
    socket.on('globalbpm', (...data) => {
        console.log("Global BPM data from: " + socket.id);
        socket.broadcast.emit('globalbpm', ...data);
    });

    socket.on('disconnect', function(){
        console.log("user: " + socket.id + " disconnected");
    });

});