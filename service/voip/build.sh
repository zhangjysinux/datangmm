#1.编译yasm

cd lib/yasm-1.3.0
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 ./configure --build=armv7tnhl-meego-linux-gnueabi --prefix=/parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/  
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make clean 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 -R make install 
echo ready
read -p "Press any key to continue. ---yasm"

#2.编译libiconv
cd ../../lib/libiconv-1.14
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 ./configure --build=armv7tnhl-meego-linux-gnueabi --prefix=/parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/  
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make clean 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 -R make install 
echo ready
read -p "Press any key to continue.---libiconv"

#3.编译v4l2
cd ../../lib/v4l-utils-1.8.1
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 ./configure --build=armv7tnhl-meego-linux-gnueabi --prefix=/parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/  
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make clean 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 -R make install 
echo ready
read -p "Press any key to continue.---v4l2"

#4.编译SDL
cd ../../lib/SDL2-2.0.4
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 CFLAGS="-O3 -fPIC" ./configure --build=armv7tnhl-meego-linux-gnueabi --enable-video-opengl=no --enable-video-opengles=no --enable-video-opengles1=no --enable-video-opengles2=no --prefix=/parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/ 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make clean 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 -R make install 
echo ready
read -p "Press any key to continue.---SDL"

#5.编译x264
cd ../../lib/x264-snapshot-20160218-2245
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 CFLAGS="-O3 -fPIC" ./configure --build=armv7tnhl-meego-linux-gnueabi --enable-static --disable-shared --enable-pic --prefix=/parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make clean 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 -R make install 
echo ready
read -p "Press any key to continue.---x264"

#6.编译ffmpeg
cd ../../lib/ffmpeg-2.5.11
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 CFLAGS="-O3 -fPIC" ./configure --enable-shared --disable-static --enable-memalign-hack --enable-gpl --enable-encoder=h263p --prefix=/parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make clean
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 -R make install 
echo ready
read -p "Press any key to continue.---ffmpeg"

#7.编译pjsip
cd ../../lib/pjproject-2.4.5/ 
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 CFLAGS="-O3 -fPIC" ./configure --build=armv7tnhl-meego-linux-gnueabi --disable-openh264 --prefix=/parentroot/srv/syberos/targets/syberos-target-armv7tnhl/usr/
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make clean
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make dep
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 make
/srv/syberos/sdks/sdk/syberos-sdk-chroot sb2 -R make install
echo ready
read -p "Press any key to continue.---pjsip"

