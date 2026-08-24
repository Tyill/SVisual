import type { Signal, SignalsMap, Sname } from '../types/telemetry';

export const BOOL_TRACK_DIST = 15;
export const BOOL_LABEL_HEIGHT = 11;

export function isBoolSignal(signal: Signal | undefined): boolean {
  return signal?.type === 0;
}

export function getBoolKeys(signalOrder: Sname[], signals: SignalsMap): Sname[] {
  return signalOrder.filter((k) => isBoolSignal(signals[k]));
}

export function getAnalogKeys(signalOrder: Sname[], signals: SignalsMap): Sname[] {
  return signalOrder.filter((k) => signals[k] && !isBoolSignal(signals[k]));
}

/** 0 = lowest bool track (first added), like Qt signBoolCnt when signBoolOnTop is false. */
export function boolTrackIndex(boolKeys: Sname[], key: Sname): number {
  const idx = boolKeys.indexOf(key);
  return idx >= 0 ? idx : 0;
}
