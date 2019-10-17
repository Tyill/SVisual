
const express = require("express");

const app = express();

// app.get("/", function(request, response){
        
//     response.send("<h2>Привет Express!</h2>");
// });

app.use(express.static('js'));
app.use(express.static('.'));

app.listen(3000);