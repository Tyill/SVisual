import type { Config, DataParams, SignalDataBatch, SignalsMap } from '../types/telemetry';

export const SET_SIGNALS_FROM_SERVER = 'SET_SIGNALS_FROM_SERVER';
export const UPDATE_FROM_SERVER = 'UPDATE_FROM_SERVER';
export const SET_DATA_PARAMS = 'SET_DATA_PARAMS';
export const SIGNAL_BUFFER_ENABLE = 'SIGNAL_BUFFER_ENABLE';
export const CHANGE_CONFIG = 'CHANGE_CONFIG';

export type AppAction =
  | { type: typeof SET_SIGNALS_FROM_SERVER; signals: SignalsMap }
  | { type: typeof UPDATE_FROM_SERVER; newSignData: SignalDataBatch }
  | { type: typeof SET_DATA_PARAMS; dataParams: DataParams }
  | { type: typeof SIGNAL_BUFFER_ENABLE; sname: string; set: boolean }
  | { type: typeof CHANGE_CONFIG; config: Config };

export function changeConfig(config: Config): AppAction {
  return { type: CHANGE_CONFIG, config };
}

export function setSignalsFromServer(signals: SignalsMap): AppAction {
  return { type: SET_SIGNALS_FROM_SERVER, signals };
}

export function updateFromServer(newSignData: SignalDataBatch): AppAction {
  return { type: UPDATE_FROM_SERVER, newSignData };
}

export function setDataParams(dataParams: DataParams): AppAction {
  return { type: SET_DATA_PARAMS, dataParams };
}

export function signalBufferEnable(sname: string, set: boolean): AppAction {
  return { type: SIGNAL_BUFFER_ENABLE, sname, set };
}
