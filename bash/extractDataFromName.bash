#! /bin/bash


echo "image,strings,error" > data.dat 
ls d:*.jpg | sed  's/.*-n:\(.*\)-s:\([0-9]*\).*-e:\([0-9]*\).*/\1,\2,\3/' >> data.dat 

