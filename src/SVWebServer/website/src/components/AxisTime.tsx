import { useCallback, useEffect, useRef } from 'react';
import type { AxisParams, TmInterval } from '../types/telemetry';

type AxisTimeProps = {
  tmInterval: TmInterval;
  axisParams: AxisParams;
  onChange: (tmInterval: TmInterval, axisParams: AxisParams) => void;
};

export function scaleByTime(
  fontMetr: number,
  delta: number,
  axisParams: AxisParams,
  tmInterval: TmInterval,
): { tmInterval: TmInterval; tmDashStep: number } {
  let tmDashStep = axisParams.tmDashStep;

  if (delta === 0) {
    return { tmInterval, tmDashStep };
  }

  if (delta > 0) {
    tmDashStep++;
  } else {
    tmDashStep--;
  }

  if (tmDashStep > 3 * fontMetr) {
    tmDashStep = 2 * fontMetr;
  } else if (tmDashStep < fontMetr * 1.1) {
    tmDashStep = 2 * fontMetr;
  }

  let offs = 10000;
  const curIntervSec = (tmInterval.endMs - tmInterval.beginMs) / 1000;

  if (curIntervSec > 86400) {
    offs *= 1000;
  } else if (curIntervSec > 3600) {
    offs *= 100;
  } else if (curIntervSec < 1) {
    offs /= 1000;
  } else if (curIntervSec < 60) {
    offs /= 10;
  }

  const nextInterval = { ...tmInterval };

  if (delta > 0) {
    nextInterval.beginMs += offs;
    nextInterval.endMs += -offs;

    if (nextInterval.beginMs >= nextInterval.endMs) {
      const mdl = Math.abs(nextInterval.beginMs + nextInterval.endMs) / 2;
      nextInterval.beginMs = mdl - 10;
      nextInterval.endMs = mdl + 10;
    }
  } else {
    nextInterval.beginMs += -offs;
    nextInterval.endMs += offs;
  }

  return { tmInterval: nextInterval, tmDashStep };
}

export function getTimeMark(width: number, offs: number, tmInterval: TmInterval): string {
  const curIntervSec = (tmInterval.endMs - tmInterval.beginMs) / 1000;
  const tmScale = (tmInterval.endMs - tmInterval.beginMs) / width;
  const dt = new Date(tmInterval.beginMs + tmScale * offs);

  let timeMark = '';

  if (curIntervSec > 86400) {
    const options: Intl.DateTimeFormatOptions = {
      hour12: false,
      day: '2-digit',
      month: '2-digit',
      year: '2-digit',
      hour: '2-digit',
      minute: '2-digit',
    };
    timeMark = dt.toLocaleTimeString('en-US', options);
  } else {
    const options: Intl.DateTimeFormatOptions = {
      hour12: false,
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
    };
    timeMark = dt.toLocaleTimeString('en-US', options).split(' ')[0];

    if (curIntervSec < 60) {
      timeMark += ':' + dt.getMilliseconds();
    }
  }

  return timeMark;
}

export default function AxisTime({ tmInterval, axisParams, onChange }: AxisTimeProps) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);

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
    ctx.moveTo(0, 0);
    ctx.lineTo(w, 0);

    const { tmOffsPos, tmDashStep } = axisParams;
    let offs = tmOffsPos % tmDashStep;
    while (offs < w) {
      ctx.moveTo(offs, 0);
      ctx.lineTo(offs, 5);
      offs += tmDashStep;
    }
    ctx.stroke();

    ctx.font = 'normal 9pt Arial';
    offs = tmOffsPos % tmDashStep;
    while (offs < w) {
      const timeMark = getTimeMark(w, offs, tmInterval);
      const fontMetr = ctx.measureText(timeMark).width;
      ctx.fillText(timeMark, offs - fontMetr / 2, h / 2);
      offs += tmDashStep;
    }
  }, [axisParams, tmInterval]);

  useEffect(() => {
    drawCanvas();
  }, [drawCanvas]);

  const handleMouseMove = (event: React.MouseEvent<HTMLCanvasElement>) => {
    const canvas = canvasRef.current;
    if (!canvas || !event.buttons) {
      return;
    }

    const width = canvas.clientWidth;
    const tmScale = (tmInterval.endMs - tmInterval.beginMs) / width;
    const diffPos = event.nativeEvent.movementX;

    const { tmDashStep, ...exPrms } = axisParams;
    let tmOffsPos = axisParams.tmOffsPos;

    if (tmOffsPos > tmDashStep) {
      tmOffsPos = 0;
    } else if (tmOffsPos < 0) {
      tmOffsPos = tmDashStep;
    }

    let shift = -tmScale * diffPos - 1;
    if (diffPos < 0) {
      shift = -tmScale * diffPos + 1;
    }

    const nextInterval = {
      beginMs: tmInterval.beginMs + shift,
      endMs: tmInterval.endMs + shift,
    };

    onChange(nextInterval, { ...exPrms, tmOffsPos, tmDashStep });
  };

  const handleWheel = (event: React.WheelEvent<HTMLCanvasElement>) => {
    const delta = -(event.deltaY || (event as unknown as { detail: number }).detail || 0);
    const { tmDashStep: _tmDashStep, ...exParams } = axisParams;
    const canvas = canvasRef.current;
    if (!canvas) {
      return;
    }

    const width = canvas.clientWidth;
    const ctx = canvas.getContext('2d');
    if (!ctx) {
      return;
    }

    const timeMark = getTimeMark(width, 0, tmInterval);
    const fontMetr = ctx.measureText(timeMark).width;
    const { tmInterval: nextInterval, tmDashStep } = scaleByTime(
      fontMetr,
      delta,
      axisParams,
      tmInterval,
    );

    onChange(nextInterval, { ...exParams, tmDashStep });
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
