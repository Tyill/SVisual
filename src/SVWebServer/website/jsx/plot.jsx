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

    
  }

  handleWheel(e){

    let delta = e.deltaY || e.detail || e.wheelDelta;

    //this.scale(delta);
  }
  
  componentDidMount() {
   
  //  this.drawCanvas();
  }

  componentDidUpdate() {
   
  //  this.drawCanvas();
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

      this.drawDashLines(w, h, ctx);
      
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