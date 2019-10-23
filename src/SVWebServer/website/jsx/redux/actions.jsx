/* eslint-disable no-unused-vars */

export
const UPDATE_FROM_SERVER = "UPDATE_FROM_SERVER";

export
function updateFromServer(newSignData){
    
    return { type : UPDATE_FROM_SERVER,
             newSignData,
           };
}