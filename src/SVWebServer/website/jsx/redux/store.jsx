/* eslint-disable no-unused-vars */

import CombReducer from "./reducers.jsx";
import { createStore } from 'redux'

const initialState = {
  signals: { 
    // name + module : {
    //                  name : "s1",
    //                  module : "module",
    //                  type : "int",
    //                  group : "group",
    //                  comment : "",
    //                  buffVals : [
    //                    {
    //                      time : 0,
    //                      vals : []
    //                    }
    //                  ]    
    //                 }
  },
};

const Store = createStore(CombReducer, initialState);

export default Store;