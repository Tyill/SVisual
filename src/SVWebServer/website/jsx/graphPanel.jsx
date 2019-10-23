/* eslint-disable no-unused-vars */

import React from "react"
import { connect } from "react-redux";
import PropTypes from "prop-types";
import Graph from "./graph.jsx";

class GraphPanel extends React.Component {

  constructor(props){
    super(props);   
      
  }

  componentDidMount() {

  }

  componentDidUpdate() {

        
  }

  render(){
    
    return(
        <Graph></Graph>     
    )  
  }
}

const mapStateToProps = (state) => {
  return state;
}

const GraphPanelRedux = connect(
  mapStateToProps
)(GraphPanel);

export default GraphPanelRedux
