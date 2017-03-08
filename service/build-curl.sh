#!/bin/bash

export HOMEPATH=/home/lilf

export CHROOT=$HOMEPATH/SyberOS-SDK2/jail/kchroot
export PATH=$PATH:$HOMEPATH/SyberOS-SDK2/jail/opt/cross/bin/
echo $PATH

cd curl-7.50.0
$CHROOT CFLAGS="-O3 -fPIC"
$CHROOT ./configure --host=armv7tnhl-meego-linux-gnueabi  --prefix=$HOMEPATH/SyberOS-SDK2/jail/opt/cross/armv7tnhl-meego-linux-gnueabi/sys-root/usr/ --disable-shared --enable-static --without-ssl

$CHROOT make clean
$CHROOT make
$CHROOT make install
cp libcurl.pc $HOMEPATH/SyberOS-SDK/jail/usr/lib/pkgconfig/

# --disable-ftp --disable-ipv6 --disable-rtsp --disable-tftp --disable-telnet --disable-largefile --disable-smtp --disable-imap
#/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2
