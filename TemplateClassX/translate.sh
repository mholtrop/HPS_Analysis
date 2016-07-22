#!/bin/bash

if [ "$1" = "" ]; then
    echo "Using ${PWD##/*/} for new name"
    new_name=${PWD##/*/}
else
    new_name=$1
fi

old_name=TemplateClassX 

rm -rf build

test -d Save_Old || mkdir Save_Old
for f in *.xcconfig; do 
    newf=`echo ${f} | sed "s/${old_name}/${new_name}/g"` 
    sed "s/${old_name}/${new_name}/g" ${f} > ${newf}
    mv ${f} Save_Old
    echo "${f} -> ${newf}"
done

mkdir ${new_name}
for f in ${old_name}/*; do 
    newf=`echo ${f} | sed "s/${old_name}/${new_name}/g"` 
    sed "s/${old_name}/${new_name}/g" ${f} > ${newf}
    echo "${f} -> ${newf}"
done
mv ${old_name} Save_Old

mkdir Test${new_name}
for f in Test${old_name}/*; do 
    newf=`echo ${f} | sed "s/${old_name}/${new_name}/g"` 
    sed "s/${old_name}/${new_name}/g" ${f} > ${newf}
    echo "${f} -> ${newf}"
done
mv Test${old_name} Save_Old

mkdir -p ${new_name}.xcodeproj/project.xcworkspace

sed "s/${old_name}/${new_name}/g" ${old_name}.xcodeproj/project.pbxproj >  ${new_name}.xcodeproj/project.pbxproj
sed "s/${old_name}/${new_name}/g" ${old_name}.xcodeproj/project.xcworkspace/contents.xcworkspacedata  >  ${new_name}.xcodeproj/project.xcworkspace/contents.xcworkspacedata 
mv ${old_name}.xcodeproj Save_Old
