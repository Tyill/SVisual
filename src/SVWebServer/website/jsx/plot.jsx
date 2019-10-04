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
    
    let canvas = this._canvasRef;

    if (!canvas || !event.buttons) return;

    let diff = event.nativeEvent.movementY;
        
    let valInterval = this.props.valInterval;

    let height = canvas.clientHeight,
        valScale = (valInterval.end - valInterval.begin) / height;

    valInterval.begin += valScale * diff;
    valInterval.end += valScale * diff;   
    
    //////////////////////////////////////

    let tmInterval = this.props.tmInterval;

    let width = canvas.clientWidth,
        tmScale = (tmInterval.endMs - tmInterval.beginMs) / width;
   
    let diffPos = event.nativeEvent.movementX;
      
    let offs = -tmScale * diffPos - 1;
    if (diffPos < 0) 
       offs = -tmScale * diffPos + 1;
  
    tmInterval.beginMs += offs;
    tmInterval.endMs += offs;    
      
    this.props.onChange(tmInterval, valInterval);
  }

  handleWheel(e){

    let delta = e.deltaY || e.detail || e.wheelDelta;

    //this.scale(delta);
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

      this.drawAxisLines(w, h, ctx);
      
    }

  }

  drawAxisLines(width, height, ctx){
    
    ctx.lineWidth = 1;
    ctx.strokeStyle = '#000000';        

    ctx.beginPath();  

    let tmAxisLines = this.props.tmAxisLines;    
    for(pos of tmAxisLines){

      ctx.moveTo(pos, 0);
      ctx.lineTo(pos, height);
    }

    let valAxisLines = this.props.valAxisLines;    
    for(pos of valAxisLines){

      ctx.moveTo(0, pos);
      ctx.lineTo(width, pos);
    }

    ctx.stroke();
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