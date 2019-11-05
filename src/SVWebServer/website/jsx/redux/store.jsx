/* eslint-disable no-unused-vars */
// @flow
import ComboReducer from "./reducers.jsx";
import { createStore } from 'redux'

/*::
export
type signalType = { name : string,   
                    module : string, 
                    type : number,   // 0 - bool, 1 - int, 2 - float 
                    group : string,  
                    comment : string,
                    isBuffEna : boolean,
                    buffVals : Array<{ beginTime : number, 
                                       vals : Array<number> }>
}

export
type configType = { backgroundColor : string,
}

export
type dataParamsType = { packetSize : number,
                        cycleTimeMs : number,
}
*/

const initialState = {
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