/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";
import {Container, Row, Col} from "react-bootstrap";
import AxisValue from "./axisValue.jsx"
import AxisTime from "./axisTime.jsx"
import Plot from "./plot.jsx"

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
   
    this.props.signParams = {};

    this.state = {tmInterval : { beginMs : Date.now(), endMs : Date.now() + 3.6e5}, 
                  valInterval : { begin : 0, end : 1000},
                  axisParams, 
                 };   

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
    
    let signals = this.props.signals;
    for (let k in signals){

      let signParams = { lineWidth : 1,
        color : '#'+Math.floor(Math.random()*16777215).toString(16) }

      signals[k].lineWidth = signals[k].lineWidth || 1;
      signals[k].color = signals[k].color || ;
    }
    
    return (
      <Container-fluid >
        <Row noGutters={true} style={{ paddingRight : "5px", backgroundColor : "grey"}}>
          <Col style={{ border: "1px solid green" }}>
            header           
          </Col>
        </Row>
        <Row noGutters={true} style={{ paddingRight : "5px", backgroundColor : "grey"}}>
          <Col className="col-1" >
            <AxisValue valInterval= { this.state.valInterval}
                       axisParams= { this.state.axisParams}
                       onChange = { this.handleAxisValueChange } />    
          </Col>
          <Col className="col-11" style={{ border: "1px solid green" }}>
            <Plot tmInterval= { this.state.tmInterval}
                  valInterval= { this.state.valInterval}
                  axisParams= { this.state.axisParams}
                  dataParams = {this.props.dataParams}
                  signals = { this.props.signals}
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

