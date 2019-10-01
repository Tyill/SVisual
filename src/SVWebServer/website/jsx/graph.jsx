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
      <Container>
        <Row noGutters={true}>
          <Col style={headerStyle}>
           Graph header
          </Col>
        </Row>
        <Row noGutters={true}>
          <Col>      
           <AxisValue/>     
          </Col>
          <Col>
           <Plot/>
          </Col>
        </Row>
        <Row>
          <Col>
           <AxisTime/>
          </Col>
        </Row>
      </Container>
    )
  }
}

const headerStyle = {  
  border: "1px solid red",
  boxSizing: "border-box",
}