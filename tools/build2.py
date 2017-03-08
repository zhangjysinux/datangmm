#coding: utf-8

import pexpect
import commands
import os,sys,time,re

orderStr = ''
versionStr = '1.0.11'

def build_phonebook():
	os.system('export CHROOT=/home/orange/SyberOS-SDK/jail/kchroot && \
		   export PATH=$PATH:/home/orange/SyberOS-SDK/SDKQt/qt530/usr/lib/qt5/bin:/usr/bin && \
		   export QTDIR=/home/orange/SyberOS-SDK/SDKQt/qt530/usr/share/qt5 && \
		   /home/orange/SyberOS-SDK/jail/kchroot qmake /home/orange/datangmplus/app/phonebook/phonebook.pro -r -spec linux-g++-armv7l CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug && \
		   /home/orange/SyberOS-SDK/jail/kchroot make in /home/orange/datangmplus/app/build-phonebook-Device-Debug && \
		   /home/orange/SyberOS-SDK/jail/kchroot buildpkg /home/orange/datangmplus/app/phonebook/phonebook.pro')
	#'/usr/bin/ins-tool -siu /tmp/com.sinux.phonebook-1.0.8.sop && rm -f /tmp/com.sinux.phonebook-1.0.8.sop'
	#/home/orange/SyberOS-SDK/jail/kchroot make clean in /home/orange/datangmplus/app/build-phonebook-Device-Debug && \

def replaceFile(fileName, dstStr, srcStr):
	proFile = open(fileName, 'r')
	content = proFile.read()
	proFile.close()
	match = re.compile(dstStr)
	contentReplaced = re.sub(match,srcStr,content)
	print contentReplaced
	replacedProFile = open(fileName, 'w')
	replacedProFile.write(contentReplaced)
	replacedProFile.close()
	print 'saved'

def build_multiNet(appName):
	os.system('rm -rf ./%s*'%(appName))
	os.system('cp -rf ../service/voipWifiService ./voipWifiService')
	os.system('cp -rf ../service/build-voip-wifi-service.sh ./build-voip-wifi-service.sh && chmod u+x build-voip-wifi-service.sh')
	
	time.sleep(1)

	replaceFile('./voipWifiService/voipWifiService.pro', r"TARGET = .*", 'TARGET = %s'%(appName))
	replaceFile('./voipWifiService/voipWifiService.pro', r"systemdservice.files = service/.*", 'systemdservice.files = service/%s.service'%(appName))

	replaceFile('./voipWifiService/service/voipWifiService.service', r"ExecStart=/usr/bin/.*", 'ExecStart=/usr/bin/%s'%(appName))

	replaceFile('./voipWifiService/voipWifiService.spec', r"Name:.*", 'Name:		%s'%(appName))
	replaceFile('./voipWifiService/voipWifiService.spec', r"ln -s ../.*  ", 'ln -s ../%s.service  '%(appName))
	replaceFile('./voipWifiService/voipWifiService.spec', r"user-session.target.wants/voip.*", 'user-session.target.wants/%s.service'%(appName))
	replaceFile('./voipWifiService/voipWifiService.spec', r"{_bindir}/.*", '{_bindir}/%s'%(appName))
	replaceFile('./voipWifiService/voipWifiService.spec', r"Version:	.*", 'Version:	%s'%(versionStr))

	replaceFile('../service/build-voip-wifi-service.sh', r"Name=.*", 'Name=%s'%(appName))
	replaceFile('./build-voip-wifi-service.sh', r"Version=.*", 'Version=%s'%(versionStr))
	
	os.system('mv ./voipWifiService/*.pro ./voipWifiService/%s.pro'%(appName))
	os.system('mv ./voipWifiService/service/*.service ./voipWifiService/service/%s.service'%(appName))
	os.system('mv ./voipWifiService/*.spec ./voipWifiService/%s.spec'%(appName))

	replaceFile('./voipWifiService/voipdbus.h', r"com.sinux..*", 'com.sinux.%s"'%(appName[:-7]))	

	os.system('rm -rf ./voipWifiService/*.user')	
	
	os.system('mv ./voipWifiService ./%s'%(appName))
	
	os.system('./build-voip-wifi-service.sh')
	time.sleep(1)

def build_adhoc():
	os.system('rm -rf ./%s*'%('voipAdHocService'))
	os.system('rm -rf ./%s*'%('voipLteService'))
	os.system('rm -rf ./%s*'%('voipWifiService'))
	os.system('rm -rf ./%s*'%('voipBattleService'))
	build_multiNet('voipAdHocService')


def build_wifi():
	os.system('rm -rf ./%s*'%('voipAdHocService'))
	os.system('rm -rf ./%s*'%('voipLteService'))
	os.system('rm -rf ./%s*'%('voipWifiService'))
	os.system('rm -rf ./%s*'%('voipBattleService'))
	build_multiNet('voipWifiService')


def build_lte():
	os.system('rm -rf ./%s*'%('voipAdHocService'))
	os.system('rm -rf ./%s*'%('voipLteService'))
	os.system('rm -rf ./%s*'%('voipWifiService'))
	os.system('rm -rf ./%s*'%('voipBattleService'))
	build_multiNet('voipLteService')

def build_battle():
	os.system('rm -rf ./%s*'%('voipBattleService'))
	os.system('rm -rf ./%s*'%('voipBattleService'))
	os.system('rm -rf ./%s*'%('voipBattleService'))
	os.system('rm -rf ./%s*'%('voipBattleService'))
	build_multiNet('voipBattleService')

def build_ptt():
	os.system('cd ../service/ && \
	 	./build-ptt-media-service.sh')
	time.sleep(1)

def build_pjsip():
	os.system('cd ../service/ && \
	 	./build-pjsip.sh')

def cp_all():
        buildPath = os.path.abspath(os.curdir) + '/allbuilds'
        if not os.path.exists(buildPath) :
            os.makedirs(buildPath)

	os.system('	cp -rf ../app/build-phonebook-Device-Debug/*.sop ./allbuilds && \
			cp -rf ../app/build-PhoneCall-Device-Debug/*.sop ./allbuilds && \
			cp -rf ../app/build-backupdate-Device-Debug/*.sop ./allbuilds && \
			cp -rf /home/orange/rpmbuild/RPMS/armv7tnhl/*.rpm ./allbuilds && \
			cp -rf ../service/build-dialadaptor-Device-Debug/*.sop ./allbuilds')

	os.system('cp -rf /home/orange/rpmbuild/RPMS/armv7tnhl/*.rpm ./allbuilds')

	os.system('cp -rf /home/orange/Downloads/nload1 ./allbuilds')

	os.system('cp /home/orange/project_files/元心安装包/语音切换问题包/pulseaudio-controllerd-1.0-1.syberos.3.1.armv7tnhl.rpm ./allbuilds')

	os.system('scp ./installvoip developer@192.168.100.100:~ && \
		   scp ./allbuilds/* developer@192.168.100.100:~')
	
	cp_videolib()
	
def cp_videolib():
	buildPath = os.path.abspath(os.curdir) + '/allbuilds/videolib'
        if not os.path.exists(buildPath) :
            os.makedirs(buildPath)
	
	os.system('cp /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libSDL*  ./allbuilds/videolib && \
			cp /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libav* ./allbuilds/videolib && \
			cp /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libswscale.so.3 ./allbuilds/videolib && \
			cp -r /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libv4l* ./allbuilds/videolib && \
			cp /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libswresample.so.1 ./allbuilds/videolib && \
			cp /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libiconv.so.2 ./allbuilds/videolib')
	

def login_root_mount():

    ssh = pexpect.spawn("ssh developer@192.168.100.100")

    index = ssh.expect([pexpect.TIMEOUT, 'yes/no', '.ssword:*', 'ssh-keygen -f "./home/orange/.ssh/known_hosts"'], 2)
    if index == 0:
        print "timeout............"
    elif index == 1:
        ssh.sendline("yes")
    elif index == 2:
        ssh.sendline("system")
    elif index == 3:
        ssh.sendline('ssh-keygen -f "/home/orange/.ssh/known_hosts"')

    ssh.sendline("su")

    if ssh.expect('Password:') == 0:
        ssh.sendline('system')

    ssh.sendline("pwd")

    if ssh.expect('$') == 0:
        ssh.sendline("mount -o remount,rw /")

    ssh.sendline("ps -ef | grep voip")
    ssh.send("\r\r\r")
    ssh.sendline("ps -ef | grep Co")
    ssh.send("\r\r\r")

    ssh.sendline("cp /data/data/com.sinux.phonebook/contact.db /data/developer")

    ssh.sendline("journalctl -af | grep voipWifi")


    ssh.send("\r\r\r\r\r\r")

    ssh.interact()


def login_root_mount_install():

    ssh = pexpect.spawn("ssh developer@192.168.100.100")

    index = ssh.expect([pexpect.TIMEOUT, 'yes/no', '.ssword:*', 'ssh-keygen -f "./home/orange/.ssh/known_hosts"'], 2)
    if index == 0:
        print "timeout............"
    elif index == 1:
        ssh.sendline("yes")
    elif index == 2:
        ssh.sendline("system")
    elif index == 3:
        ssh.sendline('ssh-keygen -f "/home/orange/.ssh/known_hosts"')

    ssh.sendline("su")

    if ssh.expect('Password:') == 0:
        ssh.sendline('system')

    ssh.sendline("pwd")

    if ssh.expect('$') == 0:
        ssh.sendline("mount -o remount,rw /")

    ssh.sendline("./installvoip")

    ssh.send("\r\r\r\r\r\r")

    ssh.interact()


def cp_login_root_mount():
	os.system('scp /home/orange/rpmbuild/RPMS/armv7tnhl/voipAdHocService-1.0.8-1.armv7tnhl.rpm developer@192.168.100.100:~')
	os.system('scp /home/orange/rpmbuild/RPMS/armv7tnhl/voipLteService-1.0.8-1.armv7tnhl.rpm developer@192.168.100.100:~')
	login_root_mount()

def reboot():
	ssh = pexpect.spawn("ssh developer@192.168.100.100")
	index = ssh.expect([pexpect.TIMEOUT, 'yes/no', '.ssword:*', 'ssh-keygen -f "./home/orange/.ssh/known_hosts"'], 2)
	if index == 0:
		print "timeout............"
	elif index == 1:
		ssh.sendline("yes")
	elif index == 2:
		ssh.sendline("system")
	elif index == 3:
		ssh.sendline('ssh-keygen -f "/home/orange/.ssh/known_hosts"')
	ssh.sendline("su")
	if ssh.expect('Password:') == 0:
		ssh.sendline('system')
	ssh.sendline("pwd")
	if ssh.expect('$') == 0:
		ssh.sendline("mount -o remount,rw /")
	ssh.sendline("toolbox reboot")
	ssh.interact()


if __name__ == '__main__':
    
	if len(sys.argv) <= 1:
		sys.exit()
	else :
		orderStr = sys.argv[1]
		print 'build %s' %(orderStr)


	
	if orderStr == 'login' :
		login_root_mount()
	elif orderStr == 'logininstall' :
		login_root_mount_install()
	elif orderStr == 'cplogin' :
		cp_all()
		cp_login_root_mount()
	elif orderStr == 'adhoc' :
		build_adhoc()
	elif orderStr == 'lte':
		build_lte()
	elif orderStr == 'ptt':
		build_ptt()
	elif orderStr == 'pjsip':
		build_pjsip()
	elif orderStr == 'cpall':
		cp_all()
	elif orderStr == 'reboot':
		reboot()
	elif orderStr == 'phonebook':
		build_phonebook()
	elif orderStr == 'wifi':
		build_wifi()
	elif orderStr == 'battle':
		build_battle()
	elif orderStr == 'videolib':
		cp_videolib()
