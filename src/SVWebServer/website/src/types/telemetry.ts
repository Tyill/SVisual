export type Sname = string;

export type SignalDataPoint = {
  beginTime: number;
  vals: Array<number | boolean>;
};

export type Signal = {
  name: string;
  module: string;
  type: number;
  group: string;
  comment: string;
  isActive: boolean;
  isBuffEna: boolean;
  buffVals: SignalDataPoint[];
};

export type SignalsMap = Record<Sname, Signal>;

export type Config = {
  backgroundColor: string;
};

export type DataParams = {
  packetSize: number;
  cycleTimeMs: number;
};

export type NavSchemeItem = {
  submenu: string;
  isShow: boolean;
  isActive: boolean;
  items: string[];
};

export type NavScheme = NavSchemeItem[];

export type ModulesState = Record<string, { isActive: boolean }>;

export type SignalDataBatch = Record<Sname, SignalDataPoint>;

export type RootState = {
  config: Config;
  dataParams: DataParams;
  signals: SignalsMap;
};

export type TmInterval = { beginMs: number; endMs: number };
export type ValInterval = { begin: number; end: number };

export type AxisParams = {
  valOffsPos: number;
  valDashStep: number;
  tmOffsPos: number;
  tmDashStep: number;
  minValDashStep: number;
  maxValDashStep: number;
};

export type SignColorParams = {
  lineWidth: number;
  transparent: number;
  color: string;
};

export type SignColorParamsMap = Record<Sname, SignColorParams>;
