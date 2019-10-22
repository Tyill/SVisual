const webpack = require('webpack');

module.exports = {
  entry: [
    'webpack-dev-server/client/?http://localhost:8080',
    './jsx/app.jsx',
  ],  
  output: {
    publicPath: 'js/',
    path: __dirname + '/js/',
    filename: 'bundle.js'
  },
  devtool:  '#sourcemap',
  mode: 'development',
  module: {
    rules: [
      { test: /\.css$/, loader: 'style-loader!css-loader'},
      { test: /\.jsx?$/, exclude: /(node_modules)/, use: ['babel-loader'] }
    ]
   },
  devServer: {
    hot: true,
    proxy: {
      '/api': 'http://localhost:3000',      
    },   
  },  
  plugins: [new webpack.HotModuleReplacementPlugin()]   
}