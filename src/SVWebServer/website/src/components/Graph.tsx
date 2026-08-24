import { useCallback, useEffect, useLayoutEffect, useMemo, useRef, useState } from 'react';
import { Container, Row, Col, Button } from 'react-bootstrap';
import AxisValue from './AxisValue';
import AxisTime from './AxisTime';
import Plot from './Plot';
import '../styles/fontello.css';
import type {
  AxisParams,
  DataParams,
  SignColorParamsMap,
  SignalsMap,
  Sname,
  TmInterval,
  ValInterval,
} from '../types/telemetry';
import {
  BOOL_TRACK_DIST,
  boolTrackIndex,
  getAnalogKeys,
  getBoolKeys,
  isBoolSignal,
} from '../utils/graphSignals';

const GRAPH_CHROME_BG = '#eef2f6';
const GRAPH_BORDER = '1px solid #d4dbe3';

type GraphProps = {
  iGraph: number;
  zIndex: React.MutableRefObject<number>;
  dataParams: DataParams;
  signals: SignalsMap;
  signalOrder: Sname[];
  backgroundColor: string;
  onAddSignal: (iGraph: number, sname: Sname) => void;
  onDelSignal: (iGraph: number, sname: Sname) => void;
  onCloseGraph: (iGraph: number) => void;
};

const defaultAxisParams: AxisParams = {
  valOffsPos: 0,
  valDashStep: 100,
  tmOffsPos: 0,
  tmDashStep: 100,
  minValDashStep: 50,
  maxValDashStep: 100,
};

export default function Graph({
  iGraph,
  zIndex,
  dataParams,
  signals,
  signalOrder,
  backgroundColor,
  onAddSignal,
  onDelSignal,
  onCloseGraph,
}: GraphProps) {
  const graphRef = useRef<HTMLDivElement | null>(null);
  const signalsRef = useRef(signals);
  signalsRef.current = signals;
  const signColorParamsRef = useRef<SignColorParamsMap>({});
  const [isPlay, setIsPlay] = useState(true);
  const [isAutoResize, setIsAutoResize] = useState(true);
  const isResizeComponentRef = useRef(false);
  const isResizingRef = useRef(false);
  const isDraggingGraphRef = useRef(false);
  const zIndexLocalRef = useRef(zIndex.current + 1);

  const [tmInterval, setTmInterval] = useState<TmInterval>({
    beginMs: Date.now(),
    endMs: Date.now() + 3.6e4,
  });
  const [valInterval, setValInterval] = useState<ValInterval>({ begin: 0, end: 1000 });
  const tmIntervalRef = useRef(tmInterval);
  const valIntervalRef = useRef(valInterval);
  tmIntervalRef.current = tmInterval;
  valIntervalRef.current = valInterval;
  const [axisParams, setAxisParams] = useState<AxisParams>(defaultAxisParams);
  const [offsPosContainer, setOffsPosContainer] = useState({ left: 0, top: 0 });
  const [csizeContainer, setCsizeContainer] = useState({
    width: 0,
    height: Math.min(
      250,
      document.documentElement ? document.documentElement.clientHeight * 0.65 : 250,
    ),
  });
  const [, forceRender] = useState(0);
  const hasLegend = signalOrder.length > 0;
  const analogLegendKeys = getAnalogKeys(signalOrder, signals);
  const boolLegendKeys = getBoolKeys(signalOrder, signals);

  useEffect(() => {
    zIndex.current += 1;
    zIndexLocalRef.current = zIndex.current + 1;
  }, [zIndex]);

  useEffect(() => {
    if (graphRef.current) {
      setCsizeContainer((prev) => ({ ...prev, width: graphRef.current!.clientWidth }));
    }
  }, [hasLegend]);

  useEffect(() => {
    const onMouseMove = (event: MouseEvent) => {
      if (!isResizingRef.current) {
        return;
      }

      event.preventDefault();
      isResizeComponentRef.current = true;
      const distX = event.movementX;
      const distY = event.movementY;
      setCsizeContainer((prev) => ({
        width: Math.max(150, prev.width + distX),
        height: Math.max(120, prev.height + distY),
      }));
    };

    const stopResize = () => {
      isResizingRef.current = false;
    };

    window.addEventListener('mousemove', onMouseMove);
    window.addEventListener('mouseup', stopResize);
    return () => {
      window.removeEventListener('mousemove', onMouseMove);
      window.removeEventListener('mouseup', stopResize);
    };
  }, []);

  const signalsDataKey = useMemo(() => {
    let key = '';
    for (const k in signals) {
      const sign = signals[k];
      key += `${k}:${sign.buffVals.length}`;
      if (sign.buffVals.length > 0) {
        key += `:${sign.buffVals[sign.buffVals.length - 1].beginTime}`;
      }
      key += '|';
    }
    return key;
  }, [signals]);

  const calcTimeInterval = useCallback((): TmInterval => {
    let minTime = Number.MAX_VALUE;
    let maxTime = -Number.MAX_VALUE;

    for (const k in signalsRef.current) {
      const sign = signalsRef.current[k];
      if (!sign.buffVals.length) {
        continue;
      }
      if (sign.buffVals[0].beginTime < minTime) {
        minTime = sign.buffVals[0].beginTime;
      }
      if (sign.buffVals[sign.buffVals.length - 1].beginTime > maxTime) {
        maxTime = sign.buffVals[sign.buffVals.length - 1].beginTime;
      }
    }

    const cyclePacket = dataParams.packetSize * dataParams.cycleTimeMs;
    if (minTime === Number.MAX_VALUE || maxTime === -Number.MAX_VALUE) {
      return tmIntervalRef.current;
    }
    return { beginMs: minTime, endMs: maxTime + cyclePacket };
  }, [dataParams]);

  const calcValueInterval = useCallback((timeRange?: TmInterval): ValInterval => {
    const currentTm = timeRange ?? tmIntervalRef.current;
    let minValue = Number.MAX_VALUE;
    let maxValue = -Number.MAX_VALUE;

    for (const k in signalsRef.current) {
      const sign = signalsRef.current[k];
      if (isBoolSignal(sign)) {
        continue;
      }

      for (const vals of sign.buffVals) {
        if (currentTm.beginMs < vals.beginTime && vals.beginTime < currentTm.endMs) {
          for (const v of vals.vals) {
            const num = v as number;
            if (num < minValue) {
              minValue = num;
            }
            if (num > maxValue) {
              maxValue = num;
            }
          }
        }
      }
    }

    if (minValue === Number.MAX_VALUE || maxValue === -Number.MAX_VALUE) {
      return valIntervalRef.current;
    }
    return { begin: minValue, end: maxValue };
  }, []);

  useLayoutEffect(() => {
    if (!isPlay || !hasLegend) {
      return;
    }

    const nextTm = calcTimeInterval();
    const cyclePacket = dataParams.packetSize * dataParams.cycleTimeMs;
    let beginMs = tmIntervalRef.current.beginMs + cyclePacket;
    const endMs = nextTm.endMs;
    if (endMs - beginMs < 1000) {
      beginMs = endMs - 1000;
    }
    const scrolledTm = { beginMs, endMs };

    setTmInterval(scrolledTm);

    if (isAutoResize) {
      setValInterval(calcValueInterval(scrolledTm));
    }
  }, [
    isAutoResize,
    isPlay,
    calcTimeInterval,
    calcValueInterval,
    dataParams,
    hasLegend,
    signalsDataKey,
  ]);

  const bumpZIndex = () => {
    zIndex.current += 1;
    zIndexLocalRef.current = zIndex.current + 1;
    forceRender((n) => n + 1);
  };

  const handlePlotChange = (
    nextTm: TmInterval,
    nextVal: ValInterval,
    nextAxis: AxisParams,
  ) => {
    bumpZIndex();
    setTmInterval(nextTm);
    setValInterval(nextVal);
    setAxisParams(nextAxis);
  };

  const handleAxisTimeChange = (nextTm: TmInterval, nextAxis: AxisParams) => {
    setTmInterval(nextTm);
    setAxisParams(nextAxis);
  };

  const handleAxisValueChange = (nextVal: ValInterval, nextAxis: AxisParams) => {
    setValInterval(nextVal);
    setAxisParams(nextAxis);
  };

  const handleDelSignal = (sname: Sname) => {
    delete signColorParamsRef.current[sname];
    onDelSignal(iGraph, sname);
  };

  const handleHeaderMouseDown = (event: React.MouseEvent) => {
    if (event.button === 0) {
      isDraggingGraphRef.current = true;
    }
  };

  const handleHeaderMouseUp = () => {
    isDraggingGraphRef.current = false;
  };

  const handleChangePosContainer = (event: React.MouseEvent) => {
    if (!isDraggingGraphRef.current || !(event.buttons & 1)) {
      return;
    }

    bumpZIndex();
    const distX = event.nativeEvent.movementX;
    const distY = event.nativeEvent.movementY;
    setOffsPosContainer((prev) => ({
      left: prev.left + distX,
      top: prev.top + distY,
    }));
  };

  const handleResizeMouseDown = (event: React.MouseEvent) => {
    if (event.button !== 0) {
      return;
    }

    event.preventDefault();
    isResizingRef.current = true;
    isResizeComponentRef.current = true;
    bumpZIndex();
  };

  const handleResizeFull = () => {
    const nextTm = calcTimeInterval();
    setTmInterval(nextTm);
    setValInterval(calcValueInterval(nextTm));
  };

  const handleResizeByValue = () => {
    setValInterval(calcValueInterval());
  };

  const handleResizeByTime = () => {
    setTmInterval(calcTimeInterval());
  };

  const handleChangeColor = () => {
    for (const k in signColorParamsRef.current) {
      signColorParamsRef.current[k].color =
        '#' + Math.floor(Math.random() * 16777215).toString(16);
    }
    forceRender((n) => n + 1);
  };

  const handleAutoResize = () => {
    setIsAutoResize((prev) => {
      const next = !prev;
      if (next) {
        setValInterval(calcValueInterval(tmIntervalRef.current));
      }
      return next;
    });
  };

  const handlePlay = () => {
    setIsPlay((prev) => !prev);
  };

  const boolLegendColor = backgroundColor === 'black' ? '#fff' : '#000';

  const renderLegendItem = (k: Sname, extraStyle?: React.CSSProperties, labelColor?: string) => {
    if (!signColorParamsRef.current[k]) {
      signColorParamsRef.current[k] = {
        lineWidth: 2,
        transparent: 0.5,
        color: '#' + Math.floor(Math.random() * 16777215).toString(16),
      };
    }

    return (
      <p
        key={k}
        onClick={() => handleDelSignal(k)}
        style={{
          margin: 0,
          cursor: 'default',
          pointerEvents: 'auto',
          color: labelColor ?? signColorParamsRef.current[k].color,
          lineHeight: '18px',
          whiteSpace: 'nowrap',
          ...extraStyle,
        }}
      >
        {signals[k].name}
      </p>
    );
  };

  const analogLegend = analogLegendKeys.map((k) => renderLegendItem(k));
  const boolLegend = boolLegendKeys.map((k) => {
    const trackIndex = boolTrackIndex(boolLegendKeys, k);
    return renderLegendItem(
      k,
      {
        position: 'absolute',
        left: 10,
        bottom: trackIndex * BOOL_TRACK_DIST + 1,
        lineHeight: '11px',
        fontSize: '8pt',
      },
      boolLegendColor,
    );
  });

  const style: React.CSSProperties = {
    position: isResizeComponentRef.current ? 'absolute' : 'relative',
    zIndex: zIndexLocalRef.current,
    left: offsPosContainer.left,
    top: offsPosContainer.top,
    border: GRAPH_BORDER,
    borderRadius: '6px',
    overflow: 'hidden',
    backgroundColor: GRAPH_CHROME_BG,
  };

  if (isResizeComponentRef.current) {
    style.width = csizeContainer.width;
  }

  const buttonStyle = { margin: '.2em' };

  return (
    <Container style={style} ref={graphRef}>
      <Row
        noGutters
        style={{ borderRadius: '3px 3px 0px 0px', padding: '5px', backgroundColor: GRAPH_CHROME_BG }}
        onMouseDown={handleHeaderMouseDown}
        onMouseUp={handleHeaderMouseUp}
        onMouseLeave={handleHeaderMouseUp}
        onMouseMove={handleChangePosContainer}
      >
        <Col style={{ maxWidth: '50px' }} />
        <Col className="col">
          <Button
            size="sm"
            className="icon-resize-full-alt"
            style={buttonStyle}
            onClick={handleResizeFull}
          />
          <Button
            size="sm"
            className="icon-resize-vertical"
            style={buttonStyle}
            onClick={handleResizeByValue}
          />
          <Button
            size="sm"
            className="icon-resize-horizontal"
            style={buttonStyle}
            onClick={handleResizeByTime}
          />
          <Button size="sm" className="icon-brush" style={buttonStyle} onClick={handleChangeColor} />
          <Button
            size="sm"
            variant={isAutoResize ? 'primary' : 'light'}
            className="icon-font"
            style={buttonStyle}
            onClick={handleAutoResize}
          />
          <Button
            size="sm"
            variant={isPlay ? 'primary' : 'light'}
            className={isPlay ? 'icon-pause' : 'icon-play'}
            style={buttonStyle}
            onClick={handlePlay}
          />
          <button type="button" className="close" aria-label="Close" onClick={() => onCloseGraph(iGraph)}>
            <span aria-hidden="true">&times;</span>
          </button>
        </Col>
      </Row>
      <Row noGutters style={{ paddingRight: '5px', backgroundColor: GRAPH_CHROME_BG }}>
        <Col style={{ maxWidth: '50px', height: csizeContainer.height }}>
          <AxisValue
            valInterval={valInterval}
            axisParams={axisParams}
            onChange={handleAxisValueChange}
          />
        </Col>
        <Col className="col" style={{ position: 'relative', height: csizeContainer.height }}>
          <Plot
            tmInterval={tmInterval}
            valInterval={valInterval}
            signals={signals}
            signalOrder={signalOrder}
            axisParams={axisParams}
            dataParams={dataParams}
            signColorParams={signColorParamsRef.current}
            backgroundColor={backgroundColor}
            onChange={handlePlotChange}
            onDrop={(sname) => onAddSignal(iGraph, sname)}
            onFullResize={handleResizeFull}
          />
          <div
            style={{
              position: 'absolute',
              inset: 0,
              zIndex: 2,
              pointerEvents: 'none',
            }}
          >
            <div
              style={{
                position: 'absolute',
                top: 4,
                left: 10,
                display: 'flex',
                flexDirection: 'column',
                gap: 2,
              }}
            >
              {analogLegend}
            </div>
            {boolLegend}
          </div>
        </Col>
      </Row>
      <Row
        noGutters
        style={{
          borderRadius: '0px 0px 3px 3px',
          paddingRight: '5px',
          backgroundColor: GRAPH_CHROME_BG,
        }}
      >
        <Col style={{ maxWidth: '50px' }} />
        <Col className="col" style={{ height: '50px' }}>
          <AxisTime
            tmInterval={tmInterval}
            axisParams={axisParams}
            onChange={handleAxisTimeChange}
          />
          <a
            style={{
              position: 'absolute',
              right: -20,
              top: 10,
              cursor: 'nwse-resize',
              width: '50px',
              height: '50px',
              paddingLeft: 25,
              paddingTop: 15,
              userSelect: 'none',
            }}
            onMouseDown={handleResizeMouseDown}
          >
            &#8250;
          </a>
        </Col>
      </Row>
    </Container>
  );
}
