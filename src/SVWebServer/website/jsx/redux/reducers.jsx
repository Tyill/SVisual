/* eslint-disable no-unused-vars */

import { UPDATE_FROM_SERVER } from "./actions.jsx";
import { combineReducers } from 'redux'
import _ from "lodash";

function signals(state = {}, action){

  switch (action) {
    case UPDATE_FROM_SERVER:      
      return _.cloneDeep(state);

    default:
      return state;
 }
}

const CombReducer = combineReducers({signals});

export default CombReducer;