import { useCallback, useEffect, useRef } from 'react';
import { scaleByValue } from './AxisValue';
import { getTimeMark, scaleByTime } from './AxisTime';
import type {
  AxisParams,
  DataParams,
  SignColorParamsMap,
  SignalsMap,
  Sname,
  TmInterval,
  ValInterval,
} from '../types/telemetry';
import { BOOL_TRACK_DIST, getBoolKeys, isBoolSignal } from '../utils/graphSignals';

type PlotProps = {
  tmInterval: TmInterval;
  valInterval: ValInterval;
  signals: SignalsMap;
  signalOrder: Sname[];
  axisParams: AxisParams;
  dataParams: DataParams;
  signColorParams: SignColorParamsMap;
  backgroundColor: string;
  onChange: (tmInterval: TmInterval, valInterval: ValInterval, axisParams: AxisParams) => void;
  onDrop: (sname: string) => void;
  onFullResize?: () => void;
};

type Point = { x?: number; y?: number; pos?: number; value?: number };
type Rect = { x: number; y: number; width: number; height: number };
type TouchPoint = { identifier: number; pageX: number; pageY: number };

function signalBoolValue(v: number | boolean): number {
  return v ? 1 : 0;
}

function syncCanvasSize(canvas: HTMLCanvasElement) {
  const w = canvas.clientWidth;
  const h = canvas.clientHeight;
  if (w > 0 && h > 0 && (canvas.width !== w || canvas.height !== h)) {
    canvas.width = w;
    canvas.height = h;
  }
}

function getCanvasPoint(canvas: HTMLCanvasElement, clientX: number, clientY: number) {
  syncCanvasSize(canvas);
  const bounds = canvas.getBoundingClientRect();
  const scaleX = canvas.width / bounds.width;
  const scaleY = canvas.height / bounds.height;
  return {
    x: (clientX - bounds.left) * scaleX,
    y: (clientY - bounds.top) * scaleY,
  };
}

function rectFromPoints(start: Point, end: Point): Rect {
  const x1 = start.x ?? 0;
  const y1 = start.y ?? 0;
  const x2 = end.x ?? 0;
  const y2 = end.y ?? 0;
  const left = Math.min(x1, x2);
  const top = Math.min(y1, y2);
  return {
    x: left,
    y: top,
    width: Math.abs(x2 - x1),
    height: Math.abs(y2 - y1),
  };
}

export default function Plot({
  tmInterval,
  valInterval,
  signals,
  signalOrder,
  axisParams,
  dataParams,
  signColorParams,
  backgroundColor,
  onChange,
  onDrop,
  onFullResize,
}: PlotProps) {
  const orderedKeys = signalOrder.length > 0 ? signalOrder : Object.keys(signals);
  const boolKeys = getBoolKeys(orderedKeys, signals);
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const rectRef = useRef<Rect>({ x: 0, y: 0, width: 0, height: 0 });
  const memMDownRef = useRef<Point>({});
  const memMPosRef = useRef<Point>({});
  const signPntsRef = useRef<Record<string, Point[][]>>({});
  const ongoingTouchesRef = useRef<TouchPoint[]>([]);
  const isSelectingRef = useRef(false);
  const isPanningRef = useRef(false);

  const setPlotCursor = (cursor: string) => {
    if (canvasRef.current) {
      canvasRef.current.style.cursor = cursor;
    }
  };

  const getSignalPoints = useCallback(
    (width: number, height: number) => {
      const tmScale = (tmInterval.endMs - tmInterval.beginMs) / width;
      const valScale = (valInterval.end - valInterval.begin) / height;
      const valMinInterval = valInterval.begin;
      const tmMinInterval = tmInterval.beginMs;
      const tmMaxInterval = tmInterval.endMs;
      const packetSize = dataParams.packetSize;
      const cycleTimeMs = dataParams.cycleTimeMs;
      const packetTimeMs = packetSize * cycleTimeMs;
      const valPosMem = valMinInterval / valScale;
      const resPnts: Record<string, Point[][]> = {};

      for (const sign in signals) {
        const buffVals = signals[sign].buffVals;
        const buffSz = buffVals.length;
        const stype = signals[sign].type;
        const tBool = 0;

        if (buffSz === 0) {
          continue;
        }

        let tmZnBegin = buffVals[0].beginTime;
        let tmZnEnd = tmZnBegin + packetTimeMs;
        let tmZnEndPrev = tmZnBegin;
        const tmPosMem: number[] = [];
        for (let i = 0; i < packetSize; ++i) {
          tmPosMem.push((i * cycleTimeMs - tmMinInterval) / tmScale);
        }

        let prevPos = -1;
        let iBuf = 0;
        const zonePnts: Point[][] = [[]];

        while (tmZnBegin < tmMaxInterval) {
          if (tmZnEnd > tmMinInterval) {
            const rd = buffVals[iBuf];
            const tmZnBeginMem = tmZnBegin / tmScale;

            if (tmZnBegin - tmZnEndPrev > packetTimeMs) {
              zonePnts.push([]);
            }

            const backZone = zonePnts[zonePnts.length - 1];

            for (let i = 0; i < packetSize; ++i) {
              const pnt: Point = {};
              pnt.pos = tmPosMem[i] + tmZnBeginMem;

              if (Math.round(pnt.pos!) > prevPos) {
                prevPos = Math.round(pnt.pos!);

                if (stype !== tBool) {
                  pnt.value = (rd.vals[i] as number) / valScale - valPosMem;
                } else {
                  pnt.value = signalBoolValue(rd.vals[i] as number | boolean);
                }

                backZone.push(pnt);
              }
            }
          }
          tmZnEndPrev = tmZnEnd;

          ++iBuf;
          if (iBuf === buffSz) {
            break;
          }

          tmZnBegin = buffVals[iBuf].beginTime;
          tmZnEnd = tmZnBegin + packetTimeMs;
        }

        resPnts[sign] = zonePnts;
      }

      return resPnts;
    },
    [dataParams, signals, tmInterval, valInterval],
  );

  const getTimePosMark = useCallback(() => {
    const canvas = canvasRef.current;
    if (!canvas) {
      return [];
    }
    const width = canvas.clientWidth;
    const { tmOffsPos, tmDashStep } = axisParams;
    let offs = tmOffsPos % tmDashStep;
    const mark: number[] = [];
    while (offs < width) {
      mark.push(offs);
      offs += tmDashStep;
    }
    return mark;
  }, [axisParams]);

  const getValPosMark = useCallback(() => {
    const canvas = canvasRef.current;
    if (!canvas) {
      return [];
    }
    const height = canvas.clientHeight;
    const { valOffsPos, valDashStep } = axisParams;
    let offs = valOffsPos % valDashStep;
    const mark: number[] = [];
    while (offs < height) {
      mark.push(offs);
      offs += valDashStep;
    }
    return mark;
  }, [axisParams]);

  const drawRect = useCallback((ctx: CanvasRenderingContext2D) => {
    const rct = rectRef.current;
    if (rct.width > 0 || rct.height > 0) {
      ctx.beginPath();
      ctx.strokeStyle = 'green';
      ctx.lineWidth = 2;
      ctx.globalAlpha = 1;
      ctx.rect(rct.x, rct.y, rct.width, rct.height);
      ctx.closePath();
      ctx.stroke();
    }
  }, []);

  const drawMousePos = useCallback(
    (w: number, h: number, ctx: CanvasRenderingContext2D) => {
      const mpos = memMPosRef.current;
      const valScale = (valInterval.end - valInterval.begin) / h;
      const valMinInterval = valInterval.begin;
      const valPosMem = valMinInterval / valScale;
      const pad = 2;
      const textGap = 7;

      ctx.globalAlpha = 1;

      for (const k of orderedKeys) {
        if (!signPntsRef.current[k] || isBoolSignal(signals[k])) {
          continue;
        }

        ctx.fillStyle = signColorParams[k].color;
        ctx.strokeStyle = signColorParams[k].color;

        const zonePnts = signPntsRef.current[k];
        let point: Point = { pos: -1, value: -1 };

        for (const pnts of zonePnts) {
          if (pnts.length === 0) {
            continue;
          }
          for (let i = 0; i < pnts.length; ++i) {
            if (Math.abs((mpos.x ?? 0) - (pnts[i].pos ?? 0)) < 5) {
              point = pnts[i];
              break;
            }
          }
          if (point.pos !== -1) {
            break;
          }
        }

        if (point.pos !== -1) {
          const val = (point.value! + valPosMem) * valScale;
          const text = val.toFixed(1);
          const dotX = point.pos!;
          const dotY = h - point.value!;

          ctx.font = 'normal 9pt Arial';
          const textW = ctx.measureText(text).width;
          const textH = 11;

          let textX = dotX;
          let textY = dotY - textGap;

          if (textY - textH < pad) {
            textY = dotY + textGap + textH;
          }
          if (textY > h - pad) {
            textY = h - pad;
          }
          if (textX + textW > w - pad) {
            textX = w - textW - pad;
          }
          if (textX < pad) {
            textX = pad;
          }

          ctx.strokeText(text, textX, textY);
          ctx.beginPath();
          ctx.arc(dotX, dotY, 3, 0, 360);
          ctx.closePath();
          ctx.fill();
          ctx.stroke();
        }
      }
    },
    [dataParams, orderedKeys, signColorParams, signals, tmInterval, valInterval],
  );

  const drawSignals = useCallback(
    (width: number, height: number, ctx: CanvasRenderingContext2D) => {
      const signPnts = signPntsRef.current;
      const packetSize = dataParams.packetSize;
      const cycleTimeMs = dataParams.cycleTimeMs;
      const packetTimeMs = packetSize * cycleTimeMs;
      const valScale = (valInterval.end - valInterval.begin) / height;

      for (const k of orderedKeys) {
        if (!signPnts[k] || isBoolSignal(signals[k])) {
          continue;
        }

        ctx.strokeStyle = signColorParams[k].color;
        ctx.fillStyle = signColorParams[k].color;

        const isFillGraph = signColorParams[k].transparent < 1;
        const isPaintPnt = tmInterval.endMs - tmInterval.beginMs < packetTimeMs * 5;
        const zonePnts = signPnts[k];

        for (const pnts of zonePnts) {
          if (pnts.length === 0) {
            continue;
          }

          ctx.beginPath();
          ctx.lineWidth = signColorParams[k].lineWidth;
          ctx.globalAlpha = 1;
          ctx.moveTo(pnts[0].pos!, height - pnts[0].value!);
          for (let i = 1; i < pnts.length; ++i) {
            ctx.lineTo(pnts[i].pos!, height - pnts[i].value!);
          }
          ctx.stroke();

          if (isPaintPnt) {
            for (let i = 0; i < pnts.length; ++i) {
              ctx.beginPath();
              ctx.arc(pnts[i].pos!, height - pnts[i].value!, 3, 0, 360);
              ctx.closePath();
              ctx.fill();
              ctx.stroke();
            }
          }

          if (isFillGraph) {
            ctx.beginPath();
            ctx.lineWidth = 1;
            ctx.globalAlpha = signColorParams[k].transparent;

            let yPos = height;
            if (valInterval.begin < 0 && valInterval.end > 0) {
              yPos = valInterval.end / valScale;
            } else if (valInterval.begin < 0 && valInterval.end < 0) {
              yPos = 0;
            }

            ctx.moveTo(pnts[0].pos!, yPos);

            const step = 1024.0 / pnts.length;
            let cxPos = 0;
            let prevxPos = -1;

            for (let i = 0; i < pnts.length; ++i) {
              if (Math.round(cxPos) > prevxPos) {
                prevxPos = Math.round(cxPos);
                ctx.lineTo(pnts[i].pos!, height - pnts[i].value!);
              }
              cxPos += step;
            }

            ctx.lineTo(pnts[pnts.length - 1].pos!, yPos);
            ctx.fill();
            ctx.stroke();
          }
        }
      }

      for (const k of boolKeys) {
        if (!signPnts[k]) {
          continue;
        }

        const signBoolCnt = boolKeys.indexOf(k);

        ctx.strokeStyle = signColorParams[k].color;
        ctx.fillStyle = signColorParams[k].color;
        ctx.lineWidth = 1;
        ctx.globalAlpha = 1;

        const zonePnts = signPnts[k];

        for (const pnts of zonePnts) {
          if (pnts.length === 0) {
            continue;
          }

          ctx.beginPath();
          const psz = pnts.length;
          const sDist = BOOL_TRACK_DIST;
          const sH = 10;
          let prevPos = 1;
          let prevVal = signalBoolValue(pnts[0].value as number | boolean);

          for (let i = 1; i < psz; ++i) {
            const curVal = signalBoolValue(pnts[i].value as number | boolean);
            if (prevVal === 0 && (curVal > 0 || i === psz - 1)) {
              ctx.moveTo(pnts[prevPos - 1].pos!, height - signBoolCnt * sDist - 1);
              ctx.lineTo(pnts[i].pos!, height - signBoolCnt * sDist - 1);
              prevVal = 1;
              prevPos = i;
            } else if (prevVal > 0 && (curVal === 0 || i === psz - 1)) {
              ctx.rect(
                pnts[prevPos - 1].pos!,
                height - signBoolCnt * sDist - sH - 1,
                pnts[i - 1].pos! - pnts[prevPos - 1].pos!,
                sH + 1,
              );
              ctx.fill();
              prevVal = 0;
              prevPos = i;
            }
          }
          ctx.stroke();
        }
      }
    },
    [boolKeys, dataParams, signColorParams, signals, tmInterval, valInterval],
  );

  const drawAxisMark = useCallback(
    (width: number, height: number, ctx: CanvasRenderingContext2D) => {
      ctx.beginPath();
      ctx.lineWidth = 1;
      ctx.strokeStyle = '#808080';
      ctx.globalAlpha = 0.2;

      for (const pos of getTimePosMark()) {
        ctx.moveTo(pos, 0);
        ctx.lineTo(pos, height);
      }

      for (const pos of getValPosMark()) {
        ctx.moveTo(0, pos);
        ctx.lineTo(width, pos);
      }

      ctx.stroke();
    },
    [getTimePosMark, getValPosMark],
  );

  const drawCanvas = useCallback(() => {
    const canvas = canvasRef.current;
    if (!canvas) {
      return;
    }

    const w = canvas.clientWidth;
    const h = canvas.clientHeight;
    const ctx = canvas.getContext('2d');
    if (!ctx) {
      return;
    }

    syncCanvasSize(canvas);

    ctx.clearRect(0, 0, w, h);
    drawAxisMark(w, h, ctx);
    drawSignals(w, h, ctx);
    drawRect(ctx);
    drawMousePos(w, h, ctx);
  }, [drawAxisMark, drawMousePos, drawRect, drawSignals]);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) {
      return;
    }
    signPntsRef.current = getSignalPoints(canvas.clientWidth, canvas.clientHeight);
    drawCanvas();
  }, [drawCanvas, getSignalPoints, tmInterval, valInterval, signals, axisParams, dataParams]);

  const onMove = (distX: number, distY: number) => {
    const canvas = canvasRef.current;
    if (!canvas) {
      return;
    }

    const nextValInterval = { ...valInterval };
    const { valDashStep, tmDashStep, ...exParams } = axisParams;
    let valOffsPos = axisParams.valOffsPos;
    let tmOffsPos = axisParams.tmOffsPos;

    const height = canvas.clientHeight;
    const valScale = (nextValInterval.end - nextValInterval.begin) / height;
    valOffsPos += distY;

    if (valOffsPos > valDashStep) {
      valOffsPos = 0;
    } else if (valOffsPos < 0) {
      valOffsPos = valDashStep;
    }

    nextValInterval.begin += valScale * distY;
    nextValInterval.end += valScale * distY;

    const nextTmInterval = { ...tmInterval };
    const width = canvas.clientWidth;
    const tmScale = (nextTmInterval.endMs - nextTmInterval.beginMs) / width;
    tmOffsPos += distX;

    if (tmOffsPos > tmDashStep) {
      tmOffsPos = 0;
    } else if (tmOffsPos < 0) {
      tmOffsPos = tmDashStep;
    }

    let offs = -tmScale * distX - 1;
    if (distX < 0) {
      offs = -tmScale * distX + 1;
    }

    nextTmInterval.beginMs += offs;
    nextTmInterval.endMs += offs;

    onChange(nextTmInterval, nextValInterval, {
      ...exParams,
      tmOffsPos,
      tmDashStep,
      valOffsPos,
      valDashStep,
    });
  };

  const onWheel = (deltaX: number, deltaY: number) => {
    const canvas = canvasRef.current;
    if (!canvas) {
      return;
    }

    const { tmDashStep: _td, valDashStep: _vd, ...exParams } = axisParams;
    let nextValInterval = { ...valInterval };
    let nextTmInterval = { ...tmInterval };

    const scaledVal = scaleByValue(deltaY, axisParams, nextValInterval);
    nextValInterval = scaledVal.valInterval;
    const valDashStep = scaledVal.valDashStep;

    const width = canvas.clientWidth;
    const ctx = canvas.getContext('2d');
    if (!ctx) {
      return;
    }

    const timeMark = getTimeMark(width, 0, nextTmInterval);
    const fontMetr = ctx.measureText(timeMark).width;
    const scaledTm = scaleByTime(fontMetr, deltaX, axisParams, nextTmInterval);
    nextTmInterval = scaledTm.tmInterval;
    const tmDashStep = scaledTm.tmDashStep;

    onChange(nextTmInterval, nextValInterval, { ...exParams, tmDashStep, valDashStep });
  };

  const handleTouchMove = (event: React.TouchEvent<HTMLCanvasElement>) => {
    event.preventDefault();

    if (event.changedTouches.length === 1) {
      const eTouch = event.changedTouches[0];
      const idx = ongoingTouchesRef.current.findIndex((it) => it.identifier === eTouch.identifier);

      if (idx >= 0) {
        const distX = eTouch.pageX - ongoingTouchesRef.current[idx].pageX;
        const distY = eTouch.pageY - ongoingTouchesRef.current[idx].pageY;
        onMove(distX, distY);
        ongoingTouchesRef.current.splice(idx, 1, {
          identifier: eTouch.identifier,
          pageX: eTouch.pageX,
          pageY: eTouch.pageY,
        });
      }
    } else {
      const eTouch0 = event.changedTouches[0];
      const idx0 = ongoingTouchesRef.current.findIndex((it) => it.identifier === eTouch0.identifier);
      const eTouch1 = event.changedTouches[1];
      const idx1 = ongoingTouchesRef.current.findIndex((it) => it.identifier === eTouch1.identifier);

      if (idx0 >= 0 && idx1 >= 0) {
        const deltaX0 = eTouch0.pageX - ongoingTouchesRef.current[idx0].pageX;
        const deltaX1 = eTouch1.pageX - ongoingTouchesRef.current[idx1].pageX;
        const deltaY0 = eTouch0.pageY - ongoingTouchesRef.current[idx0].pageY;
        const deltaY1 = eTouch1.pageY - ongoingTouchesRef.current[idx1].pageY;

        let deltaX = 0;
        if (deltaX0 > 0 && deltaX1 < 0) {
          deltaX = eTouch0.pageX > eTouch1.pageX ? 1 : -1;
        } else if (deltaX0 < 0 && deltaX1 > 0) {
          deltaX = eTouch0.pageX > eTouch1.pageX ? -1 : 1;
        }

        let deltaY = 0;
        if (deltaY0 > 0 && deltaY1 < 0) {
          deltaY = eTouch0.pageY > eTouch1.pageY ? 1 : -1;
        } else if (deltaY0 < 0 && deltaY1 > 0) {
          deltaY = eTouch0.pageY > eTouch1.pageY ? -1 : 1;
        }

        if (deltaX || deltaY) {
          onWheel(deltaX, deltaY);
        }

        ongoingTouchesRef.current.splice(idx0, 1, {
          identifier: eTouch0.identifier,
          pageX: eTouch0.pageX,
          pageY: eTouch0.pageY,
        });
        const idx = ongoingTouchesRef.current.findIndex((it) => it.identifier === eTouch1.identifier);
        ongoingTouchesRef.current.splice(idx, 1, {
          identifier: eTouch1.identifier,
          pageX: eTouch1.pageX,
          pageY: eTouch1.pageY,
        });
      }
    }
  };

  const handleTouchStart = (event: React.TouchEvent<HTMLCanvasElement>) => {
    for (const eTouch of Array.from(event.changedTouches)) {
      ongoingTouchesRef.current.push({
        identifier: eTouch.identifier,
        pageX: eTouch.pageX,
        pageY: eTouch.pageY,
      });
    }

    const canvas = event.currentTarget;
    memMPosRef.current = getCanvasPoint(canvas, event.changedTouches[0].clientX, event.changedTouches[0].clientY);
    drawCanvas();
    event.preventDefault();
  };

  const handleTouchEnd = (event: React.TouchEvent<HTMLCanvasElement>) => {
    event.preventDefault();
    for (const eTouch of Array.from(event.changedTouches)) {
      const idx = ongoingTouchesRef.current.findIndex((it) => it.identifier === eTouch.identifier);
      if (idx >= 0) {
        ongoingTouchesRef.current.splice(idx, 1);
      }
    }
  };

  const updateSelectionRect = (mpos: Point) => {
    const presPnt = memMDownRef.current;
    if (presPnt.x === undefined || presPnt.y === undefined) {
      memMDownRef.current = mpos;
    }
    rectRef.current = rectFromPoints(memMDownRef.current, mpos);
    drawCanvas();
  };

  const applySelectionZoom = useCallback(() => {
    const rct = { ...rectRef.current };
    rectRef.current = { x: 0, y: 0, width: 0, height: 0 };

    if (rct.width < 30 || rct.height < 30) {
      if (rct.width > 0 || rct.height > 0) {
        drawCanvas();
      }
      return;
    }

    const canvas = canvasRef.current;
    if (!canvas) {
      return;
    }

    syncCanvasSize(canvas);
    const w = canvas.width;
    const h = canvas.height;
    const tmScale = (tmInterval.endMs - tmInterval.beginMs) / w;
    const tmBegin = tmInterval.beginMs + rct.x * tmScale;
    const tmEnd = tmInterval.beginMs + (rct.x + rct.width) * tmScale;
    const valScale = (valInterval.end - valInterval.begin) / h;
    const valBegin = valInterval.begin + (h - rct.y - rct.height) * valScale;
    const valEnd = valInterval.begin + (h - rct.y) * valScale;

    onChange({ beginMs: tmBegin, endMs: tmEnd }, { begin: valBegin, end: valEnd }, axisParams);
  }, [axisParams, drawCanvas, onChange, tmInterval, valInterval]);

  const endSelection = useCallback(
    (button: number) => {
      if (!isSelectingRef.current) {
        return;
      }

      isSelectingRef.current = false;
      memMDownRef.current = {};

      if (button !== 0) {
        rectRef.current = { x: 0, y: 0, width: 0, height: 0 };
        drawCanvas();
        return;
      }

      applySelectionZoom();
    },
    [applySelectionZoom, drawCanvas],
  );

  useEffect(() => {
    const onWindowMouseUp = (event: MouseEvent) => {
      if (event.button === 2 && isPanningRef.current) {
        isPanningRef.current = false;
        setPlotCursor('crosshair');
      }
      endSelection(event.button);
    };

    window.addEventListener('mouseup', onWindowMouseUp);
    return () => window.removeEventListener('mouseup', onWindowMouseUp);
  }, [endSelection]);

  const handleMouseDown = (event: React.MouseEvent<HTMLCanvasElement>) => {
    if (event.button === 2) {
      event.preventDefault();
      isPanningRef.current = true;
      setPlotCursor('grabbing');
      return;
    }

    if (event.button !== 0) {
      return;
    }

    if (event.detail > 1) {
      return;
    }

    event.preventDefault();
    event.stopPropagation();

    const canvas = event.currentTarget;
    const mpos = getCanvasPoint(canvas, event.clientX, event.clientY);
    isSelectingRef.current = true;
    memMDownRef.current = mpos;
    memMPosRef.current = mpos;
    rectRef.current = { x: mpos.x ?? 0, y: mpos.y ?? 0, width: 0, height: 0 };
  };

  const handleMouseMove = (event: React.MouseEvent<HTMLCanvasElement>) => {
    const canvas = event.currentTarget;
    const mpos = getCanvasPoint(canvas, event.clientX, event.clientY);
    memMPosRef.current = mpos;

    if (isSelectingRef.current && (event.buttons & 1)) {
      event.preventDefault();
      updateSelectionRect(mpos);
      return;
    }

    if (event.buttons & 2) {
      if (!isPanningRef.current) {
        isPanningRef.current = true;
      }
      setPlotCursor('grabbing');
      onMove(event.movementX, event.movementY);
      return;
    }

    drawCanvas();
  };

  const handleMouseUp = (event: React.MouseEvent<HTMLCanvasElement>) => {
    if (event.button === 2) {
      isPanningRef.current = false;
      setPlotCursor('crosshair');
      return;
    }

    endSelection(event.button);
  };

  const handleDragOver = (event: React.DragEvent<HTMLCanvasElement>) => {
    const types = Array.from(event.dataTransfer.types);
    if (!types.includes('text') && !types.includes('text/plain')) {
      return;
    }

    event.preventDefault();
    event.dataTransfer.dropEffect = 'copy';
  };

  const handleDoubleClick = (event: React.MouseEvent<HTMLCanvasElement>) => {
    event.preventDefault();
    event.stopPropagation();
    isSelectingRef.current = false;
    memMDownRef.current = {};
    rectRef.current = { x: 0, y: 0, width: 0, height: 0 };
    drawCanvas();
    onFullResize?.();
  };

  const handleWheel = (event: React.WheelEvent<HTMLCanvasElement>) => {
    const delta = -(event.deltaY || (event as unknown as { detail: number }).detail || 0);
    onWheel(delta, delta);
  };

  return (
    <canvas
      ref={canvasRef}
      draggable={false}
      style={{
        backgroundColor,
        height: '100%',
        width: '100%',
        cursor: 'crosshair',
        touchAction: 'none',
        userSelect: 'none',
      }}
      onMouseDown={handleMouseDown}
      onMouseMove={handleMouseMove}
      onMouseUp={handleMouseUp}
      onDoubleClick={handleDoubleClick}
      onWheel={handleWheel}
      onTouchMove={handleTouchMove}
      onTouchStart={handleTouchStart}
      onTouchEnd={handleTouchEnd}
      onTouchCancel={handleTouchEnd}
      onDragStart={(event) => event.preventDefault()}
      onDragOver={handleDragOver}
      onContextMenu={(e) => e.preventDefault()}
      onDrop={(e) => {
        e.preventDefault();
        const text = e.dataTransfer.getData('text');
        const dot = text.indexOf('.');
        if (dot < 0) {
          return;
        }
        const module = text.slice(0, dot);
        const name = text.slice(dot + 1);
        onDrop(name + module);
      }}
    />
  );
}
