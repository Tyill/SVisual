/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";


export default 
class AxisTime extends React.Component {

  constructor(props){
    super(props);

    this._canvasRef = null;
   
    this.handleMouseMove = this.handleMouseMove.bind(this);
  
    this._curOffsPos = 0;
    this._curDashStep = 100;
    this._isMouseDown = false;
  }
   
  handleMouseMove(event) {
    
    let canvas = this._canvasRef;

    if (!canvas || !this._isMouseDown) return;
    
    let tmInterval = this.props.tmInterval;

    let width = canvas.clientWidth,
        tmScale = (tmInterval.beginMs - tmInterval.endMs) / width;
   
    let diffPos = event.nativeEvent.movementX;
       
    this._curOffsPos += diffPos;
    
    if (this._curOffsPos_ > this._curDashStep) 
      this._curOffsPos_ = 0;
    else if (this._curOffsPos_ < 0) 
      this._curOffsPos_ = this._curDashStep; 
      
    let offs = -tmScale * diffPos - 1;
    if (diffPos < 0) 
       offs = -tmScale * diffPos + 1;
  
    tmInterval.first += offs;
    tmInterval.second += offs;    
    
    this.props.onChange(tmInterval);
  }

  componentDidMount() {
    
    console.log("mount");

    this.drawCanvas();
  }

  componentDidUpdate() {

    console.log("update");

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
    
      ctx.clearRect(0, 0, canvas.width, canvas.height);

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

        curIntervSec = (tmInterval.beginMs - tmInterval.endMs) / 1000,

        tmScale = (tmInterval.beginMs - tmInterval.endMs) / width,
   
        dt = new Date(tmInterval.beginMs - tmScale * offs);
  
    let timeMark = '';

    if (curIntervSec > 86400){     
      let options = { hour12 : false, day: '2-digit', month: '2-digit', year: '2-digit', hour: '2-digit', minute: '2-digit' };
      timeMark = dt.toLocaleTimeString('en-US', options).split(' ');
      timeMark.pop();
      timeMark = timeMark.join('');
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
                   onMouseDown={ e => this._isMouseDown = true }
                   onMouseUp={ e => this._isMouseDown = false } >
           </canvas>
  }
}

const style = {  
  height: "50px",
  width: "100%",
}

AxisTime.propTypes = {
   // scheme_: PropTypes.object,
};