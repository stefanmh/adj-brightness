#!/bin/bash

set -o errexit
set -o xtrace

g++ adj-brightness.cpp -o adj-brightness -Wall -ggdb -std=c++0x -l'stdc++fs'
sudo chown root adj-brightness
sudo chmod u+s adj-brightness

