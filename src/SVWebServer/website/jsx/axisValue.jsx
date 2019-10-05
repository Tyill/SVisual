/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";

export default
class AxisValue extends React.Component {
    
  constructor(props){
    super(props);

    this._canvasRef = null;
   
    this.handleMouseMove = this.handleMouseMove.bind(this);
    this.handleWheel = this.handleWheel.bind(this);
  
  }
   
  handleMouseMove(event) {
    
    const canvas = this._canvasRef;

    if (!canvas || !event.buttons) return;

    const diff = event.nativeEvent.movementY;
      
    let {valOffsPos, valDashStep, ...exPrms} = this.props.axisParams;

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

  handleWheel(e){

    const delta = e.deltaY || e.detail || e.wheelDelta;

    this.scale(delta);
  }
  
  scale(delta){
    
    let {valDashStep, minValDashStep, maxValDashStep, ...exParams} = this.props.axisParams;

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
      valInterval.first += offs;
      valInterval.second -= offs;

      if (valInterval.first >= valInterval.second)
        valInterval.first = valInterval.second - 0.1;
    }
    else{ 
      valInterval.first -= offs;
      valInterval.second += offs;
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

  drawDashLines(width, height, ctx){
    
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
  
  drawValMark(width, height, ctx){
   
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
  
  getValMark(height, offs){
     
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

  getPosMark(){

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
                   onWheel={ this.handleWheel } >
           </canvas>
  }
}

const style = {  
  height: "100%",
  width: "100%",
}

AxisValue.propTypes = { 
  // _curOffsPos : PropTypes.number,
  // _curDashStep : PropTypes.number,
  // _maxDashStep : PropTypes.number,
  // _minDashStep : PropTypes.number,
};