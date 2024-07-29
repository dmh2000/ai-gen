#!/bin/bash

# create a directory (manually) for the app
# > mkdir canapp

# >conda create -n aider python=3
# includes python 3 (latest)

# activate conda virtual environment
conda activate aider
which python
python --version

# install aider-chat
# pip install aider-chat
aider --version

# create git archive
git init .
git branch -m main

# setup directory structure
mkdir c
mkdir can
mkdir g
cd g && go mod init can
mkdir ts 
tree


