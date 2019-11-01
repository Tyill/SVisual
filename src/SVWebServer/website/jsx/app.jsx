/* eslint-disable no-unused-vars */
import React from "react";
import ReactDOM from "react-dom";
import { connect, Provider } from "react-redux";
import {Container, Row, Col, Button, Modal} from "react-bootstrap";
import TreeNav from "./treeNav.jsx";
import GraphPanelRedux from "./graphPanel.jsx";
import PropTypes from 'prop-types';
 
import { updateFromServer, 
         setDataParams, 
         setSignalsFromServer,
         signalBufferEnable,
         changeConfig } from "./redux/actions.jsx"; 
import Store from "./redux/store.jsx"; 

import "../css/app.css";
import "bootstrap/dist/css/bootstrap.min.css";


class App extends React.Component {
  
  constructor(props){
    super(props);
    
    this.state = { navScheme: [], 
                   listGraph: [[]],
                   isShowConfig : false,
                   isDarkThemeConfig : false };

    document.body.style.overflow = "hidden";
        
    this.handleAddGraph = this.handleAddGraph.bind(this); 
    this.handleCloseGraph = this.handleCloseGraph.bind(this);  
    this.handleShowConfig = this.handleShowConfig.bind(this); 
    this.handleChangeConfig = this.handleChangeConfig.bind(this); 
  
    this.handleAddSignalOnGraph = this.handleAddSignalOnGraph.bind(this); 
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

  handleAddSignalOnGraph(sname){

    if (this.state.listGraph.length && !this.state.listGraph[0].includes(sname)){ 

      this.props.onSignalBufferEnable(sname, true);

      this.setState((oldState, props) => { 
        
        let listGraph = [...oldState.listGraph];
       
        listGraph[0].push(sname);
       
        return { listGraph };
      });  
    }  
  }

  handleShowConfig(){

    this.setState((oldState, props)=>{

      let isShowConfig = !oldState.isShowConfig;

      return {isShowConfig}
    });
  }

  handleChangeConfig(e){
    
    let config = {
      backgroundColor : "white",
    };

    if (!this.state.isDarkThemeConfig) 
      config.backgroundColor = "black";

    this.props.onChangeConfig(config)

    this.setState((oldState, props)=>{

      let isDarkThemeConfig = !oldState.isDarkThemeConfig;

      return {isDarkThemeConfig}
    });
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
      <div>
      <Container className="col app-container"
                 style={{overflow: "auto", height: document.documentElement.clientHeight}}>
        <Row noGutters={true} className="m-1 p-2"
             style = {{  border: "1px solid #dbdbdb", borderRadius: "5px"}}>
          <Col className="col-auto"> 
            <Button size="md" className = {"icon-cog"} style = {buttonStyle}
                    onClick = {this.handleShowConfig}/>
            <Button size="md" className = {"icon-doc"} style = {buttonStyle}
                    onClick = {this.handleAddGraph} />
            <TreeNav scheme={this.state.navScheme}
                     onDoubleClick = { this.handleAddSignalOnGraph } />
          </Col>
          <Col className="col"> 
            <GraphPanelRedux listGraph = { this.state.listGraph } 
                             onCloseGraph = { this.handleCloseGraph } />
          </Col>
        </Row>
      </Container>  

      <Modal show = {this.state.isShowConfig} onHide={this.handleShowConfig} >
        <Modal.Header closeButton>
          <Modal.Title>Settings</Modal.Title>
        </Modal.Header>      
        <Modal.Body>        
         <Checkbox checked={this.state.isDarkThemeConfig}
                   onChange={this.handleChangeConfig}/> dark theme        
        </Modal.Body>            
      </Modal>
      </div>
    )
  } 
}

const Checkbox = props => (
   <input type="checkbox" {...props} />
)


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
      onSignalBufferEnable: signalBufferEnable(dispatch),
      onChangeConfig: changeConfig(dispatch),  
  }
}

let AppRedux = connect(mapStateToProps, mapDispatchToProps)(App);

ReactDOM.render(
  <Provider store={Store}>
     <AppRedux /> ,
  </Provider>,
  document.getElementById('root')
);