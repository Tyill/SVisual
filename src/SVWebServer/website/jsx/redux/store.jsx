/* eslint-disable no-unused-vars */

import { changeSignalData } from "./reducers.jsx";
import { createStore, combineReducers } from 'react-redux'

const initialState = {
  signals: [
  {
    // name : "s1",
    // module : "myModule",
    // type : "int",
    // group : "myGroup",
    // comment : "int",
    // buffVals : [
    //   {
    //     time : 0,
    //     vals : []
    //   }
    // ]    
  }
]
};

export
const Store = createStore(
  combineReducers({changeSignalData}),
  initialState
);