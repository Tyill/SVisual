/* eslint-disable no-unused-vars */

import React from "react"
import { connect } from "react-redux";
import PropTypes from "prop-types";
import Graph from "./graph.jsx";
import { signalBufferEnable } from "./redux/actions.jsx";

class GraphPanel extends React.Component {

  constructor(props){
    super(props);   
   
    this.handleAddSignal = this.handleAddSignal.bind(this); 

    this._listGraph = [[]];
  }
  
  handleAddSignal(id, name, module){

    this._listGraph[id].push(name + module);
   
    this.props.onSignalBufferEnable(name, module);
  }

  render(){
    
    let objList = [];
    for (let i = 0; i < this._listGraph.length; ++i){
     
      let signals = {};
      for (let s of this._listGraph[i]) 
        signals[s] = this.props.signals[s];
        
      objList.push(<Graph key = {i} id = {i}
                          dataParams = {this.props.dataParams}
                          signals = {signals}
                          onAddSignal = {this.handleAddSignal} ></Graph>);
    }

    return(
      <div> {objList} </div>
    )  
  }
}

const mapStateToProps = (state) => {
  return state;
}

const mapDispatchToProps = (dispatch) => {
  return {
      onSignalBufferEnable: signalBufferEnable(dispatch),    
  }
}

let GraphPanelRedux = connect(
  mapStateToProps,
  mapDispatchToProps
)(GraphPanel);

export default GraphPanelRedux
