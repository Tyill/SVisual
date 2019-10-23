/* eslint-disable no-unused-vars */

export const SET_SIGNALS_FROM_SERVER = "SET_SIGNALS_FROM_SERVER";
export const UPDATE_FROM_SERVER = "UPDATE_FROM_SERVER";
export const SET_DATA_PARAMS = "SET_DATA_PARAMS";

export
function setSignalsFromServer(dispatch){
    
    return function(signals){

      dispatch({ type : SET_SIGNALS_FROM_SERVER,
                 signals,
               });
    }
}

export
function updateFromServer(dispatch){
    
    return function(newSignData){

      dispatch({ type : UPDATE_FROM_SERVER,
                 newSignData,
               });
    }
}

export
function setDataParams(dispatch){
    
    return function(dataParams){
    
      dispatch({ type : SET_DATA_PARAMS,
                 dataParams,
               });
    }
}