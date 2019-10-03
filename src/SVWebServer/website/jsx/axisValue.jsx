/* eslint-disable no-unused-vars */

import React from "react"
import PropTypes from "prop-types";

export default
class AxisValue extends React.Component {
    
  render(){

    
    return <div style={style}> Graph Value  </div>
  }
}

const style = {  
  border: "1px solid blue",
  boxSizing: "border-box",
}

AxisValue.propTypes = {
   // scheme_: PropTypes.object,
};