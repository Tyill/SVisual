import { useCallback, useEffect, useRef } from 'react';
import type { AxisParams, ValInterval } from '../types/telemetry';

type AxisValueProps = {
  valInterval: ValInterval;
  axisParams: AxisParams;
  onChange: (valInterval: ValInterval, axisParams: AxisParams) => void;
};

export function scaleByValue(
  delta: number,
  axisParams: AxisParams,
  valInterval: ValInterval,
): { valInterval: ValInterval; valDashStep: number } {
  let valDashStep = axisParams.valDashStep;
  const { minValDashStep, maxValDashStep } = axisParams;

  if (delta === 0) {
    return { valInterval, valDashStep };
  }

  if (delta > 0) {
    valDashStep++;
  } else {
    valDashStep--;
  }

  if (valDashStep > maxValDashStep) {
    valDashStep = minValDashStep;
  } else if (valDashStep < minValDashStep) {
    valDashStep = maxValDashStep;
  }

  const nextInterval = { ...valInterval };
  const curInterval = nextInterval.end - nextInterval.begin;
  let offs = 10;

  if (curInterval > 1000) {
    offs *= 10;
  } else if (curInterval > 10000) {
    offs *= 100;
  } else if (curInterval < 100) {
    offs /= 10;
  }

  if (delta > 0) {
    nextInterval.begin += offs;
    nextInterval.end -= offs;
    if (nextInterval.begin >= nextInterval.end) {
      nextInterval.begin = nextInterval.end - 0.1;
    }
  } else {
    nextInterval.begin -= offs;
    nextInterval.end += offs;
  }

  return { valInterval: nextInterval, valDashStep };
}

export default function AxisValue({ valInterval, axisParams, onChange }: AxisValueProps) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);

  const getValMark = useCallback(
    (height: number, offs: number) => {
      const scale = (valInterval.end - valInterval.begin) / height;
      let vl = valInterval.end - scale * offs;
      const sign = vl > 0 ? 1 : -1;
      const diap = Math.abs(valInterval.end - valInterval.begin);

      vl *= sign;

      if (diap > 100) {
        vl = parseInt(String(vl + 0.5), 10);
      } else if (diap > 10) {
        vl = parseInt(String(vl * 10 + 0.5), 10) / 10.0;
      } else if (diap > 1) {
        vl = parseInt(String(vl * 100 + 0.5), 10) / 100.0;
      } else {
        vl = parseInt(String(vl * 1000 + 0.5), 10) / 1000.0;
      }

      return (vl * sign).toString();
    },
    [valInterval],
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

    if (canvas.width !== w || canvas.height !== h) {
      canvas.width = w;
      canvas.height = h;
    }

    ctx.clearRect(0, 0, w, h);

    ctx.lineWidth = 1;
    ctx.strokeStyle = '#000000';
    ctx.beginPath();
    ctx.moveTo(w, 0);
    ctx.lineTo(w, h);

    const { valOffsPos, valDashStep } = axisParams;
    let offs = valOffsPos % valDashStep;
    while (offs < h) {
      ctx.moveTo(w - 5, offs);
      ctx.lineTo(w, offs);
      offs += valDashStep;
    }
    ctx.stroke();

    ctx.font = 'normal 9pt Arial';
    offs = valOffsPos % valDashStep;
    while (offs < h) {
      const valMark = getValMark(h, offs);
      const fontMetr = ctx.measureText(valMark).width;
      const xp = Math.max(0, w - fontMetr - 15);
      ctx.fillText(valMark, xp, offs + 5);
      offs += valDashStep;
    }
  }, [axisParams, getValMark]);

  useEffect(() => {
    drawCanvas();
  }, [drawCanvas]);

  const handleMouseMove = (event: React.MouseEvent<HTMLCanvasElement>) => {
    const canvas = canvasRef.current;
    if (!canvas || !event.buttons) {
      return;
    }

    const diff = event.nativeEvent.movementY;
    const { valDashStep, ...exPrms } = axisParams;
    let valOffsPos = axisParams.valOffsPos;
    valOffsPos += diff;

    if (valOffsPos > valDashStep) {
      valOffsPos = 0;
    } else if (valOffsPos < 0) {
      valOffsPos = valDashStep;
    }

    const height = canvas.clientHeight;
    const valScale = (valInterval.end - valInterval.begin) / height;
    const nextInterval = {
      begin: valInterval.begin + valScale * diff,
      end: valInterval.end + valScale * diff,
    };

    onChange(nextInterval, { ...exPrms, valOffsPos, valDashStep });
  };

  const handleWheel = (event: React.WheelEvent<HTMLCanvasElement>) => {
    const delta = -(event.deltaY || (event as unknown as { detail: number }).detail || 0);
    const { valDashStep: _valDashStep, ...exParams } = axisParams;
    const { valInterval: nextInterval, valDashStep } = scaleByValue(delta, axisParams, valInterval);
    onChange(nextInterval, { ...exParams, valDashStep });
  };

  return (
    <canvas
      ref={canvasRef}
      style={{ height: '100%', width: '100%' }}
      onMouseMove={handleMouseMove}
      onWheel={handleWheel}
    />
  );
}
