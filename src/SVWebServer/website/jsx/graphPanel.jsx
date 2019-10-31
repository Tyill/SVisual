/* eslint-disable no-unused-vars */

import React from "react"
import { connect } from "react-redux";
import {Container, Row, Col } from "react-bootstrap";
import PropTypes from "prop-types";
import Graph from "./graph.jsx";
import { signalBufferEnable } from "./redux/actions.jsx";

class GraphPanel extends React.Component {

  constructor(props){
    super(props);   
   
    this.handleAddSignal = this.handleAddSignal.bind(this); 
    this.handleDelSignal = this.handleDelSignal.bind(this);  
    this.handleCloseGraph = this.handleCloseGraph.bind(this);   
  }
   
  handleAddSignal(iGraph, sname){

    let listGraph = this.props.listGraph;

    if ((iGraph < listGraph.length) && sname){ 
      
      if (!listGraph[iGraph].includes(sname)){ 
        listGraph[iGraph].push(sname);
       
        this.props.onSignalBufferEnable(sname, true);
      }
    }
    else
     console.log("graphPanel::handleAddSignal error (iGraph < this.props.listGraph.length) && sname");
  }

  handleDelSignal(iGraph, sname){

    let listGraph = this.props.listGraph;

    if ((iGraph < listGraph.length) && sname){ 

      let idx = listGraph[iGraph].indexOf(sname);
  
      listGraph[iGraph].splice(idx, 1);

      let isOtherGraph = false;
      for (let i = 0; i < listGraph.length; ++i){

        if (i == iGraph) continue;

        if (listGraph[i].includes(sname)){
            isOtherGraph = true;
            break;
        }
      }
        
      if (!isOtherGraph)
        this.props.onSignalBufferEnable(sname, false);
    }
    else
      console.log("graphPanel::handleDelSignal error (iGraph < this.props.listGraph.length) && sname");
  }

  handleCloseGraph(iGraph){

    let listGraph = this.props.listGraph;

    if (iGraph < listGraph.length){ 
   
      for (const sname of listGraph[iGraph]){  

        let isOtherGraph = false;
        for (let i = 0; i < listGraph.length; ++i){

          if (i == iGraph) continue;

          if (listGraph[i].includes(sname)){
              isOtherGraph = true;
              break;
          }
        }
        
        if (!isOtherGraph)
          this.props.onSignalBufferEnable(sname, false);
      }

      this.props.onCloseGraph(iGraph);  
    }
    else
     console.log("graphPanel::handleCloseGraph error (iGraph < this.props.listGraph.length)");

  }

  render(){
    
    let objList = [],
        sz = this.props.listGraph.length;
    for (let i = 0; i < sz; ++i){
         
      let signals = {};
      for (let s of this.props.listGraph[i]) 
        signals[s] = this.props.signals[s];

      objList.push(
        <Col key = {i} xs={ sz == 1 ? 12 : 6} className = "p-2">
          <Graph iGraph = {i}
                 dataParams = {this.props.dataParams}
                 signals = {signals}  
                 onAddSignal = {this.handleAddSignal}
                 onDelSignal = {this.handleDelSignal} 
                 onCloseGraph = {this.handleCloseGraph} />
        </Col>   
      );
    }
    
    return(
      <Container > 
        <Row>
          {objList} 
        </Row>
      </Container>
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
