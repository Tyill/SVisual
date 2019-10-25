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

    this.handlePlotChange = this.handlePlotChange.bind(this); 
    this.handleAxisTimeChange = this.handleAxisTimeChange.bind(this);    
    this.handleAxisValueChange = this.handleAxisValueChange.bind(this);    
    
  }

  handleAxisTimeChange(tmInterval, axisParams){
    
    this.setState({tmInterval, axisParams });
  }

  handleAxisValueChange(valInterval, axisParams){
    
    this.setState({valInterval, axisParams });
  }

  handlePlotChange(tmInterval, valInterval, axisParams){
        
    this.setState({tmInterval, valInterval, axisParams});
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
           onClick = {() => this.props.onDelSignal(this.props.id, signals[k].name, signals[k].module) } 
           style = {{ marginLeft : 10,
             marginTop : 20 * legend.length,                                  
             position : "absolute",                                               
             color : this._signParams[k].color }}>
          {signals[k].name}
        </p>
      );
    }
    
    return (
      <Container-fluid >
        <Row noGutters={true} style={{ padding : "5px", backgroundColor : "grey"}}>
          <Col className="col-1"/>
          <Col className="col-10">
           <Button size="sm" className= { "icon-resize-full-alt"} style = {{ marginLeft : "5px", backgroundColor: "#747F74ff"}}/>
           <Button size="sm" className= { "icon-resize-vertical"} style = {{ marginLeft : "5px", backgroundColor: "#747F74ff"}}/>
           <Button size="sm" className= { "icon-resize-horizontal"} style = {{ marginLeft : "5px", backgroundColor: "#747F74ff"}}/>
           <Button size="sm" className= { "icon-brush"} style = {{ marginLeft : "5px", backgroundColor: "#747F74ff"}}/>
           <Button size="sm" className= { "icon-font"} style = {{ marginLeft : "5px", backgroundColor: "#747F74ff"}}/>
          </Col>
          <Col className="col-1">
           <Button size="sm" className= { "icon-cancel"} style = {{ marginLeft : "5px", backgroundColor: "#747F74ff"}}/>
          </Col>
        </Row>
        <Row noGutters={true} style={{ paddingRight : "5px", backgroundColor : "grey"}}>
          <Col className="col-1">
            <AxisValue valInterval= { this.state.valInterval}
                       axisParams= { this.state.axisParams}
                       onChange = { this.handleAxisValueChange } />    
          </Col>
          <Col className="col-11">
            {legend}
            <Plot tmInterval= { this.state.tmInterval}
                  valInterval= { this.state.valInterval}
                  signals = { this.props.signals}
                  axisParams= { this.state.axisParams}
                  dataParams = {this.props.dataParams}
                  signParams = { this._signParams }
                  onChange = { this.handlePlotChange }
                  onDrop = { (name, module) => this.props.onAddSignal(this.props.id, name, module) } />            
          </Col>
        </Row>
        <Row noGutters={true} style={{ paddingRight : "5px", backgroundColor : "grey" }}>
          <Col className="col-1" >
          </Col>
          <Col>                
            <AxisTime tmInterval={ this.state.tmInterval}
                      axisParams={ this.state.axisParams}
                      onChange = { this.handleAxisTimeChange } /> 
          </Col>
        </Row>       
      </Container-fluid>
    )
  }
}

