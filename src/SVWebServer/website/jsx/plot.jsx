/* eslint-disable no-unused-vars */

import React from "react"
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

    const delta = e.deltaY || e.detail || e.wheelDelta;

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
      valInterval.first += offs;
      valInterval.second -= offs;

      if (valInterval.first >= valInterval.second)
        valInterval.first = valInterval.second - 0.1;
    }
    else{ 
      valInterval.first -= offs;
      valInterval.second += offs;
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
      
    }

  }

  drawAxisMark(width, height, ctx){
    
    ctx.lineWidth = 1;
    ctx.strokeStyle = '#000000';        

    ctx.beginPath();  

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
                   onWheel={ this.handleWheel } >
           </canvas>
  }
}

const style = {  
  border: "1px solid blue",
  height: "100%",
  width: "100%",
}

Plot.propTypes = { 

};