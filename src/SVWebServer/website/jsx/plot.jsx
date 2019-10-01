/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";

export default 
class Plot extends React.Component {
  
  render(){
  
    return <div style={style}> Graph Plot </div>
  }
}

const style = {  
  border: "1px solid red",
  boxSizing: "border-box",
}

Plot.propTypes = {
   // scheme_: PropTypes.object,
};