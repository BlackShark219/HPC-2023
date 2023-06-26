#!/bin/bash
echo "-85040 49900 300" | awk '{xmin=$1; xmax=$2; steps=$3; dx=(xmax-xmin)/steps;x=xmin; for (i=0; i<steps; i++){print x" "x+dx;x+=dx}}' >solv.inp
