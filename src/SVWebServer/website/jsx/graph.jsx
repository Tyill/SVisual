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
  
  }

  render(){
    
    return (
      <Container-fluid >
        <Row style={{backgroundColor : "grey"}}>
          <Col style={headerStyle}>
            <div style={{backgroundColor : "red"}}> header </div>           
          </Col>
        </Row>
        <Row style={{backgroundColor : "grey", height : "100%"}}>
          <Col style={headerStyle}>
            <div style={{backgroundColor : "green"}}> body </div>           
          </Col>
        </Row>
        <Row style={{backgroundColor : "grey"}}>
          <Col style={headerStyle}>
            <div style={{backgroundColor : "blue"}}> footer </div>           
          </Col>
        </Row>       
      </Container-fluid>
    )
  }
}

const headerStyle = {  
  border: "1px solid black",
  boxSizing: "border-box",
}