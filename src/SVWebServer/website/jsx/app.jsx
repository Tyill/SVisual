/* eslint-disable no-unused-vars */
import React from "react";
import ReactDOM from "react-dom";
import "../node_modules/bootstrap/dist/css/bootstrap.min.css";

import {TreeWidget} from "./tree.jsx"
 
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

    return <TreeWidget scheme = {scheme}/>
}

ReactDOM.render(
  <App />, 
  document.getElementById('root')
);
