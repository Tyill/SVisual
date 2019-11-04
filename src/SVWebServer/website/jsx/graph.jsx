/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";
import {Container, Row, Col, Button } from "react-bootstrap";
import AxisValue from "./axisValue.jsx"
import AxisTime from "./axisTime.jsx"
import Plot from "./plot.jsx"
import "../css/fontello.css";

export default
class Graph extends React.Component {

  constructor(props){
    super(props);   
     
    let axisParams = { valOffsPos : 0,
                       valDashStep : 100,  
                       tmOffsPos : 0,
                       tmDashStep : 100,
                       minValDashStep : 50,
                       maxValDashStep : 100}

    this.state = {tmInterval : { beginMs : Date.now(), endMs : Date.now() + 3.6e4}, 
                  valInterval : { begin : 0, end : 1000},
                  axisParams,                  
                 };   

    this._signParams = {};
    this._isPlay = true;
    this._isAutoResize = true;

    this.handlePlotChange = this.handlePlotChange.bind(this); 
    this.handleAxisTimeChange = this.handleAxisTimeChange.bind(this);    
    this.handleAxisValueChange = this.handleAxisValueChange.bind(this);    
    
    this.handleAddSignal = this.handleAddSignal.bind(this); 
    this.handleDelSignal = this.handleDelSignal.bind(this);    

    this.handleResizeFull = this.handleResizeFull.bind(this);    
    this.handleResizeByValue = this.handleResizeByValue.bind(this);    
    this.handleResizeByTime = this.handleResizeByTime.bind(this);    
    this.handleChangeColor = this.handleChangeColor.bind(this);    
    this.handleAutoResize = this.handleAutoResize.bind(this);   
    this.handlePlay = this.handlePlay.bind(this); 
    this.handleClose = this.handleClose.bind(this);
  }

  handleAxisTimeChange(tmInterval, axisParams){
    
    this._isPlay = false;

    this.setState({tmInterval, axisParams });
  }

  handleAxisValueChange(valInterval, axisParams){
    
    this._isPlay = false;

    this.setState({valInterval, axisParams });
  }

  handlePlotChange(tmInterval, valInterval, axisParams){
        
    this._isPlay = false;

    this.setState({tmInterval, valInterval, axisParams});
  }

  handleAddSignal(name, module){

    this.props.onAddSignal(this.props.iGraph, name + module);
  }

  handleDelSignal(name, module){
    
    delete this._signParams[name + module]
   
    this.props.onDelSignal(this.props.iGraph, name + module);
  }
 
  handleResizeFull(){

    const tmInterval = this.calcTimeInterval();

    this.state.tmInterval = tmInterval;

    const valInterval = this.calcValueInterval();

    this.setState( { tmInterval,
                     valInterval, 
                     axisParams : this.state.axisParams});
  }
  
  handleResizeByValue(){

    const valInterval = this.calcValueInterval();

    this.setState( { tmInterval : this.state.tmInterval,
                     valInterval, 
                     axisParams : this.state.axisParams});
  }
  
  handleResizeByTime(){

    const tmInterval = this.calcTimeInterval();

    this.setState( { tmInterval,
                     valInterval : this.state.valInterval, 
                     axisParams : this.state.axisParams});
  }

  calcTimeInterval(){

    const signals = this.props.signals;

    let minTime = Number.MAX_VALUE,
        maxTime = -Number.MAX_VALUE;
    for (const k in signals){

      const sign = signals[k];
      
      if (!sign.buffVals.length) continue;

      if (sign.buffVals[0].beginTime < minTime)
        minTime = sign.buffVals[0].beginTime;

      if (sign.buffVals[sign.buffVals.length - 1].beginTime > maxTime)
        maxTime = sign.buffVals[sign.buffVals.length - 1].beginTime;
    }

    const cyclePacket = this.props.dataParams.packetSize * this.props.dataParams.cycleTimeMs;
    
    let tmInterval = { beginMs : minTime, endMs : maxTime + cyclePacket};
    if ((minTime == Number.MAX_VALUE) || (maxTime == -Number.MAX_VALUE))
      tmInterval = this.state.tmInterval;

    return tmInterval;
  }

  calcValueInterval(){

    const signals = this.props.signals;

    const tmInterval = this.state.tmInterval;

    let minValue = Number.MAX_VALUE, 
        maxValue = -Number.MAX_VALUE;        
    for (const k in signals){

      const sign = signals[k];

      if (sign.type == 0) continue; // bool pass

      for (const vals of sign.buffVals){
        
        if ((tmInterval.beginMs < vals.beginTime) &&
            (vals.beginTime < tmInterval.endMs)){

          for (const v of vals.vals){
  
            if (v < minValue)
              minValue = v;
            if (v > maxValue)
              maxValue = v;
          }
        }
      }
    }

    let valInterval = { begin : minValue, end : maxValue };
    if ((minValue == Number.MAX_VALUE) || (maxValue == -Number.MAX_VALUE))
      valInterval = this.state.valInterval;

    return valInterval;
  }
  
  handleChangeColor(){

    for (let k in this._signParams){
    
      let prms = this._signParams[k];
      prms.color = '#'+Math.floor(Math.random()*16777215).toString(16);
    }
  }

  handleAutoResize(e){

    this._isAutoResize = !this._isAutoResize;
  }
  
  handlePlay(e){

    this._isPlay = !this._isPlay; 
  }

  handleClose(){

    this.props.onCloseGraph(this.props.iGraph);
  }

  render(){
    
    let signals = this.props.signals,
        legend = [];
    for (let k in signals){

      if (!this._signParams[k]){ 
        this._signParams[k] = { lineWidth : 2,
                                transparent : 0.5,
                                color : '#'+Math.floor(Math.random()*16777215).toString(16) };
      }
      
      legend.push(
        <p key = {legend.length} 
           onClick = { () => this.handleDelSignal(signals[k].name, signals[k].module) } 
           style = {{ marginLeft : 10,
                      marginTop : 20 * legend.length,                                  
                      position : "absolute", 
                      cursor: "default",                                              
                      color : this._signParams[k].color }}>
          {signals[k].name}
        </p>
      );
    }

    if (this._isPlay && legend.length){
               
      const tmInterval = this.calcTimeInterval();
     
      const cyclePacket = this.props.dataParams.packetSize * this.props.dataParams.cycleTimeMs;
     
      this.state.tmInterval.beginMs += cyclePacket;
      this.state.tmInterval.endMs = tmInterval.endMs;
     
      if ((this.state.tmInterval.endMs -  this.state.tmInterval.beginMs) < 1000)
        this.state.tmInterval.beginMs = this.state.tmInterval.endMs - 1000;
        
      if (this._isAutoResize){
        const valInterval = this.calcValueInterval();
        this.state.valInterval = valInterval;
      }
    }

    return (
      <Container>
        <Row noGutters={true} style={{ borderRadius: "3px 3px 0px 0px", padding : "5px", backgroundColor : "silver"}} >
          <Col style = {{ maxWidth : "50px" }} />
          <Col className="col">
           <Button size="sm" className= { "icon-resize-full-alt"} style = {buttonStyle}
                   onClick = {this.handleResizeFull} />
           <Button size="sm" className= { "icon-resize-vertical"} style = {buttonStyle}
                   onClick = {this.handleResizeByValue} />
           <Button size="sm" className= { "icon-resize-horizontal"} style = {buttonStyle}
                   onClick = {this.handleResizeByTime} />
           <Button size="sm" className= { "icon-brush"} style = {buttonStyle}
                   onClick = {this.handleChangeColor} />
           <Button size="sm" variant = { this._isAutoResize ? "light" : "primary" } 
                   className= { "icon-font"} style = {buttonStyle}
                   onClick = {this.handleAutoResize} />
           <Button size="sm" variant = { this._isPlay ? "light" : "primary" }
                   className= { this._isPlay ? "icon-pause" : "icon-play" } style = {buttonStyle}
                   onClick = {this.handlePlay} /> 
            <Button className="close" onClick = { this.handleClose } > 
                <span>&times;</span>
            </Button>
           </Col>          
        </Row>
        <Row noGutters={true} style={{ paddingRight : "5px", backgroundColor : "silver"}}>
          <Col style = {{ maxWidth : "50px", height: "250px" }}>
            <AxisValue valInterval= { this.state.valInterval}
                       axisParams= { this.state.axisParams}
                       onChange = { this.handleAxisValueChange } />    
          </Col>
          <Col className="col" style={{height: "250px"}}>
            {legend} 
            <Plot tmInterval= { this.state.tmInterval}
                  valInterval= { this.state.valInterval}
                  signals = { this.props.signals}
                  axisParams= { this.state.axisParams}
                  dataParams = {this.props.dataParams}
                  signParams = { this._signParams }
                  backgroundColor = { this.props.backgroundColor}
                  onChange = { this.handlePlotChange }
                  onDrop = { this.handleAddSignal } />            
          </Col>
        </Row>
        <Row noGutters={true} style={{ borderRadius: "0px 0px 3px 3px",
                                       paddingRight : "5px",
                                       backgroundColor : "silver" }}>
          <Col style = {{ maxWidth : "50px" }} >
          </Col>
          <Col className="col" style = {{ height : "50px" }}>                
            <AxisTime tmInterval={ this.state.tmInterval}
                      axisParams={ this.state.axisParams}
                      onChange = { this.handleAxisTimeChange } /> 
          </Col>
        </Row>       
      </Container>
    )
  }
}

const buttonStyle = {
  margin : ".1em", 
}

