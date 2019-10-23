/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";
import "../css/treeNav.css";

export default 
class TreeNav extends React.Component {

  constructor(props){
    super(props);   
  
    this.state = { scheme : this.props.scheme};
  }

  renderSubmenu(level, obj, outObjList){
   
    if (typeof(obj) == "string"){

      outObjList.push(<section key={level + "." + obj} 
                               className="treeNav-leaf-container"
                               draggable="true"
                               style={{paddingLeft: 10 + level * 5}}>
                               {obj}
                      </section>);      
    }  
    else{

      outObjList.push(<section key={level + "." + obj.submenu}
                               className="treeNav-node-container"
                               style={{paddingLeft: 10 + level * 5}}
                               onClick= { () => {obj.isShow = !obj.isShow;
                                                 this.setState({ scheme : this.props.scheme});} }> 
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

TreeNav.propTypes = {
   // scheme_: PropTypes.object,
};