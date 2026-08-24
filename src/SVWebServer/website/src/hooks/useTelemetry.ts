import { useEffect, useMemo, useRef, type Dispatch, type SetStateAction } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import type { DataParams, NavScheme, RootState, Signal, SignalDataBatch, SignalsMap } from '../types/telemetry';
import { setDataParams, setSignalsFromServer, updateFromServer } from '../store/actions';

function buildNavScheme(signs: SignalsMap): NavScheme {
  const navScheme: NavScheme = [];

  for (const k in signs) {
    const s = signs[k];
    let item = navScheme.find((it) => s.module === it.submenu);

    if (!item) {
      item = {
        submenu: s.module,
        isShow: true,
        isActive: true,
        items: [],
      };
      navScheme.push(item);
    }

    item.items.push(s.name);
  }

  return navScheme;
}

function mergeNavScheme(prev: NavScheme, next: NavScheme): NavScheme {
  return next.map((item) => {
    const old = prev.find((p) => p.submenu === item.submenu);
    if (!old) {
      return item;
    }
    return {
      ...item,
      isShow: old.isShow,
      isActive: old.isActive,
    };
  });
}

async function fetchJson<T>(url: string): Promise<T> {
  const response = await fetch(url);
  if (!response.ok) {
    throw new Error(`${url} -> ${response.status}`);
  }
  return response.json() as Promise<T>;
}

export function useTelemetry(setNavScheme: Dispatch<SetStateAction<NavScheme>>) {
  const dispatch = useDispatch();
  const signals = useSelector((state: RootState) => state.signals);
  const dataParams = useSelector((state: RootState) => state.dataParams);
  const signalsRef = useRef(signals);
  const navSchemeRef = useRef<NavScheme>([]);
  const tickRef = useRef(0);
  const timerRef = useRef<ReturnType<typeof setTimeout> | null>(null);

  signalsRef.current = signals;

  const signalsLayoutKey = useMemo(
    () =>
      Object.values(signals)
        .map((s) => `${s.name}\0${s.module}`)
        .sort()
        .join('\n'),
    [signals],
  );

  useEffect(() => {
    if (!signalsLayoutKey) {
      return;
    }

    const scheme = mergeNavScheme(navSchemeRef.current, buildNavScheme(signalsRef.current));
    navSchemeRef.current = scheme;
    setNavScheme(scheme);
  }, [signalsLayoutKey, setNavScheme]);

  useEffect(() => {
    fetchJson<Record<string, Omit<Signal, 'isBuffEna' | 'buffVals'>>>('/api/allSignals')
      .then((signs) => {
        const prepared: SignalsMap = {};
        for (const k in signs) {
          prepared[k] = {
            ...signs[k],
            isBuffEna: false,
            buffVals: [],
          };
        }
        dispatch(setSignalsFromServer(prepared));
      })
      .catch(() => console.log('api/allSignals error'));

    fetchJson<DataParams>('/api/dataParams')
      .then((params) => {
        dispatch(setDataParams(params));
      })
      .catch(() => console.log('api/dataParams error'));
  }, [dispatch]);

  useEffect(() => {
    if (!dataParams.cycleTimeMs) {
      return;
    }

    const updateFunc = () => {
      const tmStart = Date.now();
      const signs = signalsRef.current;
      const snames = Object.values(signs)
        .filter((it) => it.isBuffEna)
        .map((it) => it.name + it.module);

      if (snames.length > 0) {
        let req = '/api/lastSignalData?';
        for (let i = 0; i < snames.length; ++i) {
          req += 'sname' + i + '=' + encodeURIComponent(snames[i]);
          if (i < snames.length - 1) {
            req += '&';
          }
        }

        fetchJson<SignalDataBatch>(req)
          .then((buffVals) => {
            if (Object.keys(buffVals).length > 0) {
              dispatch(updateFromServer(buffVals));
            }
          })
          .catch(() => console.log('api/lastSignalData error'));
      }

      if (tickRef.current % 10 === 0 && navSchemeRef.current.length > 0) {
        fetchJson<Record<string, { isActive: boolean }>>('/api/allModules')
          .then((modState) => {
            const navScheme = navSchemeRef.current.map((item) => ({
              ...item,
              isActive: modState[item.submenu]?.isActive ?? item.isActive,
            }));
            navSchemeRef.current = navScheme;
            setNavScheme(navScheme);
          })
          .catch(() => {
            console.log('api/allModules error');
            const navScheme = navSchemeRef.current.map((it) => ({
              ...it,
              isActive: false,
            }));
            navSchemeRef.current = navScheme;
            setNavScheme(navScheme);
          });
      }

      tickRef.current += 1;

      let tout = Date.now() - tmStart;
      tout = Math.max(0, dataParams.cycleTimeMs * dataParams.packetSize * 0.9 - tout);
      timerRef.current = setTimeout(updateFunc, tout);
    };

    updateFunc();

    return () => {
      if (timerRef.current !== null) {
        clearTimeout(timerRef.current);
      }
    };
  }, [dataParams.cycleTimeMs, dataParams.packetSize, dispatch, setNavScheme]);
}
