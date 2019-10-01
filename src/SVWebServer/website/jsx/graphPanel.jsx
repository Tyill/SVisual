/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";
import Graph from "./graph.jsx";

export default 
class GraphPanel extends React.Component {

  constructor(props){
    super(props);   
  
  }

  render(){
    
    return(
       <div> 
         <Graph></Graph>
       </div>
    )  
}
}
