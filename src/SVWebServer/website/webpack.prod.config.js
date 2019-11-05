module.exports = {
  entry: './jsx/app.jsx',  
  output: {
    publicPath: 'js/',
    path: __dirname + '/js/',
    filename: 'bundle.js'
  },
  mode: 'production',
    module: {
    rules: [
      { test: /\.css$/, loader: 'style-loader!css-loader'},
      { test: /\.jsx?$/, exclude: /(node_modules)/, use: 'babel-loader' }
    ],    
  },  
}