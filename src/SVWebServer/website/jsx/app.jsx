/* eslint-disable no-unused-vars */
import React from "react";
import ReactDOM from "react-dom";
import {Container, Row, Col} from "react-bootstrap";
import Header from "./header.jsx"; 
import GraphPanel from "./graphPanel.jsx"; 
import Footer from "./footer.jsx"; 
import TreeNav from "./treeNav.jsx";
import "../node_modules/bootstrap/dist/css/bootstrap.min.css";
 
// const { Provider } = require('react-redux')
// const { createStore } = require('react-redux')
//const reducers = require('./modules')

class App extends React.Component {
    
  constructor(props){
    super(props);
    
    this.state = { navScheme: [] };
  }

  componentDidMount() {
      
    (async () => {
      let response = await fetch('api/allSignals');
      let signs = await response.json();     
      
      let scheme = [];
      for (let s of signs){
    
        let it = scheme.find((it) => {
          return s.module == it.submenu;
        });
  
        if (!it){ 
  
          it = { submenu : s.module,
                 isShow : true,
                 items : []};
  
          scheme.push(it);
        }
  
        it.items.push(s.name);
      }
    
      this.setState({navScheme : scheme});

    })().catch(() => console.log('api/allSignals error'));

  }
    
  render(){

    return (
      <Container style={containerStyle}>
        <Row style={headerStyle}>
          <Col>
            <Header/>
          </Col>
        </Row>
        
        <Row className="row h-100" style={articleStyle}>
          <Col className="col-auto"> 
            <TreeNav scheme={this.state.navScheme} />
          </Col>
          <Col className="col-auto"> 
            <GraphPanel/>
          </Col>
        </Row>
  
        <Row style={footerStyle}>
          <Col> 
            <Footer/>
          </Col>
        </Row>
      </Container>
    )
  } 
}

const containerStyle = {   
  height: document.documentElement.clientHeight - 150,
}

const headerStyle = {  
  border: "1px solid black",
  boxSizing: "border-box",
  backgroundColor : "yellow",
  height : "100px",
}

const articleStyle = {  
  border: "1px solid black",
  boxSizing: "border-box",
  backgroundColor : "brown",
}

const footerStyle = {  
  border: "1px solid black",
  boxSizing: "border-box",
  backgroundColor : "#898b8f",
  height : "50px",
}

ReactDOM.render(
  <App />, 
  document.getElementById('root')
);
