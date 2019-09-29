/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from 'prop-types';
import "../css/treeWidget.css";

export {TreeWidget}

class TreeWidget extends React.Component {

  constructor(props){
    super(props);   
  
  }

  renderSubmenu(level, obj, outObjList){
   
    if (typeof(obj) == "string"){

      outObjList.push(<section key={level + "." + obj} 
                               className="treeWidget-leaf-container"
                               style={{paddingLeft: 10 + level * 5}}>
                               {obj}
                      </section>);      
    }  
    else{

      outObjList.push(<section key={level + "." + obj.submenu}
                               className="treeWidget-node-container"
                               style={{paddingLeft: 10 + level * 5}}>
                               {obj.submenu}
                      </section>);

      if (obj.isShow){
        for(let i of obj.items)
          outObjList.push(this.renderSubmenu(level + 1, i, outObjList)); 
      }
    }  
  }

  render(){

    let outObjList = [];

    for(let obj of this.props.scheme)
      this.renderSubmenu(0, obj, outObjList);

    return <div style={{ overflow: "auto"}}> {outObjList}  </div>
  }
}

TreeWidget.propTypes = {
   // scheme_: PropTypes.object,
};