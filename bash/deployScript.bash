#! /bin/bash

attribut1=( abstract1 city1 dance1 elgreco1 starik1 tree3 tree4)
attribut2=( equilibrate diffOriResult ) 


for i in ${attribut1[*]}
do

  attributs="$i ${attribut2[0]} $i ${attribut2[1]} -gif"
  echo "we process the compareTwoSetOfImagesScriptAndGenerateGif.bash with attributs: $attributs"
  ./compareTwoSetOfImagesScriptAndGenerateGif.bash $attributs 	
  
done  

echo
echo "end"

