/* eslint-disable no-unused-vars */
// @flow
import React from "react"

/*::
import type { axisParamsType } from "./graph.jsx";

export
type valIntervalType = {begin : number, end : number}

type Props = {
  valInterval : valIntervalType,
  axisParams : axisParamsType,
  onChange : (valIntervalType, axisParamsType) => void; 
}
*/

export default
class AxisValue extends React.Component/*::<Props>*/ {
    
  /*::
  _canvasRef : any;
  handleMouseMove: (event : any) => void;
  handleWheel: (event : any) => void;
  */

  constructor(props/*:: : Props*/){
    super(props);

    this._canvasRef = null;
   
    this.handleMouseMove = this.handleMouseMove.bind(this);
    this.handleWheel = this.handleWheel.bind(this);
  
  }
   
  handleMouseMove(event/*:: : any*/) {
    
    const canvas = this._canvasRef;

    if (!canvas || !event.buttons) return;

    const diff = event.nativeEvent.movementY;
      
    let {valOffsPos, valDashStep, ...exPrms} /*:: : any */ = this.props.axisParams;

    valOffsPos += diff;

    if (valOffsPos > valDashStep) 
      valOffsPos = 0;
    else if (valOffsPos < 0) 
      valOffsPos = valDashStep; 
        
    let valInterval = this.props.valInterval;

    const height = canvas.clientHeight,
          valScale = (valInterval.end - valInterval.begin) / height;

    valInterval.begin += valScale * diff;
    valInterval.end += valScale * diff;    
       
    this.props.onChange(valInterval, {valOffsPos, valDashStep, ...exPrms});
  }

  handleWheel(event/*:: : any*/){

    const delta = -(event.deltaY || event.detail || event.wheelDelta);

    let {valDashStep, minValDashStep, maxValDashStep, ...exParams}  /*:: : any */ = this.props.axisParams;

    if (delta > 0) valDashStep++;
    else valDashStep--;

    if (valDashStep > maxValDashStep) 
      valDashStep = minValDashStep;
    else if (valDashStep < minValDashStep) 
      valDashStep = maxValDashStep;
   
    let valInterval = this.props.valInterval,
        curInterval = valInterval.end - valInterval.begin,
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

    this.props.onChange(valInterval, {valDashStep, minValDashStep, maxValDashStep, ...exParams});
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

      this.drawDashLines(w, h, ctx);

      this.drawValMark(w, h, ctx);
    }

  }

  drawDashLines(width/*:: : number*/, height/*:: : number*/, ctx/*:: : any*/){
    
    ctx.lineWidth = 1;
    ctx.strokeStyle = '#000000';        

    ctx.beginPath();
    ctx.moveTo(width, 0);
    ctx.lineTo(width, height);    
    
    let {valOffsPos, valDashStep} = this.props.axisParams,
        offs = valOffsPos % valDashStep;
    while (offs < height){
      
      ctx.moveTo(width - 5, offs);
      ctx.lineTo(width, offs);
      
      offs += valDashStep;
    }
    
    ctx.stroke();
  }
  
  drawValMark(width/*:: : number*/, height/*:: : number*/, ctx/*:: : any*/){
   
    ctx.font = "normal 9pt Arial";

    let {valOffsPos, valDashStep} = this.props.axisParams,
        offs = valOffsPos % valDashStep;
    while (offs < height){
   
      let valMark = this.getValMark(height, offs),
          fontMetr = ctx.measureText(valMark).width,
          xp = Math.max(0, width - fontMetr - 15);

      ctx.fillText(valMark, xp, offs + 5);
   
      offs += valDashStep; 
    }
  }
  
  getValMark(height/*:: : number*/, offs/*:: : number*/){
     
    let valInterval = this.props.valInterval,
        scale = (valInterval.end - valInterval.begin) / height,
        vl = valInterval.end - scale * offs;

    const sign = vl > 0 ? 1 : -1,
          diap = Math.abs(valInterval.end - valInterval.begin);

    vl *= sign;

    if (diap > 100) vl = parseInt(vl + 0.5);
    else if (diap > 10) vl = parseInt(vl * 10 + 0.5) / 10.0;
    else if (diap > 1) vl = parseInt(vl * 100 + 0.5) / 100.0;
    else vl = parseInt(vl * 1000 + 0.5) / 1000.0;

    return (vl * sign).toString();
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

