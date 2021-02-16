const WebSocket = require('ws')
const port = 8080
const wss = new WebSocket.Server({port: port}, {clientTracking: true})

 
console.log(`Server listening on port ${port}`);


wss.on('connection', ws => {
    
  ws.on('message', message => {
      
    //Make sure first message is always uuid
    if(!ws.id){
        var msg = JSON.parse(message);
        
        ws.id = msg[1];
    }
    else{
        
        var msg = JSON.parse(message);
        
        var dimensions = [msg.length, msg[1].length]
        
        if(msg[0] == "id"){
            var clientID = msg[1];
        }
        
        switch(msg[2][0]){
            case "channeldata":
                var channelOne = msg[2][1];
                var channelTwo = msg[2][2];
                var channelThree = msg[2][3];
                var channelFour = msg[2][4];
                var channelFive = msg[2][5];
                var channelSix = msg[2][6];
                var channelSeven = msg[2][7];
                var channelEight = msg[2][8];
                break;
            case "globaltempo":
                break;
            case "channelvolume":
                break;
        }
        
        console.log(`Received message from ${clientID}`);
        
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


