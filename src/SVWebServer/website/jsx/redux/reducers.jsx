/* eslint-disable no-unused-vars */

import { UPDATE_FROM_SERVER,
         SET_DATA_PARAMS,
         SET_SIGNALS_FROM_SERVER,
         SIGNAL_BUFFER_ENABLE } from "./actions.jsx";
import { combineReducers } from 'redux'
import _ from "lodash";

function signals(curSignals = {}, action){

  switch (action.type) {
    case UPDATE_FROM_SERVER:{     
     
      let signalsCpy = _.cloneDeep(curSignals);

      for (let k in action.newSignData){

        signalsCpy[k].buffVals.push(action.newSignData[k]);
      }      

      return signalsCpy;
    }

    case SET_SIGNALS_FROM_SERVER:     
      return action.signals;

    case SIGNAL_BUFFER_ENABLE:{

      let signalsCpy = _.cloneDeep(curSignals);

      signalsCpy[action.name + action.module].isBuffEna = true;
      
      return signalsCpy;
    }

     
    default:
      return curSignals;
 }
}

function dataParams(curParams, action){

  switch (action.type) {
    case SET_DATA_PARAMS:     
      return action.dataParams;
    
    default: 
      return action.dataParams ? action.dataParams : { 
          packetSize : 10,
          cycleTimeMs : 100,
        };
 }
}

const ComboReducer = combineReducers({signals, dataParams});

export default ComboReducer;