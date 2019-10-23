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
    //                  type : "int",
    //                  group : "group",
    //                  comment : "",
    //                  isBuffEna : false, 
    //                  buffVals : [
    //                    {
    //                      time : 0,
    //                      vals : []
    //                    }
    //                  ]    
    //                 }
  },
};

const Store = createStore(ComboReducer, initialState);

export default Store;