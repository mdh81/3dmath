#!/bin/bash

if ! [ $# -eq 1 ]; then
    echo "Usage: $0 <latex file>"
    exit -1
fi

texFile=$(basename $1)
problemName=${texFile%.*}

latexml -dest=web/$problemName.xml $1 
latexmlpost --dest=web/$problemName.html web/$problemName.xml

rm $problemName*.log
