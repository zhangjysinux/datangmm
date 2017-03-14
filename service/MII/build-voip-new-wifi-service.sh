#!/bin/bash
Name=voipWifiService	
Version=1.0.5
if [ -d $Name'-'$Version  ]; then
    echo "$Name'-'$Version exist"
    rm -rf $Name'-'$Version
else 
    echo "$Name'-'$Version not exist"
fi

path=$Name
mkdir $Name'-'$Version
cp -rf $path  $Name'-'$Version

if [ -f  $Name'-'$Version.tar.gz ]; then
    echo "$Name'-'$Version.tar.gz exist"
    rm -rf $Name'-'$Version.tar.gz
else
    "$Name'-'$Version.tar.gz not exist"
fi

tar cvf  $Name'-'$Version.tar.gz $Name'-'$Version
 
cp $Name'-'$Version.tar.gz ~/rpmbuild/SOURCES/
spec=$Name/$Name.spec					
cp $spec ~/rpmbuild/SPECS/

/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 -t syberos-target-armv7tnhl rpmbuild -ba ~/rpmbuild/SPECS/${Name}.spec

#清理中间生成的包
rm -rf $Name-$Version
rm -rf $Name-$Version.tar.gz
