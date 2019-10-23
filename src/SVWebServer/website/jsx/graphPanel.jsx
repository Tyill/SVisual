/* eslint-disable no-unused-vars */

import React from "react"
import { connect } from "react-redux";
import PropTypes from "prop-types";
import Graph from "./graph.jsx";

class GraphPanel extends React.Component {

  constructor(props){
    super(props);   
  
    this._dataParams = {};  
   
  }

  componentDidMount() {

    (async () => {
      
      let response = await fetch('api/dataParams');
      this._dataParams = await response.json();     
      
      let cycleMs = this._dataParams.packetSize * this._dataParams.cycleTimeMs;
     
      let update = async function(){

        let sname = "s1"; 
        let response = await fetch(`api/signalData?name=${sname}`);
        let jnData = await response.json();     
             
        this.render();

        setTimeout(update, cycleMs);
      }

      update = update.bind(this);

      update();

    })().catch(() => console.log('api/dataParams error'));

  }

  render(){
    
    return(
        <Graph dataParams = {this._dataParams}></Graph>     
    )  
  }
}

const mapStateToProps = (state) => {
  return state.signals;
}

const GraphPanelRedux = connect(
  mapStateToProps
)(GraphPanel);

export default GraphPanelRedux
