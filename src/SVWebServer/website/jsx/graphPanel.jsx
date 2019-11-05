/* eslint-disable no-unused-vars */
// @flow
import React from "react"
import { connect } from "react-redux";
import {Container, Row, Col } from "react-bootstrap";
import PropTypes from "prop-types";
import Graph from "./graph.jsx";
import { signalBufferEnable } from "./redux/actions.jsx";

/*:: import type {signalType, configType, dataParamsType, signalDataType } from "./redux/store.jsx"; */
/*:: import type {signalBufferEnableType} from "./redux/actions.jsx"; */

/*::   
type Props = {
  signals : { obj: signalType },
  config : configType,
  dataParams : dataParamsType,  
  listGraph : Array<Array<string>>,
  onCloseGraph : (iGraph : number) => void,
  onSignalBufferEnable: signalBufferEnableType,
};

*/

class GraphPanel extends React.Component/*::<Props>*/ {

  /*::
  handleAddSignal: (iGraph : number, sname : string) => void;
  handleDelSignal: (iGraph : number, sname : string) => void;
  handleCloseGraph: (iGraph : number) => void;
  */

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

      const xs = 12,
            md = (sz == 1) ? 12 : 6,
            xl = (sz == 1) ? 12 : (sz == 2) ? 6 : 4;

      objList.push( 
        <Col key = {i} className = {`pb-2 col-${xs} col-md-${md} col-xl-${xl} `}>
          <Graph iGraph = {i}                 
                 dataParams = {this.props.dataParams}
                 signals = {signals}  
                 backgroundColor = {this.props.config.backgroundColor}
                 onAddSignal = {this.handleAddSignal}
                 onDelSignal = {this.handleDelSignal} 
                 onCloseGraph = {this.handleCloseGraph} />
        </Col>   
      );
    }
    
    return(
      <Container className="col"> 
        <Row noGutters={true} >
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
