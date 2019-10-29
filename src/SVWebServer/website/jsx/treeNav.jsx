/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";
import "../css/treeNav.css";
import "../css/fontello.css";

export default 
class TreeNav extends React.Component {

  constructor(props){
    super(props);   
  
    this.state = { scheme : this.props.scheme};
  }

  renderSubmenu(level, submenu, obj, outObjList){
   
    if (typeof(obj) == "string"){

      outObjList.push(<section key={level + "." + obj} 
                               className="treeNav-leaf-container"
                               draggable="true"
                               style={{ cursor: "default", paddingLeft: 10 + level * 5}}
                               onDragStart = {(e) => e.dataTransfer.setData('text', submenu + obj) } >
                               {obj}
                      </section>);      
    }  
    else{

      outObjList.push(<section key={level + "." + obj.submenu}
                               className="treeNav-node-container"
                               style={{ cursor: "default", paddingLeft: 10 + level * 5}}
                               onClick= { (e) => {obj.isShow = !obj.isShow;
                                                 this.setState({ scheme : this.props.scheme});} }> 
                               {obj.submenu} 
                               <span className= { obj.isShow ? "icon-down-dir" : "icon-left-dir" } />
                      </section>); 

      submenu += obj.submenu + ".";
      if (obj.isShow){
        for(let i of obj.items)
          outObjList.push(this.renderSubmenu(level + 1, submenu, i, outObjList)); 
      }
    }  
  }

  render(){

    let outObjList = [];

    for(let obj of this.props.scheme)
      this.renderSubmenu(0, "", obj, outObjList);

    return <div style={{ borderRadius: "3px 3px 3px 3px", overflow: "auto"}}> {outObjList}  </div>
  }
}

TreeNav.propTypes = {
   // scheme_: PropTypes.object,
};