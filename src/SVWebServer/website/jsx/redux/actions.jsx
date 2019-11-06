/* eslint-disable no-unused-vars */
// @flow
export const SET_SIGNALS_FROM_SERVER = "SET_SIGNALS_FROM_SERVER";
export const UPDATE_FROM_SERVER = "UPDATE_FROM_SERVER";
export const SET_DATA_PARAMS = "SET_DATA_PARAMS";
export const SIGNAL_BUFFER_ENABLE = "SIGNAL_BUFFER_ENABLE";
export const CHANGE_CONFIG = "CHANGE_CONFIG";

/*::
import type {signalType, configType, dataParamsType, signalDataType} from "./store.jsx";

export 
type changeConfigType = (configType) => void

export 
type setSignalsFromServerType = ({obj : signalType}) => void

export 
type updateFromServerType = ({obj : signalDataType}) => void

export 
type setDataParamsType = (dataParamsType) => void

export
type signalBufferEnableType = (sname : string, set : boolean) => void
*/

export
function changeConfig(dispatch /*:: : any */) /*:: : changeConfigType */{
    
    return function(config){

      dispatch({ type : CHANGE_CONFIG,
                 config,
               });
    }
}

export
function setSignalsFromServer(dispatch /*:: : any */) /*:: : setSignalsFromServerType */{
    
    return function(signals){

      dispatch({ type : SET_SIGNALS_FROM_SERVER,
                 signals,
               });
    }
}

export
function updateFromServer(dispatch /*:: : any */) /*:: : updateFromServerType */{
    
    return function(newSignData){

      dispatch({ type : UPDATE_FROM_SERVER,
                 newSignData,
               });
    }
}

export
function setDataParams(dispatch /*:: : any */) /*:: : setDataParamsType */{
    
    return function(dataParams){
    
      dispatch({ type : SET_DATA_PARAMS,
                 dataParams,
               });
    }
}

export
function signalBufferEnable(dispatch /*:: : any */) /*:: : signalBufferEnableType */{
    
    return function(sname, set){
    
      dispatch({ type : SIGNAL_BUFFER_ENABLE,
                 sname,
                 set,
               });
    }
}