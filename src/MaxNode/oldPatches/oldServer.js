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

    //Track info
    socket.on('trackstate', (...data) => {
        console.log("Track data from: " + socket.id);
        socket.broadcast.emit('trackstate', ...data);
    });

    //Sample start and end times
    socket.on('tracktimes', (...data) => {
        console.log("Sample times from: " + socket.id);
        socket.broadcast.emit('tracktimes', ...data);
    });

    //Send effect info
    socket.on('sendinfo', (...data) => {
        console.log("Send data from: " + socket.id);
        socket.broadcast.emit('sendinfo', ...data);
    });

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