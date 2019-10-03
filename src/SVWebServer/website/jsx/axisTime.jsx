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
    this._curDashStep = 150;
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

      this.drawDashLines(canvas);

      this.drawTimeMark(canvas);
    }

  }

  drawDashLines(canvas){

    let w = canvas.clientWidth,
        h = canvas.clientHeight,
        ctx = canvas.getContext("2d");
  
    ctx.lineWidth = 1;
    ctx.strokeStyle = '#000000';        

    ctx.beginPath();
    ctx.moveTo(0, 0);
    ctx.lineTo(w, 0);    
    
    let offs = this._curOffsPos % this._curDashStep;
    while (offs < w){
      
      ctx.moveTo(offs, 0);
      ctx.lineTo(offs, 5);
      
      offs += this._curDashStep;
    }
    
    ctx.stroke();
  }
  
  drawTimeMark(canvas){
  
    let w = canvas.clientWidth,
        h = canvas.clientHeight,
        ctx = canvas.getContext("2d");
    
    let offs = this._curOffsPos % this._curDashStep + this._curDashStep;
  //  while (offs < w){
   
      let timeMark = this.getTimeMark(w, offs),
          fontMetr = ctx.measureText(timeMark).width;

      ctx.fillText(timeMark, offs - fontMetr / 2, h / 2);
   
  //    offs += this._curDashStep; 
 //   }
  }
  
  getTimeMark(width, offs){
      
    let tmInterval = this.props.tmInterval,

        curIntervSec = (tmInterval.beginMs - tmInterval.endMs) / 1000,

        tmScale = (tmInterval.beginMs - tmInterval.endMs) / width,
   
        dt = new Date(tmInterval.beginMs + tmScale * offs);
  
    let options = { hour: '2-digit', minute: '2-digit', second: '2-digit' };
    let timeMark = dt.toLocaleTimeString('en-US', options).split(' ')[0];

    timeMark += ":" + dt.getMilliseconds();

  //  if (curIntervSec > 86400){
     
      options = { day: '2-digit', month: '2-digit', year: '2-digit', hour: '2-digit', minute: '2-digit' };
      timeMark = dt.toLocaleTimeString('en-US', options);

     // timeMark = dt.toString("dd.MM.yy hh:mm");
  //  }
    // else if (curIntervSec > 60)
    //   timeMark = dt.toString("hh:mm:ss");
      
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