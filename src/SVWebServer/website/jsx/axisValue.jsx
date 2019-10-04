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
  
    this._curOffsPos = 0;
    this._curDashStep = 50;
    this._maxDashStep = 100;
    this._minDashStep = 50;
  }
   
  handleMouseMove(event) {
    
    let canvas = this._canvasRef;

    if (!canvas || !event.buttons) return;

    let diff = event.nativeEvent.movementY;
      
    this._curOffsPos += diff;

    if (this._curOffsPos > this._curDashStep) 
      this._curOffsPos = 0;
    else if (this._curOffsPos < 0) 
      this._curOffsPos = this._curDashStep; 
        
    let valInterval = this.props.valInterval;

    let height = canvas.clientHeight,
        valScale = (valInterval.end - valInterval.begin) / height;

    valInterval.begin += valScale * diff;
    valInterval.end += valScale * diff;    
    
    this.props.onChange(valInterval);
  }

  handleWheel(e){

    let delta = e.deltaY || e.detail || e.wheelDelta;

    this.scale(delta);
  }
  
  scale(delta){
    
    if (delta > 0) this._curDashStep++;
    else this._curDashStep--;

    if (this._curDashStep > this._maxDashStep) 
      this._curDashStep = this._minDashStep;
    else if (this._curDashStep < this._minDashStep) 
      this._curDashStep = this._maxDashStep;
   
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

    this.props.onChange(valInterval);
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
    
    let offs = this._curOffsPos % this._curDashStep;
    while (offs < height){
      
      ctx.moveTo(width - 5, offs);
      ctx.lineTo(width, offs);
      
      offs += this._curDashStep;
    }
    
    ctx.stroke();
  }
  
  drawValMark(width, height, ctx){
   
    ctx.font = "normal 9pt Arial";

    let offs = this._curOffsPos % this._curDashStep;
    while (offs < height){
   
      let valMark = this.getValMark(height, offs),
          fontMetr = ctx.measureText(valMark).width,
          xp = Math.max(0, width - fontMetr - 15);

      ctx.fillText(valMark, xp, offs + 5);
   
      offs += this._curDashStep; 
    }
  }
  
  getValMark(height, offs){
     
    let valInterval = this.props.valInterval,
        scale = (valInterval.end - valInterval.begin) / height,
        vl = valInterval.end - scale * offs;

    let sign = vl > 0 ? 1 : -1;

    vl *= sign;

    let diap = Math.abs(valInterval.end - valInterval.begin);

    if (diap > 100) vl = parseInt(vl + 0.5);
    else if (diap > 10) vl = parseInt(vl * 10 + 0.5) / 10.0;
    else if (diap > 1) vl = parseInt(vl * 100 + 0.5) / 100.0;
    else vl = parseInt(vl * 1000 + 0.5) / 1000.0;

    return (vl * sign).toString();
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
  _curOffsPos : PropTypes.number,
  _curDashStep : PropTypes.number,
  _maxDashStep : PropTypes.number,
  _minDashStep : PropTypes.number,
};