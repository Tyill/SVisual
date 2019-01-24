#!/bin/bash
set -e

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew update -qq;
fi

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get update -qq;
fi