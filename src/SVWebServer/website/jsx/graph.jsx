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
            header           
          </Col>
        </Row>
        <Row style={{backgroundColor : "grey", height : "50%"}}>
          <Col className="col-1" style={headerStyle}>
            <AxisValue/>          
          </Col>
          <Col style={headerStyle}>
            <Plot/>          
          </Col>
        </Row>
        <Row style={{backgroundColor : "grey"}}>
          <Col className="col-1" >
          </Col>
          <Col style={{border: "1px solid green", height: "50px"}}>
            <AxisTime tmInterval={{ beginMs : Date.now(), endMs : Date.now() + 3600000 }} />           
          </Col>
        </Row>       
      </Container-fluid>
    )
  }
}

const headerStyle = {  
  border: "1px solid green",
  boxSizing: "border-box",
}
