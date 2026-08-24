import { type Reducer } from 'redux';
import type { Config, DataParams, RootState, SignalsMap } from '../types/telemetry';
import {
  CHANGE_CONFIG,
  SET_DATA_PARAMS,
  SET_SIGNALS_FROM_SERVER,
  SIGNAL_BUFFER_ENABLE,
  UPDATE_FROM_SERVER,
  type AppAction,
} from './actions';

function signals(curSignals: SignalsMap = {}, action: AppAction): SignalsMap {
  switch (action.type) {
    case UPDATE_FROM_SERVER: {
      const signalsCpy = { ...curSignals };
      for (const k in action.newSignData) {
        signalsCpy[k].buffVals.push(action.newSignData[k]);
      }
      return signalsCpy;
    }
    case SET_SIGNALS_FROM_SERVER:
      return action.signals;
    case SIGNAL_BUFFER_ENABLE: {
      const signalsCpy = { ...curSignals };
      const sname = action.sname;
      signalsCpy[sname].isBuffEna = action.set;
      if (!action.set) {
        signalsCpy[sname].buffVals = [];
      }
      return signalsCpy;
    }
    default:
      return curSignals;
  }
}

function dataParams(
  curParams: DataParams = { packetSize: 10, cycleTimeMs: 100 },
  action: AppAction,
): DataParams {
  switch (action.type) {
    case SET_DATA_PARAMS:
      return action.dataParams;
    default:
      return curParams;
  }
}

function config(curConfig: Config = { backgroundColor: 'white' }, action: AppAction): Config {
  switch (action.type) {
    case CHANGE_CONFIG:
      return action.config;
    default:
      return curConfig;
  }
}

const initialState: RootState = {
  config: { backgroundColor: 'white' },
  dataParams: { packetSize: 10, cycleTimeMs: 100 },
  signals: {},
};

const rootReducer: Reducer<RootState, AppAction> = (
  state = initialState,
  action,
): RootState => ({
  config: config(state.config, action),
  signals: signals(state.signals, action),
  dataParams: dataParams(state.dataParams, action),
});

export default rootReducer;
