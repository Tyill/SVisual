/* eslint-disable no-unused-vars */
// @flow
import React from "react"
import "../css/treeNav.css";
import "../css/fontello.css";

/*::   
export
type navSchemeType = { submenu : string,
                       isShow : boolean,
                       iActive : boolean,
                       items : Array<string | navSchemeType>
}

type Props = {
  scheme : Array<string | navSchemeType>,
  onDoubleClick : (sname : string) => void
}

type State = {
  scheme: Array<string | navSchemeType>, 
}
*/

export default 
class TreeNav extends React.Component/*:: <Props, State>*/ {
 
  constructor(props){
    super(props);   
  
    this.state = { scheme : this.props.scheme};
  }

  renderSubmenu(level /*:: : number*/, submenu/*:: : string*/, obj, outObjList){
   
    if (typeof(obj) == "string"){

      outObjList.push(<section key={level + "." + obj} 
                               className="treeNav-leaf-container"
                               draggable="true"
                               style={{ cursor: "default", paddingLeft: 10 + level * 5}}
                               onDragStart = {(e) => e.dataTransfer.setData('text', submenu + obj) }
                               onDoubleClick = { (e) => this.props.onDoubleClick(obj + submenu.slice(0, -1)) } >
                               {obj}
                      </section>);      
    }  
    else{

      outObjList.push(<section key={level + "." + obj.submenu}
                               className="treeNav-node-container"
                               style={{ cursor: "default", 
                                        paddingLeft: 10 + level * 5,
                                        backgroundColor : (obj.isActive ? "silver" : "rgb(200, 20, 20)") }}
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

    return <div style={{ borderRadius: "3px 3px 3px 3px", 
                         overflow: "auto",
                         maxHeight : document.documentElement.clientHeight * 0.8 + "px"}}>
            {outObjList}
           </div>
  }
}