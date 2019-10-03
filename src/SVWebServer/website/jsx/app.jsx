/* eslint-disable no-unused-vars */
import React from "react";
import ReactDOM from "react-dom";
import {Container, Row, Col} from "react-bootstrap";
import Header from "./header.jsx"; 
import GraphPanel from "./graphPanel.jsx"; 
import Footer from "./footer.jsx"; 
import TreeNav from "./treeNav.jsx"

import "../node_modules/bootstrap/dist/css/bootstrap.min.css";
 
function App() {
    
    let scheme = [
      "patch",
      { submenu : "users",
        isShow : true,
        items : [
          "Ivan",
          "Petrov",
          { submenu : "address",
            isShow : true,
            items : [
              "Московское ш., 101, кв.101",
              {
                submenu: "index",
                isShow : true,
                items: [ "101101" ],
              },
              "Ленинград"
             ],
          },
          { submenu : "phoneNumbers",
            isShow : true,
            items : [
              "812 123-1234",
              "916 123-4567"
            ]
          }
        ]
      }
    ]

  return (
    <Container style={containerStyle}>
      <Row style={headerStyle}>
        <Col>
         <Header/>
        </Col>
      </Row>
     
      <Row className="row h-100" style={articleStyle}>
        <Col className="col-auto"> 
         <TreeNav scheme={scheme} />
        </Col>
        <Col > 
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
