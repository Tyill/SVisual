/* eslint-disable no-unused-vars */
// @flow
import React from "react"

/*::
import type { axisParamsType } from "./graph.jsx";

export
type tmIntervalType = {beginMs : number, endMs : number}

type Props = {
  tmInterval : tmIntervalType,
  axisParams : axisParamsType,
  onChange : (tmIntervalType, axisParamsType) => void; 
}
*/

export default 
class AxisTime extends React.Component/*::<Props>*/ {

  /*::
  _canvasRef : any;
  handleMouseMove: (event : any) => void;
  handleWheel: (iGraph : number) => void;
  */

  constructor(props/*:: : Props*/){
    super(props );

    this._canvasRef = null;
   
    this.handleMouseMove = this.handleMouseMove.bind(this);
    this.handleWheel = this.handleWheel.bind(this);
  
  }
    
  handleMouseMove(event /*:: : any*/) {
    
    const canvas = this._canvasRef;

    if (!canvas || !event.buttons) return;
    
    let tmInterval = this.props.tmInterval;

    const width = canvas.clientWidth,
          tmScale = (tmInterval.endMs - tmInterval.beginMs) / width,
          diffPos = event.nativeEvent.movementX;

    let {tmOffsPos, tmDashStep, ...exPrms} /*:: : any*/ = this.props.axisParams;

    tmOffsPos += diffPos;
    
    if (tmOffsPos > tmDashStep) 
      tmOffsPos = 0;
    else if (tmOffsPos < 0) 
      tmOffsPos = tmDashStep; 
      
    let offs = -tmScale * diffPos - 1;
    if (diffPos < 0) 
       offs = -tmScale * diffPos + 1;
  
    tmInterval.beginMs += offs;
    tmInterval.endMs += offs;  
    
    this.props.onChange(tmInterval, {tmOffsPos, tmDashStep, ...exPrms});
  }

  handleWheel(event /*:: : any*/){

    const delta = -(event.deltaY || event.detail || event.wheelDelta);

    let {tmDashStep, ...exParams} /*:: : any*/ = this.props.axisParams,        
        tmInterval = this.props.tmInterval;
    
    const canvas = this._canvasRef,
          width = canvas.clientWidth,
          ctx = canvas.getContext("2d"),
          timeMark = AxisTime.getTimeMark(width, 0, tmInterval),
          fontMetr = ctx.measureText(timeMark).width;

    ({tmInterval, tmDashStep} = AxisTime.scaleByTime(fontMetr, delta, this.props.axisParams, tmInterval));
      
    this.props.onChange(tmInterval, {tmDashStep, ...exParams});
  }
  
  static scaleByTime(fontMetr/*:: :number*/, delta/*:: :number*/, axisParams/*:: :axisParamsType*/, tmInterval/*:: :tmIntervalType*/)
  /*:: : {tmInterval : tmIntervalType, tmDashStep : number } */
  {
    let tmDashStep = axisParams.tmDashStep;

    if (delta == 0)
      return {tmInterval, tmDashStep};
      
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

      this.drawDashLines(w, ctx);

      this.drawTimeMark(w, h, ctx);
    }

  }

  drawDashLines(width /*:: : number*/, ctx/*:: : any*/){

    ctx.lineWidth = 1;
    ctx.strokeStyle = '#000000';        

    ctx.beginPath();
    ctx.moveTo(0, 0);
    ctx.lineTo(width, 0);    
    
    let {tmOffsPos, tmDashStep} = this.props.axisParams,
        offs = tmOffsPos % tmDashStep;
    while (offs < width){
      
      ctx.moveTo(offs, 0);
      ctx.lineTo(offs, 5);
      
      offs += tmDashStep;
    }
    
    ctx.stroke();
  }
  
  drawTimeMark(width /*:: : number*/, height /*:: : number*/, ctx /*:: : any*/){
      
    ctx.font = "normal 9pt Arial";

    let {tmOffsPos, tmDashStep} = this.props.axisParams,
        offs = tmOffsPos % tmDashStep;
    while (offs < width){
   
      let timeMark = AxisTime.getTimeMark(width, offs, this.props.tmInterval),
          fontMetr = ctx.measureText(timeMark).width;

      ctx.fillText(timeMark, offs - fontMetr / 2, height / 2);
   
      offs += tmDashStep; 
    }
  }
  
  static getTimeMark(width /*:: : number*/, offs /*:: : number*/, tmInterval/*:: : tmIntervalType*/){
      
    const curIntervSec = (tmInterval.endMs - tmInterval.beginMs) / 1000,
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

  render(){

    const style = {  
      height: "100%",
      width: "100%",
    }

    return <canvas style={ style }
                   ref={ el => this._canvasRef = el }
                   onMouseMove={ this.handleMouseMove } 
                   onWheel={ this.handleWheel } >
           </canvas>
  }
}

