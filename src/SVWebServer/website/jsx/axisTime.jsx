/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";


export default 
class AxisTime extends React.Component {

  constructor(props){
    super(props);

    this._canvasRef = null;
   
    this.handleMouseMove = this.handleMouseMove.bind(this);
    this.handleWheel = this.handleWheel.bind(this);
  
  }
    
  handleMouseMove(event) {
    
    const canvas = this._canvasRef;

    if (!canvas || !event.buttons) return;
    
    let tmInterval = this.props.tmInterval;

    const width = canvas.clientWidth,
          tmScale = (tmInterval.endMs - tmInterval.beginMs) / width,
          diffPos = event.nativeEvent.movementX;
      
    let {tmOffsPos, tmDashStep, ...exPrms} = this.props.axisParams;

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

  handleWheel(e){

    const delta = e.deltaY || e.detail || e.wheelDelta;

    const canvas = this._canvasRef,
          width = canvas.clientWidth,
          ctx = canvas.getContext("2d"),
          timeMark = this.getTimeMark(width, 0),
          fontMetr = ctx.measureText(timeMark).width;
    
    let {tmDashStep, ...exParams} = this.props.axisParams;

    if (delta > 0) tmDashStep++;
    else tmDashStep--;

    if (tmDashStep > 3 * fontMetr) tmDashStep = 2 * fontMetr;
    else if (tmDashStep < fontMetr * 1.1) tmDashStep = 2 * fontMetr;

    let tmInterval = this.props.tmInterval,
        offs = 10000,
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
    
    this.props.onChange(tmInterval, {tmDashStep, ...exParams});
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

  drawDashLines(width, ctx){

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
  
  drawTimeMark(width, height, ctx){
      
    ctx.font = "normal 9pt Arial";

    let {tmOffsPos, tmDashStep} = this.props.axisParams,
        offs = tmOffsPos % tmDashStep;
    while (offs < width){
   
      let timeMark = this.getTimeMark(width, offs),
          fontMetr = ctx.measureText(timeMark).width;

      ctx.fillText(timeMark, offs - fontMetr / 2, height / 2);
   
      offs += tmDashStep; 
    }
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

  getPosMark(){

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

  render(){

    return <canvas style={ style }
                   ref={ el => this._canvasRef = el }
                   onMouseMove={ this.handleMouseMove } 
                   onWheel={ this.handleWheel } >
           </canvas>
  }
}

const style = {  
  height: "50px",
  width: "100%",
}

AxisTime.propTypes = { 
  _curOffsPos : PropTypes.number,
  _curDashStep : PropTypes.number,
};