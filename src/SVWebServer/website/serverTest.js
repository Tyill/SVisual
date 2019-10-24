
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

    let signals = {
        s1oneModule : {
            name : "s1",
            module: "oneModule",
            type : 1,
            group: "oneGroup",
            comment: "",
            isActive: true, 
        },
        s2twoModule : {
            name : "s2",
            module: "twoModule",
            type : 2,
            group: "twoGroup",
            comment: "",
            isActive: true, 
        },
        s3threeModule : {
            name : "s3",
            module: "threeModule",
            type : 0,
            group: "threeGroup",
            comment: "",
            isActive: true, 
        },        
    };
   
    response.send(signals);
});

let counterMain = { s1oneModule : 0,
                    s2twoModule : 0,
                    s3threeModule : 0,
                  };

app.get("/api/lastSignalData", function(request, response){
    
    let name = request.query.name,
        module = request.query.module,
        counter = counterMain[name + module];
   
    let signData = { 
        beginTime : Date.now(),
        vals : [],             
    };

    for (let i = 0; i < dataParams.packetSize; ++i){
        signData.vals.push(counter + i);
    }

    counter += dataParams.packetSize;

    if (counter >= 300)
      counter = -300; 
   
    counterMain[name + module] = counter;

    response.send(signData);
});

app.listen(3000);