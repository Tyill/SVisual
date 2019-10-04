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
  
    this._curOffsPos = 0;
    this._curDashStep = 100;
  }
   
  handleMouseMove(event) {
    
    let canvas = this._canvasRef;

    if (!canvas || !event.buttons) return;
    
    let tmInterval = this.props.tmInterval;

    let width = canvas.clientWidth,
        tmScale = (tmInterval.endMs - tmInterval.beginMs) / width;
   
    let diffPos = event.nativeEvent.movementX;
      
    this._curOffsPos += diffPos;
    
    if (this._curOffsPos_ > this._curDashStep) 
      this._curOffsPos_ = 0;
    else if (this._curOffsPos_ < 0) 
      this._curOffsPos_ = this._curDashStep; 
      
    let offs = -tmScale * diffPos - 1;
    if (diffPos < 0) 
       offs = -tmScale * diffPos + 1;
  
    tmInterval.beginMs += offs;
    tmInterval.endMs += offs;    
    
    this.props.onChange(tmInterval);
  }

  handleWheel(e){

    let delta = e.deltaY || e.detail || e.wheelDelta;

    this.scale(delta);
  }
  
  scale(delta){
    
    if (delta > 0) this._curDashStep++;
    else this._curDashStep--;

    let canvas = this._canvasRef,
        w = canvas.clientWidth,
        ctx = canvas.getContext("2d"),
        timeMark = this.getTimeMark(w, 0),
        fontMetr = ctx.measureText(timeMark).width;

    if (this._curDashStep > 3 * fontMetr) this._curDashStep = 2 * fontMetr;
    else if (this._curDashStep < fontMetr * 1.1) this._curDashStep = 2 * fontMetr;

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

    ////////////////////////////////////

  //   QVector<int> mark;
	
	// int w = width();

	// int cWidth = curOffsPos_ % curDashStep_;

	// while (cWidth < w){
		
	// 	mark.push_back(cWidth);

	// 	cWidth += curDashStep_;
	// }

	// return mark;

    this.props.onChange(tmInterval, );
  }

  componentDidMount() {
   
    this.drawCanvas();
  }

  componentDidUpdate() {
   
    this.drawCanvas();
  }

  drawCanvas(){    
    
    let canvas = this._canvasRef;
    
    if (canvas) {
      
      let w = canvas.clientWidth,
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
    
    let offs = this._curOffsPos % this._curDashStep;
    while (offs < width){
      
      ctx.moveTo(offs, 0);
      ctx.lineTo(offs, 5);
      
      offs += this._curDashStep;
    }
    
    ctx.stroke();
  }
  
  drawTimeMark(width, height, ctx){
      
    ctx.font = "normal 9pt Arial";

    let offs = this._curOffsPos % this._curDashStep;
    while (offs < width){
   
      let timeMark = this.getTimeMark(width, offs),
          fontMetr = ctx.measureText(timeMark).width;

      ctx.fillText(timeMark, offs - fontMetr / 2, height / 2);
   
      offs += this._curDashStep; 
    }
  }
  
  getTimeMark(width, offs){
      
    let tmInterval = this.props.tmInterval,

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