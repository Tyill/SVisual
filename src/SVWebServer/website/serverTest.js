
const express = require("express");

const app = express();

const dataParams = {
    packetSize : 10,
    cycleTimeMs : 100, 
};

// app.use(express.static(__dirname + "/js"));

// app.use(function(request, response){
    
//     let path = request.path;
//     if (path == '/') path = "/index.html";

//     response.sendFile(__dirname + path);
// });

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

app.get("/api/allModules", function(request, response){

    let modules = {
        oneModule : {
            isActive: true, 
        },
        twoModule : {
            isActive: true, 
        },
        threeModule : {
            isActive: true, 
        },        
    };
   
    response.send(modules);
});

let counterMain = { s1oneModule : 0,
                    s2twoModule : 0,
                    s3threeModule : 0,
                  };

app.get("/api/lastSignalData", function(request, response){
   
    let sname = request.query.sname0,
        result = {}, 
        cnt = 0;
    while(sname){
    
      let counter = counterMain[sname];
   
      let signData = { 
          beginTime : Date.now(),
          vals : [],             
      };

      if ((sname) !== "s3threeModule"){   // int, float

        for (let i = 0; i < dataParams.packetSize; ++i)
          signData.vals.push(counter + i);
        
        counter += dataParams.packetSize;
  
        if (counter >= 100)
          counter = -100; 
       
        counterMain[sname] = counter;
      }
      else{                                       // bool
            
        counter++;
  
        let v = ((counter % 2) === 0);
  
        for (let i = 0; i < dataParams.packetSize; ++i){
          signData.vals.push(v);
        }
  
        counterMain[sname] = counter;
      }


      result[sname] = signData;

      ++cnt;
      sname =  request.query["sname" + cnt];
    }

    response.send(result);
});

app.listen(3000);