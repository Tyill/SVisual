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
     
    this.state = {tmInterval : { beginMs : Date.now(), endMs : Date.now() + 3.6e6}, 
                  valInterval : { begin : 0, end : 1000},
                 };

    this.tmAxisLines = [];
    this.valAxisLines = [];

    this.handlePlotChange = this.handlePlotChange.bind(this); 
    this.handleAxisTimeChange = this.handleAxisTimeChange.bind(this);    
    this.handleAxisValueChange = this.handleAxisValueChange.bind(this);             
  }

  handleAxisTimeChange(tmIntl, tmAxisLines){
    
    this.tmAxisLines = tmAxisLines;

    this.setState({tmInterval : tmIntl })
  }

  handleAxisValueChange(valIntl, valAxisLines){
    
    this.valAxisLines = valAxisLines;

    this.setState({valInterval : valIntl })
  }

  handlePlotChange(tmIntl, valIntl){
        
    this.setState({tmInterval : tmIntl, 
                   valInterval : valIntl })
  }

  render(){
    
    return (
      <Container-fluid >
        <Row noGutters={true} style={{ paddingRight : "5px", backgroundColor : "grey"}}>
          <Col style={{ border: "1px solid green" }}>
            header           
          </Col>
        </Row>
        <Row noGutters={true} style={{ paddingRight : "5px", backgroundColor : "grey"}}>
          <Col className="col-1" >
            <AxisValue valInterval={this.state.valInterval}
                       onChange = { this.handleAxisValueChange } />          
          </Col>
          <Col className="col-11" style={{ border: "1px solid green" }}>
            <Plot tmInterval={ this.state.tmInterval}
                  tmAxisLines={ this.tmAxisLines}
                  valInterval={ this.state.valInterval}
                  valAxisLines={ this.valAxisLines}
                  onChange = { this.handlePlotChange } />            
          </Col>
        </Row>
        <Row noGutters={true} style={{ paddingRight : "5px", backgroundColor : "grey" }}>
          <Col className="col-1" >
          </Col>
          <Col>
            <AxisTime tmInterval={ this.state.tmInterval}
                      onChange = { this.handleAxisTimeChange } />           
          </Col>
        </Row>       
      </Container-fluid>
    )
  }
}

