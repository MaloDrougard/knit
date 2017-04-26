#! /bin/bash

if [ "$#" -le 3  ]
then
  echo "Illegal number of parameters, at least 4 param required: setImage1Atrribut1 setImage1Atrribut2 setImage2Atrribut1 setImage2Atrribut2 [-gif]"
  exit -2	
fi



set1Attr1="$1"
set1Attr2="$2"
set2Attr1="$3"
set2Attr2="$4"


array1=( $(ls -tr d:*$set1Attr1*$set1Attr2*.jpg) ) 
array2=( $(ls -tr d:*$set2Attr1*$set2Attr2*.jpg) ) 

count1="0"
count2="0"

for i in ${array1[*]} 
do
  count1=$(expr $count1 + 1)
done

for i in ${array2[*]} 
do
  count2=$(expr $count2 + 1)
done


# Test if the number of file in each array
if [ "${count1}" !=  "${count2}" ] 
then
  echo "We have not the same amount of file to compare!"
  exit -1
else
  echo "The correct amount of files was provided"
fi


for i in $(seq 0 $( expr $count1 -  1 ) ) 
do
  echo $i
  file1=${array1[$i]}
  file2=${array2[$i]}
  outname="out${i}Compare${set1Attr1}${set1Attr2}${set2Attr1}${set2Attr2}.jpg"

  # here we use the compare cmd of imagemagik
  compare "$file1" "$file2" -metric mae -fuzz 1% -compose src -quality 100 -highlight-color IndianRed4 -lowlight-color White $outname  

done



delay="60"
loop="3"

if [ "$5" = "-gif" ] 
then
  echo
  echo "Gif generation option is activated"
  convert -delay $delay -loop $loop ${array1[*]} "${set1Attr1}${set1Attr2}".gif
  convert -delay $delay -loop $loop ${array2[*]} "${set2Attr1}${set2Attr2}".gif
  convert -delay $delay -loop $loop $(ls -tr out*Compare${set1Attr1}${set1Attr2}${set2Attr1}${set2Attr2}.jpg ) "${set1Attr1}${set1Attr2}Diff${set2Attr2}".gif

fi


