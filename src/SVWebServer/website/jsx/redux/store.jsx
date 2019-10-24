/* eslint-disable no-unused-vars */

import ComboReducer from "./reducers.jsx";
import { createStore } from 'redux'

const initialState = {
  dataParams : { 
    packetSize : 10,
    cycleTimeMs : 100,
  },
  signals: { 
    // name + module : {
    //                  name : "s1",
    //                  module : "module",
    //                  type : 0,   // 0 - bool, 1 - int, 2 - float 
    //                  group : "group",
    //                  comment : "",
    //                  isBuffEna : false, 
    //                  buffVals : [
    //                    {
    //                      beginTime : 0,
    //                      vals : []
    //                    }
    //                  ]    
    //                 }
  },
};

const Store = createStore(ComboReducer, initialState);

export default Store;