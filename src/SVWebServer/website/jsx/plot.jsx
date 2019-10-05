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

    //this.scale(delta);
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