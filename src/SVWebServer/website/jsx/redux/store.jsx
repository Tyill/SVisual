/* eslint-disable no-unused-vars */
// @flow
import ComboReducer from "./reducers.jsx";
import { createStore } from 'redux'

/*::

export 
type snameType = string // name + module

export 
type signalDataType = Array<{ beginTime : number,
                              vals : Array<number> }>

export
type signalType = { name : string,   
                    module : string, 
                    type : number,   // 0 - bool, 1 - int, 2 - float 
                    group : string,  
                    comment : string,
                    isBuffEna : boolean,
                    buffVals : signalDataType,
}

export
type configType = { backgroundColor : string,
}

export
type dataParamsType = { packetSize : number,
                        cycleTimeMs : number,
}

export
type storeType = { config : configType,
                   dataParams : dataParamsType,
                   signals : {sname : signalType},
}
*/

const initialState /*:: : storeType */ = {
  config : {
    backgroundColor : "white",
  },
  dataParams : { 
    packetSize : 10,
    cycleTimeMs : 100,
  },  
  signals: { 
    // name + module : signalType,
  },
};

const Store = createStore(ComboReducer, initialState);

export default Store; 