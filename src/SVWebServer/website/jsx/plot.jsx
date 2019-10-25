/* eslint-disable no-unused-vars */

import React from "react"
const RDom = require('react-dom');
import PropTypes from "prop-types";

export default 
class Plot extends React.Component {
  
  constructor(props){
    super(props);

    this._canvasRef = null;
   
    this.handleMouseMove = this.handleMouseMove.bind(this);
    this.handleWheel = this.handleWheel.bind(this);  
  }
   
  handleMouseMove(event) {
    
    const canvas = this._canvasRef;

    if (!canvas || !event.buttons) return;
        
    let valInterval = this.props.valInterval,
        {valOffsPos, valDashStep, tmOffsPos, tmDashStep, ...exParams} = this.props.axisParams;

    const height = canvas.clientHeight,
          valScale = (valInterval.end - valInterval.begin) / height,
          valDiff = event.nativeEvent.movementY;          
    
    valOffsPos += valDiff;

    if (valOffsPos > valDashStep) 
      valOffsPos = 0;
    else if (valOffsPos < 0) 
      valOffsPos = valDashStep; 

    valInterval.begin += valScale * valDiff;
    valInterval.end += valScale * valDiff;   
    
    //////////////////////////////////////

    let tmInterval = this.props.tmInterval;

    const width = canvas.clientWidth,
          tmScale = (tmInterval.endMs - tmInterval.beginMs) / width,   
          tmDiff = event.nativeEvent.movementX;
      
    tmOffsPos += tmDiff;

    if (tmOffsPos > tmDashStep) 
      tmOffsPos = 0;
    else if (tmOffsPos < 0) 
      tmOffsPos = tmDashStep; 

    let offs = -tmScale * tmDiff - 1;
    if (tmDiff < 0) 
       offs = -tmScale * tmDiff + 1;

  
    tmInterval.beginMs += offs;
    tmInterval.endMs += offs;    
      
    this.props.onChange(tmInterval, valInterval, 
      {tmOffsPos, tmDashStep, valOffsPos, valDashStep, ...exParams});
  }

  handleWheel(e){

    const delta = -(e.deltaY || e.detail || e.wheelDelta);

    let {tmDashStep, valDashStep, ...exParams} = this.props.axisParams,
        valInterval = this.props.valInterval,
        tmInterval = this.props.tmInterval;

    ({valInterval, valDashStep} = this.scaleByValue(delta, valInterval, valDashStep));
    
    ({tmInterval, tmDashStep} = this.scaleByTime(delta, tmInterval, tmDashStep));

    this.props.onChange(tmInterval, valInterval, {tmDashStep, valDashStep, ...exParams});
      
  }

  scaleByValue(delta, valInterval, valDashStep){

    if (delta > 0) valDashStep++;
    else valDashStep--;

    const {minValDashStep, maxValDashStep} = this.props.axisParams;

    if (valDashStep > maxValDashStep) 
      valDashStep = minValDashStep;
    else if (valDashStep < minValDashStep) 
      valDashStep = maxValDashStep;
   
    let curInterval = valInterval.end - valInterval.begin,
        offs = 10;
  
    if (curInterval > 1000) offs *= 10;
    else if (curInterval > 10000) offs *= 100;
    else if (curInterval < 100) offs /= 10;
          
    if (delta > 0){ 
      valInterval.begin += offs;
      valInterval.end -= offs;

      if (valInterval.begin >= valInterval.end)
        valInterval.begin = valInterval.end - 0.1;
    }
    else{ 
      valInterval.begin -= offs;
      valInterval.end += offs;
    }

    return {valInterval, valDashStep};
  }

  scaleByTime(delta, tmInterval, tmDashStep){

    const canvas = this._canvasRef,
    width = canvas.clientWidth,
    ctx = canvas.getContext("2d"),
    timeMark = this.getTimeMark(width, 0),
    fontMetr = ctx.measureText(timeMark).width;
   
    if (delta > 0) tmDashStep++;
    else tmDashStep--;

    if (tmDashStep > 3 * fontMetr) tmDashStep = 2 * fontMetr;
    else if (tmDashStep < fontMetr * 1.1) tmDashStep = 2 * fontMetr;

    let offs = 10000,
        curIntervSec = (tmInterval.endMs - tmInterval.beginMs) / 1000;

    if (curIntervSec > 86400) offs *= 1000;
    else if (curIntervSec > 3600) offs *= 100;
    else if (curIntervSec < 1) offs /= 1000;
    else if (curIntervSec < 60) offs /= 10;

    if (delta > 0){ 

      tmInterval.beginMs += offs;
      tmInterval.endMs += -offs;
  
      if (tmInterval.beginMs >= tmInterval.endMs){
        let mdl = Math.abs(tmInterval.beginMs + tmInterval.endMs) / 2;
        tmInterval.beginMs = mdl - 10;
        tmInterval.endMs = mdl + 10;
      }
    }
    else{ 
      tmInterval.beginMs += -offs;
      tmInterval.endMs += offs;
    }

    return {tmInterval, tmDashStep};
  }
  
  componentDidMount() {
   
    this.drawCanvas();
  }

  componentDidUpdate() {
   
    this.drawCanvas();
  }

  drawCanvas(){    
    
    const canvas = this._canvasRef;
    
    if (canvas) {
      
      const w = canvas.clientWidth,
            h = canvas.clientHeight,
            ctx = canvas.getContext("2d");
 
      if ((canvas.width != w) || (canvas.height != h)){
        canvas.width = w;
        canvas.height = h;
      }
    
      ctx.clearRect(0, 0, w, h);

      this.drawAxisMark(w, h, ctx);
      
      this.drawSignals(w, h, ctx);   
    }
  }

  drawSignals(width, height, ctx){
      
    let signPnts = this.getSignalPoints(width, height);
   
    const valInterval = this.props.valInterval,
          tmInterval = this.props.tmInterval,
          packetSize = this.props.dataParams.packetSize,
          cycleTimeMs = this.props.dataParams.cycleTimeMs,
          packetTimeMs = packetSize * cycleTimeMs,
          valScale = (valInterval.end - valInterval.begin) / height;

    // int, float
    for (let k in signPnts){
          
      // pass bool 
      if (this.props.signals[k].type == 0) continue; 

      ctx.strokeStyle = this.props.signParams[k].color;
      ctx.fillStyle = this.props.signParams[k].color;
      
      let isFillGraph = this.props.signParams[k].transparent < 1,
          isPaintPnt = (tmInterval.endMs - tmInterval.beginMs) < (packetTimeMs * 5);

      const zonePnts = signPnts[k]; 
     
      for (const pnts of zonePnts){
        
        if (pnts.length == 0) continue; 

        ctx.beginPath();

        ctx.lineWidth = this.props.signParams[k].lineWidth;
        ctx.globalAlpha = 1;
       
        ctx.moveTo(pnts[0].pos, height - pnts[0].value);
        for (let i = 1; i < pnts.length; ++i)
          ctx.lineTo(pnts[i].pos, height - pnts[i].value);
       
        ctx.stroke();    
        
        if (isPaintPnt){
          for (let i = 0; i < pnts.length; ++i){
            ctx.beginPath();
            ctx.arc(pnts[i].pos, height - pnts[i].value, 3, 0, 360);
            ctx.closePath();
            ctx.fill();   
            ctx.stroke();             
          }
        }

        if (isFillGraph){
          
          ctx.beginPath();

          ctx.lineWidth = 1;
          ctx.globalAlpha = this.props.signParams[k].transparent;
      
          let yPos = height;

          if ((valInterval.begin < 0) && (valInterval.end > 0))
            yPos = valInterval.end / valScale;
          else if ((valInterval.begin < 0) && (valInterval.end < 0))
            yPos = 0;
      
          ctx.moveTo(pnts[0].pos, yPos);
          
          let step = 1024.0 / pnts.length,
              cxPos = 0,
              prevxPos = -1;

          for (let i = 0; i < pnts.length; ++i){
                                                              
              if (Math.round(cxPos) > prevxPos){
                  prevxPos = Math.round(cxPos);
                  ctx.lineTo(pnts[i].pos, height - pnts[i].value);
              }
              cxPos += step;
          }

          ctx.lineTo(pnts[pnts.length - 1].pos, yPos);

          ctx.fill();

          ctx.stroke();   
        }
      }     
    }


    // bool
    let signBoolCnt = 0;
    for (let k in signPnts){
          
      // pass int, float 
      if (this.props.signals[k].type != 0) continue;
    
      ctx.strokeStyle = this.props.signParams[k].color;
      ctx.fillStyle = this.props.signParams[k].color;
      ctx.lineWidth = 1;
      ctx.globalAlpha = 1;

      const zonePnts = signPnts[k]; 
     
      for (const pnts of zonePnts){
        
        if (pnts.length == 0) continue; 

        ctx.beginPath();
       
        const psz = pnts.length,
              sDist = 15, 
              sH = 10;
              
        let prevPos = 1, 
            prevVal = pnts[0].value; 
        for (let i = 1; i < psz; ++i){

          if ((prevVal == 0) && ((pnts[i].value > 0) || (i == (psz - 1)))){
           
            ctx.moveTo(pnts[prevPos - 1].pos, height - signBoolCnt * sDist - 1);

            ctx.lineTo(pnts[i].pos, height - signBoolCnt * sDist - 1);

            prevVal = 1;
            prevPos = i;
          }
          else if ((prevVal > 0) && ((pnts[i].value == 0) || (i == (psz - 1)))){
                        
            ctx.rect(pnts[prevPos - 1].pos,
                     height - signBoolCnt * sDist - sH - 1, 
                     pnts[i - 1].pos - pnts[prevPos - 1].pos,
                     sH + 1);
            
            ctx.fill();   

            prevVal = 0;
            prevPos = i;
          }
        }
        ctx.stroke();   
      } 
      ++signBoolCnt;     
    }
  }

  getSignalPoints(width, height){
         
      const tmInterval = this.props.tmInterval,
            valInterval = this.props.valInterval,
            tmScale = (tmInterval.endMs - tmInterval.beginMs) / width,
            valScale = (valInterval.end - valInterval.begin) / height; 
      
      const valMinInterval = valInterval.begin;
      
      const tmMinInterval = tmInterval.beginMs,
            tmMaxInterval = tmInterval.endMs;

      const packetSize = this.props.dataParams.packetSize,
            cycleTimeMs = this.props.dataParams.cycleTimeMs,
            packetTimeMs = packetSize * cycleTimeMs;

      const valPosMem = valMinInterval / valScale;
    
      //////////////////////////////////////////

      let resPnts = {};

      for (let sign in this.props.signals){

        const buffVals = this.props.signals[sign].buffVals,
              buffSz = buffVals.length,
              stype = this.props.signals[sign].type,
              tBool = 0;

        if (buffSz == 0) continue;
      
        let tmZnBegin = buffVals[0].beginTime,
            tmZnEnd = tmZnBegin + packetTimeMs;    
      
        let tmZnEndPrev = tmZnBegin;

        let tmPosMem = [];
        for (let i = 0; i < packetSize; ++i)
          tmPosMem.push((i * cycleTimeMs - tmMinInterval) / tmScale);

        let prevPos = -1,
            iBuf = 0;
        
        let zonePnts = [[]];

        while (tmZnBegin < tmMaxInterval){

          if (tmZnEnd > tmMinInterval){

            const rd = buffVals[iBuf];

            const tmZnBeginMem = tmZnBegin / tmScale;

            if ((tmZnBegin - tmZnEndPrev) > packetTimeMs){
              
              zonePnts.push([]);
            }

            let backZone = zonePnts[zonePnts.length - 1];

            for (let i = 0; i < packetSize; ++i){

              let pnt = {};
              pnt.pos = tmPosMem[i] + tmZnBeginMem;
          
              if (Math.round(pnt.pos) > prevPos){
                prevPos = Math.round(pnt.pos);

                if (stype != tBool)
                  pnt.value = rd.vals[i] / valScale - valPosMem;
                else
                  pnt.value = rd.vals[i];
                
                backZone.push(pnt);
              }           
          }
        }
        tmZnEndPrev = tmZnEnd;

        ++iBuf;
        if (iBuf == buffSz) break;

        tmZnBegin = buffVals[iBuf].beginTime;
        tmZnEnd = tmZnBegin + packetTimeMs;        
      }

      resPnts[sign] = zonePnts;
    }

    return resPnts;
  }

  drawAxisMark(width, height, ctx){
    
    ctx.beginPath();  

    ctx.lineWidth = 1;
    ctx.strokeStyle = '#808080';        
    ctx.globalAlpha = 0.2;
  
    const tmAxisMark = this.getTimePosMark();    
    for(let pos of tmAxisMark){

      ctx.moveTo(pos, 0);
      ctx.lineTo(pos, height);
    }

    const valAxisMark =  this.getValPosMark();    
    for(let pos of valAxisMark){
      
      ctx.moveTo(0, pos);
      ctx.lineTo(width, pos);
    }

    ctx.stroke();
  }
    
  getTimeMark(width, offs){
      
    const tmInterval = this.props.tmInterval,
          curIntervSec = (tmInterval.endMs - tmInterval.beginMs) / 1000,
          tmScale = (tmInterval.endMs - tmInterval.beginMs) / width,   
          dt = new Date(tmInterval.beginMs + tmScale * offs);
  
    let timeMark = '';

    if (curIntervSec > 86400){     
      let options = { hour12 : false, day: '2-digit', month: '2-digit', year: '2-digit', hour: '2-digit', minute: '2-digit' };
      timeMark = dt.toLocaleTimeString('en-US', options);
    }
    else{
      let options = { hour12 : false, hour: '2-digit', minute: '2-digit', second: '2-digit' };
      timeMark = dt.toLocaleTimeString('en-US', options).split(' ')[0];  
    
      if (curIntervSec < 60)
        timeMark += ":" + dt.getMilliseconds();
    }
        
     return timeMark;
  }

  getTimePosMark(){

    const canvas = this._canvasRef,
          width = canvas.clientWidth;        

    let {tmOffsPos, tmDashStep} = this.props.axisParams,
        offs = tmOffsPos % tmDashStep,
        mark = [];

    while (offs < width){
		
      mark.push(offs);

      offs += tmDashStep;
    }
    
    return mark;
  }

  getValPosMark(){

    const canvas = this._canvasRef,
          height = canvas.clientHeight;

    let {valOffsPos, valDashStep} = this.props.axisParams,
        offs = valOffsPos % valDashStep,
        mark = [];	

    while (offs < height){
    
      mark.push(offs);
      offs += valDashStep;
    }

    return mark;
  }

  render(){

    return <canvas style={ style }
                   ref={ el => this._canvasRef = el }
                   onMouseMove={ this.handleMouseMove } 
                   onWheel={ this.handleWheel } 
                   onDragOver = { (e) => e.preventDefault() }
                   onDrop = { (e) => { e.preventDefault();
                                       let module = e.dataTransfer.getData('text').split('.')[0],
                                           name = e.dataTransfer.getData('text').split('.')[1];
                                       this.props.onDrop(name, module);
                                     } } >
           </canvas>
  }
}

const style = {  
  backgroundColor : "white",
  height: "100%",
  width: "100%",
}

Plot.propTypes = { 

};