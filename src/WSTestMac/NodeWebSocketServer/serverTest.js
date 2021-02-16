const WebSocket = require('ws')
const port = 8080
const wss = new WebSocket.Server({port: port}, {clientTracking: true})

 
console.log(`Server listening on port ${port}`);


wss.on('connection', ws => {
    
  ws.on('message', message => {
      
    //Make sure first message is always uuid
    if(!ws.id){
        ws.id = message;
    }
    else{
        
        var msg = JSON.parse(message);
        
        var clientID = msg[0];
        var messageText = msg[1];
        
        console.log(`Received message from ${clientID}`);
        console.log(messageText);
        
        wss.clients.forEach(function each(client) {
            //console.log('Client.ID: ' + client.id);
            
            //Send message to all connected clients except the one that is sending it
            if(client.id != ws.id && client.readyState === WebSocket.OPEN){
                client.send(message);
            }
        });
    }    
      
  })
    
  //ws.send('Hello! Message From Server!!')
  console.log(`Current number of clients: ${wss.clients.size}`)
    
  ws.on('close', function() {
      console.log(`connection closed`)
      console.log(`Current number of clients: ${wss.clients.size}`)
  });
    
})


