
const express = require("express");

const app = express();

const dataParams = {
    packetSize : 10,
    cycleTimeMs : 100, 
};

app.get("/api/dataParams", function(request, response){
      
    response.send(dataParams);
});

app.get("/api/allSignals", function(request, response){

    let signals = [
        {name : "s1",
         type : "int",
         module: "oneModule",
         group: "oneGroup",
         isActive: true, 
        },   
        {name : "s2",
         type : "float",
         module: "twoModule",
         group: "twoGroup",
         isActive: true, 
        },  
        {name : "s3",
         type : "bool",
         module: "threeModule",
         group: "threeGroup",
         isActive: true, 
        },        
    ];
   
    response.send(signals);
});

let counter = 0;

app.get("/api/signalData", function(request, response){
    
    //let nm = request.query.name;

   // console.log(nm);

    let signData = { 
        beginTime : Date.now(),
        vals : [],             
    };

    for (let i = 0; i < dataParams.packetSize; ++i){
        signData.vals.push(counter + i);
    }

    counter += dataParams.packetSize;

    if (counter >= 100)
      counter = -100; 
   
    response.send(signData);
});

app.listen(3000);