const WebSocket = require('ws')
 
const port = 8080

const wss = new WebSocket.Server({ port: port })
 
console.log(`Server listening on port ${port}`);
wss.on('connection', ws => {
  ws.on('message', message => {
    console.log(`Received message => ${message}`)
    ws.send(`Server echoes back: ${message}!`)
  })
  ws.send('Hello! Message From Server!!')
})


