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
        <Row>
          <Col>
           Graph header
          </Col>
        </Row>
        <Row>
          <Col className="col-auto">      
           <AxisValue/>     
          </Col>
          <Col>
           <Plot/>
          </Col>
        </Row>
        <Row>
          <Col className="col-auto">
           <AxisTime/>
          </Col>
        </Row>
      </Container>
    )
  }
}
