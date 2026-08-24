import { createStore } from 'redux';
import type { RootState } from '../types/telemetry';
import rootReducer from './reducers';

const initialState: RootState = {
  config: {
    backgroundColor: 'white',
  },
  dataParams: {
    packetSize: 10,
    cycleTimeMs: 100,
  },
  signals: {},
};

const store = createStore(rootReducer, initialState);

export default store;
