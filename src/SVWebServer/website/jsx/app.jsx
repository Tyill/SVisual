/* eslint-disable no-unused-vars */
import React from "react";
import ReactDOM from "react-dom";
import { connect, Provider } from "react-redux";
import {Container, Row, Col, Button} from "react-bootstrap";
import TreeNav from "./treeNav.jsx";
import GraphPanelRedux from "./graphPanel.jsx";
import PropTypes from 'prop-types';
 
import { updateFromServer, 
         setDataParams, 
         setSignalsFromServer } from "./redux/actions.jsx"; 
import Store from "./redux/store.jsx"; 

import "../css/app.css";
import "../node_modules/bootstrap/dist/css/bootstrap.min.css";


class App extends React.Component {
  
  constructor(props){
    super(props);
    
    this.state = { navScheme: [], 
                   listGraph: [[]] };

    document.body.style.overflow = "hidden";
        
    this.handleAddGraph = this.handleAddGraph.bind(this); 
    this.handleCloseGraph = this.handleCloseGraph.bind(this);   
  }

  handleAddGraph(){

    this.setState((oldState, props) => (
       { listGraph : [...oldState.listGraph, []] } 
       ));
  }

  handleCloseGraph(iGraph){
   
    if (iGraph < this.state.listGraph.length){ 

      this.setState((oldState, props) => { 
        
        let listGraph = [...oldState.listGraph];

        listGraph.splice(iGraph, 1);  

        return { listGraph };
      });     
    }
    else
     console.log("app::handleCloseGraph error (iGraph < this.state.listGraph.length)");

  }

  componentDidMount() {
   
     ReactDOM.findDOMNode(this).addEventListener('wheel', (event) => {
       event.preventDefault();
     }, false);
    
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
                 iActive : true,
                 items : []};
  
          navScheme.push(it);
        }
  
        it.items.push(s.name);
      }
    
      this.setState({navScheme});

    })().catch(() => console.log('api/allSignals error'));

  }

  updateSignalData(dataParams){

    let count = 0;

    let update = async function () {
      
      const signs = this.props.signals,       
            snames = Object.values(signs).filter(it => it.isBuffEna)
                                         .map(it => it.name + it.module);
     
      if (snames.length > 0){

        let req = "api/lastSignalData?";
        for (let i = 0; i < snames.length; ++i){
          req += "sname" + i + "=" + snames[i];
        
          if (i < snames.length - 1)
            req += "&";
        }
       
        try{
          let response = await fetch(req),          
              buffVals = await response.json();         
        
          if (Object.keys(buffVals).length > 0) 
            this.props.onUpdateFromServer(buffVals);
  
        }catch(err){
          console.log('api/lastSignalData error');
        }
      } 
      
      if ((count % 10) == 0){
        try{
          
          let response = await fetch('api/allModules'),          
              modState = await response.json();  
          
          let navScheme = this.state.navScheme;

          for (let k in modState){    
            
            let module = k;
            
            let it = navScheme.find((it) => {
              return module == it.submenu;
            });
  
            if (it)  
              it.isActive = modState[k].isActive; 
          }

          this.setState(navScheme);

        }catch(err){
          console.log('api/allModules error');              
        }
      }
      ++count;

      setTimeout(update, dataParams.cycleTimeMs * dataParams.packetSize);
    };

    update = update.bind(this);

    update();
  }    
    
  render(){

    return (
      <Container className="col-auto app-container"
                 style={{overflow: "auto", height: document.documentElement.clientHeight}}>
        <Row className="row h-100"
             style = {{  border: "1px solid #dbdbdb", borderRadius: "5px"}}>
          <Col className="col-auto"> 
            <Button size="md" className = {"icon-cog"} style = {buttonStyle}
                    onClick = {this.handleAddGraph}/>
            <Button size="md" className = {"icon-doc"} style = {buttonStyle}
                    onClick = {this.handleAddGraph} />
            <TreeNav scheme={this.state.navScheme} />
          </Col>
          <Col className="col-auto"> 
            <GraphPanelRedux listGraph = { this.state.listGraph } 
                             onCloseGraph = { this.handleCloseGraph } />
          </Col>
        </Row>
      </Container>
    )
  } 
}


App.propTypes = {
  state : PropTypes.shape({
    //{ submenu : s.module,
    //  isShow : true,
    //  iActive : true,
    //  items : [string]};
    navScheme : PropTypes.arrayOf(
      PropTypes.shape({
        submenu : PropTypes.string,
        isShow : PropTypes.bool,
        iActive : PropTypes.bool,
        items : PropTypes.arrayOf(PropTypes.string),
      })
    ),
    // [[string]]
    listGraph : PropTypes.arrayOf(PropTypes.arrayOf(PropTypes.string)),
  }),
  handleAddGraph: PropTypes.func,
  handleCloseGraph: PropTypes.func,
}


const buttonStyle = {   
  fontSize : "16pt", 
  margin : "5px", 
  backgroundColor: "#747F74ff",
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