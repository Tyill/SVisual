/* eslint-disable no-unused-vars */
// @flow
import React from "react"
import PropTypes from "prop-types";
import {Container, Row, Col, Button } from "react-bootstrap";
import AxisValue from "./axisValue.jsx"
import AxisTime from "./axisTime.jsx"
import Plot from "./plot.jsx"
import "../css/fontello.css";


/*::
import type { snameType, signalType, configType, dataParamsType, signalDataType } from "./redux/store.jsx";
import type { tmIntervalType } from "./axisTime.jsx"; 
import type { valIntervalType } from "./axisValue.jsx"; 

export
type axisParamsType = {
  valOffsPos : number,
  valDashStep : number, 
  tmOffsPos : number,
  tmDashStep : number,
  minValDashStep : number,
  maxValDashStep : number,
}

export
type signColorParamsType = {
  lineWidth : number,
  transparent : number,
  color : number,
}

type Props = {
  iGraph : number,   
  zIndex : {val : number},              
  dataParams : dataParamsType,
  signals : {sname : signalType},  
  backgroundColor : string,
  onAddSignal : (iGraph : number, sname : snameType) => void,
  onDelSignal : (iGraph : number, sname : snameType) => void,
  onCloseGraph : (iGraph : number) => void
}
  
type State = {
  tmInterval : tmIntervalType,
  valInterval : valIntervalType,
  axisParams : axisParamsType,
  offsPosContainer : {left : number, top : number},
  csizeContainer : {width : number, height : number},
};

*/

export default
class Graph extends React.Component/*::<Props, State>*/ {

  /*::
  _graphRef : any;
  _signColorParams : signColorParamsType;
  _isPlay : boolean;
  _isAutoResize : boolean;
  _isResizeComponent : boolean;
  _zIndex : number;
  
  handlePlotChange: (tmIntervalType, valIntervalType, axisParamsType) => void;
  handleAxisTimeChange: (tmIntervalType, axisParamsType) => void;
  handleAxisValueChange: (valIntervalType, axisParamsType) => void;
  handleAddSignal: (snameType) => void;
  handleDelSignal: (snameType) => void;

  handleResizeFull : () => void;   
  handleResizeByValue : () => void;    
  handleResizeByTime : () => void;   
  handleChangeColor : () => void;  
  handleAutoResize : () => void;  
  handlePlay : () => void; 
  handleClose : () => void; 

  handleChangePosContainer : (event : any) => void; 
  handleResizeContainer : (event : any) => void; 
  */

  constructor(props /*:: : Props */){
    super(props);   
     
    let axisParams = { valOffsPos : 0,
                       valDashStep : 100,  
                       tmOffsPos : 0,
                       tmDashStep : 100,
                       minValDashStep : 50,
                       maxValDashStep : 100}
   
    const offsPosContainer = {
      left : 0,
      top : 0, 
    }

    const csizeContainer = {
      width : 0,
      height : Math.min(250, document.documentElement ? document.documentElement.clientHeight * 0.65 : 250)
    }

    this.state = {tmInterval : { beginMs : Date.now(), endMs : Date.now() + 3.6e4}, 
                  valInterval : { begin : 0, end : 1000},
                  axisParams, 
                  offsPosContainer, 
                  csizeContainer,                
                 };   

    this._graphRef = null;
    this._signColorParams = {};
    this._isPlay = true;
    this._isAutoResize = true;
    this._isResizeComponent = false;

    this.props.zIndex.val += 1; 
    this._zIndex = this.props.zIndex.val + 1;
   
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

    this.handleChangePosContainer = this.handleChangePosContainer.bind(this);
    this.handleResizeContainer = this.handleResizeContainer.bind(this);
  }

  componentDidMount() {
   
    const graph = this._graphRef;
          
    this.state.csizeContainer.width = graph.clientWidth;
  }


  componentDidUpdate() {
   
    const graph = this._graphRef;
          
    this.state.csizeContainer.width = graph.clientWidth;    
  }

  handleAxisTimeChange(tmInterval /*:: : tmIntervalType*/, axisParams /*:: : axisParamsType*/){
    
    this._isPlay = false;

    this.setState({tmInterval, axisParams });
  }

  handleAxisValueChange(valInterval/*:: : valIntervalType*/, axisParams /*:: : axisParamsType*/){
    
    this._isPlay = false;

    this.setState({valInterval, axisParams });
  }

  handlePlotChange(tmInterval /*:: : tmIntervalType*/,
                   valInterval/*:: : valIntervalType*/, 
                   axisParams/*:: : axisParamsType*/){
        
    this._isPlay = false;

    this.props.zIndex.val += 1;      
    this._zIndex = this.props.zIndex.val + 1;    

    this.setState({tmInterval, valInterval, axisParams});
  }

  handleAddSignal(sname /*:: : string*/){

    this.props.onAddSignal(this.props.iGraph, sname);
  }

  handleDelSignal(sname /*:: : string*/){
    
    delete this._signColorParams[sname]
   
    this.props.onDelSignal(this.props.iGraph, sname);
  }
 
  handleChangePosContainer(event /*:: : any*/){
   
    // left mouse button
    if (event.nativeEvent.which === 1){
  
      this.props.zIndex.val += 1;
      this._zIndex = this.props.zIndex.val + 1;
      
      let distX = event.nativeEvent.movementX,
          distY = event.nativeEvent.movementY;

      this.setState((cState /*:: : State*/, props /*:: : Props*/) => {

        let offsPosContainer = {
          left : cState.offsPosContainer.left + distX,
          top : cState.offsPosContainer.top + distY,
        }
        return {offsPosContainer};
      })
    }
  }

  handleResizeContainer(event/*:: : any*/){

    // left mouse button
    if (event.nativeEvent.which === 1){
      
      this._isResizeComponent = true;

      this.props.zIndex.val += 1;
      this._zIndex = this.props.zIndex.val + 1;

      let distX = event.nativeEvent.movementX,
          distY = event.nativeEvent.movementY;

      this.setState((cState/*:: : State*/, props/*:: : Props*/) => {

        let csizeContainer = {
          width : cState.csizeContainer.width + distX,
          height : cState.csizeContainer.height + distY,
        }
        return {csizeContainer};
      })
    }
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

    for (let k in this._signColorParams){
    
      let prms = this._signColorParams[k];
      prms.color = '#'+Math.floor(Math.random()*16777215).toString(16);
    }
  }

  handleAutoResize(){

    this._isAutoResize = !this._isAutoResize;
  }
  
  handlePlay(){

    this._isPlay = !this._isPlay; 
  }

  handleClose(){

    this.props.onCloseGraph(this.props.iGraph);
  }

  render(){
    
    let signals = this.props.signals,
        legend = [];
    for (let k in signals){

      if (!this._signColorParams[k]){ 
        this._signColorParams[k] = { lineWidth : 2,
                                transparent : 0.5,
                                color : '#'+Math.floor(Math.random()*16777215).toString(16) };
      }
      
      legend.push(
        <p key = {legend.length} 
           onClick = { () => this.handleDelSignal(k) } 
           style = {{ marginLeft : 10,
                      marginTop : 20 * legend.length,                                  
                      position : "absolute", 
                      cursor: "default",                                              
                      color : this._signColorParams[k].color }}>
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

    let {left, top} = this.state.offsPosContainer; 

    let style /*:: : any */ = {
      position : this._isResizeComponent ? "absolute" : "relative",
      zIndex : this._zIndex,
      left, top,
    }

    if (this._isResizeComponent)
      style.width = this.state.csizeContainer.width;
    
    const buttonStyle = {
      margin : ".1em", 
    }

    return (
      <Container style = {style} ref={ el => this._graphRef = el }>
        <Row noGutters={true} 
             style={{ borderRadius: "3px 3px 0px 0px", padding : "5px", backgroundColor : "silver"}}
             onMouseMove = {this.handleChangePosContainer} >
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
           <button type="button" className="close" aria-label="Close" onClick = { this.handleClose }>
              <span aria-hidden="true">&times;</span>
           </button>
           </Col>          
        </Row>
        <Row noGutters={true} style={{ paddingRight : "5px", backgroundColor : "silver"}}>
          <Col style = {{ maxWidth : "50px", height: this.state.csizeContainer.height }}>
            <AxisValue valInterval= { this.state.valInterval}
                       axisParams= { this.state.axisParams}
                       onChange = { this.handleAxisValueChange } />    
          </Col>
          <Col className="col" style={{height: this.state.csizeContainer.height}}>
            {legend} 
            <Plot tmInterval= { this.state.tmInterval}
                  valInterval= { this.state.valInterval}
                  signals = { this.props.signals}
                  axisParams= { this.state.axisParams}
                  dataParams = {this.props.dataParams}
                  signColorParams = { this._signColorParams }
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
            <a style = {{ position : "absolute", right : -20, top : 10, cursor: "default", 
                          width : "50px", height : "50px", paddingLeft : 25, paddingTop : 15 }} 
                onMouseMove = {this.handleResizeContainer}>&#8250;</a>
          </Col>
        </Row>       
      </Container>
    )
  }
}


