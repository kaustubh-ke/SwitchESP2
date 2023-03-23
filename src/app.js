import express from 'express';
import http from 'http';
import { Server } from 'socket.io';

const PORT = process.env.PORT || 4001;

const app = express();
const httpServer = http.createServer(app);
const io = new Server(httpServer, { cors: { origin: '*' } });

app.use(express.static('src/ui'));

let buttonState1 = false;
let buttonState2 = false;

io.on('connection', socket => {
    console.log('New Connection');

    io.to(socket.id).emit('buttonState1', buttonState1);

    socket.on('disconnect', () => {
        console.log('Disconnected');
    });

    socket.on('buttonState1', value => {
        console.log('buttonState1:', value);
        buttonState1 = value;
        socket.broadcast.emit('buttonState1', value);
    });
    socket.on('buttonState2', value => {
        console.log('buttonState2:', value);
        buttonState2 = value;
        socket.broadcast.emit('buttonState2', value);
    });
});

httpServer.listen(PORT, () => {
    console.log('Running on : ', httpServer.address());
});
