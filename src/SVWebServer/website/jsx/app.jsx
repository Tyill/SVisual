/* eslint-disable no-unused-vars */
// @flow
import React from "react";
import ReactDOM from "react-dom";
import { connect, Provider } from "react-redux";
import {Container, Row, Col, Button, Modal} from "react-bootstrap";
import TreeNav from "./treeNav.jsx";
import GraphPanelRedux from "./graphPanel.jsx";
 
import { updateFromServer, 
         setDataParams, 
         setSignalsFromServer,
         signalBufferEnable,
         changeConfig } from "./redux/actions.jsx"; 
import Store from "./redux/store.jsx"; 

import "../css/app.css";
import "bootstrap/dist/css/bootstrap.min.css";

/*::   
import type {snameType, signalType, configType, dataParamsType, signalDataType } from "./redux/store.jsx"; 
import type {changeConfigType, setSignalsFromServerType,
             setDataParamsType, updateFromServerType, signalBufferEnableType} from "./redux/actions.jsx";
import type {navSchemeType} from "./treeNav.jsx";

type Props = {
  signals : { obj: signalType },
  config : configType,
  dataParams : dataParamsType,
  onSignalBufferEnable: signalBufferEnableType,
  onChangeConfig: changeConfigType,
  onSetSignalsFromServer: setSignalsFromServerType,
  onSetDataParams: setDataParamsType,
  onUpdateFromServer: updateFromServerType,
};

type State = {
  navScheme: Array<string | navSchemeType>,
  listGraph: Array<Array<snameType>>,
  isShowConfig : boolean,
  isDarkThemeConfig : boolean,
}
*/

class App extends React.Component/*::<Props, State>*/{
  
  /*::
  handleAddGraph: () => void;
  handleCloseGraph: (iGraph : number) => void;
  handleShowConfig: () => void;
  handleChangeConfig: (event : any) => void;
  handleAddSignalOnGraph: (sname : string) => void;
  */

  constructor(props){
    super(props);
    
    this.state = { navScheme: [], 
                   listGraph: [[]],
                   isShowConfig : false,
                   isDarkThemeConfig : false };

    if (document.body)                       
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

  handleCloseGraph(iGraph /*:: : number*/){
   
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

  handleAddSignalOnGraph(sname /*:: : string*/){

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
    
    let config /*:: : configType */=  {
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
   
    let nd = ReactDOM.findDOMNode(this);
    if (nd){
      nd.addEventListener('wheel', (event /*:: : any*/) => {
        event.preventDefault();
      }, false);
    }
      
    fetch('api/allSignals')
    .then(response => response.json())    
    .then(signs =>{ 
        
      for (let k in signs){
        signs[k].isBuffEna = false,
        signs[k].buffVals = []    
      }
      
      this.props.onSetSignalsFromServer(signs);
    
      setNavScheme(signs);
    })
    .catch(() => console.log('api/allSignals error'))  


    fetch('api/dataParams')
    .then(response => response.json())    
    .then(dataParams => {
     
      this.props.onSetDataParams(dataParams);
      
      this.updateSignalData(dataParams);
    })    
    .catch(() => console.log('api/dataParams error'));    


    let setNavScheme = (signs /*:: : {obj : signalType} */) => {
      
      let navScheme /*:: : Array<navSchemeType | string> */ = [];
      for (let k in signs){
    
        let s = signs[k];

        let it /*:: : any */ = navScheme.find((it) => {

          return (typeof it === 'object') && (typeof it.submenu === 'string') ?
                    s.module == it.submenu : false;
        });

        if (!it){ 

          it = { submenu : s.module,
                            isShow : true,
                            isActive : true,
                            items : []};

          navScheme.push(it);
        }
        
        it.items.push(s.name);
      }
          
      this.setState({navScheme});
    } 

  }

  updateSignalData(dataParams /*:: : dataParamsType */){

    let count = 0;

    let updateFunc = () => {
      
      let tmStart = Date.now();
      
      const signs = this.props.signals,       
            snames = Object.values(signs).filter((it /*:: : any */)  => it.isBuffEna)
                                         .map((it /*:: : any */) => it.name + it.module);

      if (snames.length > 0){

        let req = "api/lastSignalData?";
        for (let i = 0; i < snames.length; ++i){
          req += "sname" + i + "=" + snames[i];
        
          if (i < snames.length - 1)
            req += "&";
        }
               
        fetch(req)
        .then(response => response.json())
        .then(buffVals => {         
          
          if (Object.keys(buffVals).length > 0) 
            this.props.onUpdateFromServer(buffVals);
        })
        .catch(() => console.log('api/lastSignalData error'));
      } 
      
      if ((count % 10) == 0){
                 
        fetch('api/allModules')
        .then(response => response.json())
        .then(modState => {  
          
          let navScheme = this.state.navScheme;

          for (let k in modState){    
            
            let module = k;
            
            let it /*:: : any */ = navScheme.find((it) => {
              return ((typeof it === 'object') && (typeof it.submenu === 'string')) ?
                (module == it.submenu) : false;
            });
  
            if (it)  
              it.isActive = modState[k].isActive; 
          }

          this.setState({navScheme});

        })
        .catch(() => {
          
          console.log('api/allModules error'); 
          
          let navScheme = this.state.navScheme;
  
          for (let it /*:: : any */ of navScheme)    
            it.isActive = false; 
  
          this.setState({navScheme});
        });
      }

      ++count;

      let tout = Date.now() - tmStart;
       
      tout = Math.max(0, dataParams.cycleTimeMs * dataParams.packetSize * 0.9 - tout);
        
      setTimeout(updateFunc, tout);
    }
        
    updateFunc();
  }
    
  render(){

    let clientHeight = document.documentElement ? document.documentElement.clientHeight : 300;

    const Checkbox = props => (
      <input type="checkbox" {...props} />
    )

    const buttonStyle = {   
      fontSize : "16pt", 
      margin : "5px",
    }

    return (
      <div>
      <Container className="col app-container"
                 style={{overflow: "auto", height: clientHeight}}>
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

const root = document.getElementById('root')

if (root){
  ReactDOM.render(
    <Provider store={Store}>
       <AppRedux/>,
    </Provider>,
    root
  );
}