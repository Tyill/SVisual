/* eslint-disable no-unused-vars */
import React from "react";
import ReactDOM from "react-dom";
import { connect, Provider } from "react-redux";
import {Container, Row, Col, Button} from "react-bootstrap";
import TreeNav from "./treeNav.jsx";
import GraphPanelRedux from "./graphPanel.jsx";
 
import { updateFromServer, 
         setDataParams, 
         setSignalsFromServer } from "./redux/actions.jsx"; 
import Store from "./redux/store.jsx"; 

import "../css/app.css";
import "../node_modules/bootstrap/dist/css/bootstrap.min.css";

class App extends React.Component {
    
  constructor(props){
    super(props);
    
    this.state = { navScheme: [] };

    document.body.style.overflow = "hidden";
        
  }

  componentDidMount() {
   
    (async () => {
      let response = await fetch('api/allSignals');
      let signs = await response.json();     

      for (let k in signs){
        signs[k].isBuffEna = false,
        signs[k].buffVals = [
          //          {
          //            time : 0,
          //            vals : []
          //          }
                     ]    
      }
      
      this.props.onSetSignalsFromServer(signs);

      response = await fetch('api/dataParams');
      let dataParams = await response.json();     
            
      this.props.onSetDataParams(dataParams);
     
      /////////////////////////////////////
      
      this.updateSignalData(dataParams);

      /////////////////////////////////////

      let navScheme = [];
      for (let k in signs){
    
        let s = signs[k];

        let it = navScheme.find((it) => {
          return s.module == it.submenu;
        });
  
        if (!it){ 
  
          it = { submenu : s.module,
                 isShow : true,
                 items : []};
  
          navScheme.push(it);
        }
  
        it.items.push(s.name);
      }
    
      this.setState({navScheme});

    })().catch(() => console.log('api/allSignals error'));

  }

  updateSignalData(dataParams){

    let update = async function () {
      
      let signs = this.props.signals;

      let buffVals = {};
      
      for (let k in signs){
        
        if (signs[k].isBuffEna){
          
          let name = signs[k].name,
              module = signs[k].module; 
          
          let response = await fetch(`api/lastSignalData?name=${name}&module=${module}`),          
              data = await response.json();         
                    
          buffVals[k] = data;
        }
      }
      
      if (Object.keys(buffVals).length > 0) 
        this.props.onUpdateFromServer(buffVals);

      setTimeout(update, dataParams.cycleTimeMs * dataParams.packetSize);
    };

    update = update.bind(this);

    update();
  }    
    
  render(){

    return (
      <Container className="app-article-container"
                 style={{ height: document.documentElement.clientHeight,}}>
        <Row className="row h-100"
             style = {{ border: "1px solid #dbdbdb", borderRadius: "5px"}}>
          <Col className="col-auto"> 
            <Button size="md" className= { "icon-cog"} style = {{ fontSize : "16pt", margin : "5px", backgroundColor: "#747F74ff"}}/>
            <Button size="md" className= { "icon-doc"} style = {{ fontSize : "16pt", margin : "5px", backgroundColor: "#747F74ff"}}/>
            <TreeNav scheme={this.state.navScheme} />
          </Col>
          <Col className="col-auto"> 
            <GraphPanelRedux/>
          </Col>
        </Row>
      </Container>
    )
  } 
}

const containerStyle = {   
 
}

//////////////////////////////////////////////////

const mapStateToProps = (state) => {
  return state;
}

const mapDispatchToProps = (dispatch) => {
  return {
      onSetSignalsFromServer: setSignalsFromServer(dispatch),    

      onUpdateFromServer: updateFromServer(dispatch),
        
      onSetDataParams: setDataParams(dispatch),
  }
}

let AppRedux = connect(mapStateToProps, mapDispatchToProps)(App);

ReactDOM.render(
  <Provider store={Store}>
     <AppRedux /> ,
  </Provider>,
  document.getElementById('root')
);