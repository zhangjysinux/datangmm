#coding: utf-8

import pexpect
import commands
import os,sys,time,re

orderStr = ''
versionStr = '2.0.4'

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
	replaceFile('./voipWifiService/voipWifiService.pro', r"DEFINES.*", 'DEFINES += %s'%(appName))
	replaceFile('./voipWifiService/voipWifiService.pro', r"systemdservice.files = service/.*", 'systemdservice.files = service/%s.service'%(appName))

	replaceFile('./voipWifiService/service/voipWifiService.service', r"ExecStart=/usr/bin/.*", 'ExecStart=/usr/bin/%s'%(appName))

	replaceFile('./voipWifiService/voipWifiService.spec', r"Name:.*", 'Name:		%s'%(appName))
	replaceFile('./voipWifiService/voipWifiService.spec', r"ln -s ../.*  ", 'ln -s ../%s.service  '%(appName))
	replaceFile('./voipWifiService/voipWifiService.spec', r"user-session.target.wants/voip.*", 'user-session.target.wants/%s.service'%(appName))
	replaceFile('./voipWifiService/voipWifiService.spec', r"{_bindir}/.*", '{_bindir}/%s'%(appName))
	replaceFile('./voipWifiService/voipWifiService.spec', r"Version:	.*", 'Version:	%s'%(versionStr))

	replaceFile('./build-voip-wifi-service.sh', r"Name=.*", 'Name=%s'%(appName))
	replaceFile('./build-voip-wifi-service.sh', r"Version=.*", 'Version=%s'%(versionStr))
	
	os.system('mv ./voipWifiService/*.pro ./voipWifiService/%s.pro'%(appName))
	os.system('mv ./voipWifiService/service/*.service ./voipWifiService/service/%s.service'%(appName))
	os.system('mv ./voipWifiService/*.spec ./voipWifiService/%s.spec'%(appName))

	replaceFile('./voipWifiService/voipdbus.h', r"com.sinux..*", 'com.sinux.%s"'%(appName[:-7]))	

	os.system('rm -rf ./voipWifiService/*.user')	
	
	os.system('mv ./voipWifiService ./%s'%(appName))
	
	os.system('./build-voip-wifi-service.sh')
	time.sleep(1)

def build_change_p2p_project(appName):
	os.system('rm -rf ./%s*'%(appName))
	os.system('cp -rf ../service/voipP2PService ./voipP2PService')
	os.system('cp -rf ../service/build-voip-p2p-service.sh ./build-voip-p2p-service.sh && chmod u+x build-voip-wifi-service.sh')
	
	time.sleep(1)

	replaceFile('./voipP2PService/voipP2PService.pro', r"TARGET = .*", 'TARGET = %s'%(appName))
	replaceFile('./voipP2PService/voipP2PService.pro', r"DEFINES.*", 'DEFINES += %s'%(appName))
	replaceFile('./voipP2PService/voipP2PService.pro', r"systemdservice.files = service/.*", 'systemdservice.files = service/%s.service'%(appName))

	replaceFile('./voipP2PService/service/voipP2PService.service', r"ExecStart=/usr/bin/.*", 'ExecStart=/usr/bin/%s'%(appName))

	replaceFile('./voipP2PService/voipP2PService.spec', r"Name:.*", 'Name:		%s'%(appName))
	replaceFile('./voipP2PService/voipP2PService.spec', r"ln -s ../.*  ", 'ln -s ../%s.service  '%(appName))
	replaceFile('./voipP2PService/voipP2PService.spec', r"user-session.target.wants/voip.*", 'user-session.target.wants/%s.service'%(appName))
	replaceFile('./voipP2PService/voipP2PService.spec', r"{_bindir}/.*", '{_bindir}/%s'%(appName))
	replaceFile('./voipP2PService/voipP2PService.spec', r"Version:	.*", 'Version:	%s'%(versionStr))

	replaceFile('./build-voip-p2p-service.sh', r"Name=.*", 'Name=%s'%(appName))
	replaceFile('./build-voip-p2p-service.sh', r"Version=.*", 'Version=%s'%(versionStr))
	
	os.system('mv ./voipP2PService/*.pro ./voipP2PService/%s.pro'%(appName))
	os.system('mv ./voipP2PService/service/*.service ./voipP2PService/service/%s.service'%(appName))
	os.system('mv ./voipP2PService/*.spec ./voipP2PService/%s.spec'%(appName))

	replaceFile('./voipP2PService/voipdbus.h', r"com.sinux..*", 'com.sinux.%s"'%(appName[:-7]))	

	os.system('rm -rf ./voipP2PService/*.user')	
	
	os.system('mv ./voipP2PService ./%s'%(appName))
	
	os.system('./build-voip-p2p-service.sh')
	time.sleep(1)

def build_change_ptt_project(appName):
	os.system('rm -rf ./%s*'%(appName))
	os.system('cp -rf ../service/voipPTTService ./voipPTTService')
	os.system('cp -rf ../service/build-voip-ptt-service.sh ./build-voip-ptt-service.sh && chmod u+x build-voip-ptt-service.sh')
	
	time.sleep(1)

	replaceFile('./voipPTTService/voipPTTService.pro', r"TARGET = .*", 'TARGET = %s'%(appName))
	replaceFile('./voipPTTService/voipPTTService.pro', r"DEFINES.*", 'DEFINES += %s'%(appName))
	replaceFile('./voipPTTService/voipPTTService.pro', r"systemdservice.files = service/.*", 'systemdservice.files = service/%s.service'%(appName))

	replaceFile('./voipPTTService/service/voipPTTService.service', r"ExecStart=/usr/bin/.*", 'ExecStart=/usr/bin/%s'%(appName))

	replaceFile('./voipPTTService/voipPTTService.spec', r"Name:.*", 'Name:		%s'%(appName))
	replaceFile('./voipPTTService/voipPTTService.spec', r"ln -s ../.*  ", 'ln -s ../%s.service  '%(appName))
	replaceFile('./voipPTTService/voipPTTService.spec', r"user-session.target.wants/voip.*", 'user-session.target.wants/%s.service'%(appName))
	replaceFile('./voipPTTService/voipPTTService.spec', r"{_bindir}/.*", '{_bindir}/%s'%(appName))
	replaceFile('./voipPTTService/voipPTTService.spec', r"Version:	.*", 'Version:	%s'%(versionStr))

	replaceFile('./build-voip-ptt-service.sh', r"Name=.*", 'Name=%s'%(appName))
	replaceFile('./build-voip-ptt-service.sh', r"Version=.*", 'Version=%s'%(versionStr))
	
	os.system('mv ./voipPTTService/*.pro ./voipPTTService/%s.pro'%(appName))
	os.system('mv ./voipPTTService/service/*.service ./voipPTTService/service/%s.service'%(appName))
	os.system('mv ./voipPTTService/*.spec ./voipPTTService/%s.spec'%(appName))

	replaceFile('./voipPTTService/voipdbus.h', r"com.sinux..*", 'com.sinux.%s"'%(appName[:-7]))	

	os.system('rm -rf ./voipPTTService/*.user')	
	
	os.system('mv ./voipPTTService ./%s'%(appName))
	
	os.system('./build-voip-ptt-service.sh')
	time.sleep(1)

def build_adhoc():
	os.system('rm -rf ./%s*'%('voip*Service'))
	build_multiNet('voipAdHocService')


def build_wifi():
	os.system('rm -rf ./%s*'%('voip*Service'))
	build_multiNet('voipWifiService')


def build_lte():
	os.system('rm -rf ./%s*'%('voip*Service'))
	build_multiNet('voipLteService')

def build_battle():
	os.system('rm -rf ./%s*'%('voip*Service'))
	build_multiNet('voipBattleService')

def build_p2padhoc():
	os.system('rm -rf ./%s*'%('voip*Service'))
	build_change_p2p_project('voipAdHocP2PService')

def build_p2pwifi():
	os.system('rm -rf ./%s*'%('voip*Service'))
	build_change_p2p_project('voipWifiP2PService')

def build_p2plte():
	os.system('rm -rf ./%s*'%('voip*Service'))
	build_change_p2p_project('voipLteP2PService')

def build_ptt():
	os.system('rm -rf ./%s*'%('voip*Service'))
	build_change_ptt_project('voipPTTService')


def build_video_lib():
	os.system(' 	cd ../service/ && \
			./build-voip-lib-service.sh')


def build_pjsip():
	os.system('cd ../service/voip/ && \
	 	./buildpj.sh')

def cp_all():
        buildPath = os.path.abspath(os.curdir) + '/allbuilds'
        if not os.path.exists(buildPath) :
            os.makedirs(buildPath)

	os.system('	\
			 \
			 \
			 \
			cp -rf /home/lilf/rpmbuild/RPMS/armv7tnhl/*.rpm ./allbuilds/ \
			')

	os.system('cp -rf /home/lilf/rpmbuild/RPMS/armv7tnhl/*.rpm ./allbuilds')

	os.system('cp -rf /home/lilf/orange/Downloads/nload1 ./allbuilds')

	os.system('cp -rf ./installvoip ./allbuilds')

	os.system('cp /home/lilf/orange/project_files/元心安装包/语音切换问题包/*.rpm ./allbuilds')

	os.system('scp ./allbuilds/* developer@192.168.100.100:~')
	
	cp_videolib()

def cp_tophone():
        buildPath = os.path.abspath(os.curdir) + '/allbuilds'
        if not os.path.exists(buildPath) :
            os.makedirs(buildPath)

	os.system('scp ./installvoip developer@192.168.100.100:~ && \
		   scp ./allbuilds/* developer@192.168.100.100:~')

	#os.system('scp -r ./allbuilds/videolib developer@192.168.100.100:~')
	
def cp_videolib():
	buildPath = os.path.abspath(os.curdir) + '/allbuilds/videolib'
        if not os.path.exists(buildPath) :
            os.makedirs(buildPath)
	
	os.system('cp -rf /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libSDL*  ../service/voipLibService/videolib/ && \
		cp -rf /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libav* ../service/voipLibService/videolib/ && \
		cp -rf /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libswscale.so.3* ../service/voipLibService/videolib/ && \
		cp -rf /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libv4l* ../service/voipLibService/videolib/ && \
		cp -rf /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libswresample.so.1* ../service/voipLibService/videolib/ && \
		cp -rf /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libiconv.so.2* ../service/voipLibService/videolib/ && \
		cp -rf /srv/syberos/targets/syberos-target-armv7tnhl/usr/lib/libpostproc.so* ../service/voipLibService/videolib/')
	

def login_root_mount():

    ssh = pexpect.spawn("ssh developer@192.168.100.100")

    index = ssh.expect([pexpect.TIMEOUT, 'yes/no', '.ssword:*', 'WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!', 'Overwrite (y/n)?', 'Enter passphrase (empty for no passphrase):', 'Enter same passphrase again:', 'Are you sure you want to continue connecting (yes/no)?'], 1)
    if index == 0:
        print "timeout............"
    elif index == 1:
        ssh.sendline("yes")
    elif index == 2:
        ssh.sendline("system")
    elif index == 3:
        ssh.sendline('ssh-keygen -f ~/.ssh/known_hosts')
    elif index == 4:
	ssh.sendline('y')
    elif index == 5:
	ssh.sendline('system')
    elif index == 6:
	ssh.sendline('system')
    elif index == 7:
	ssh.sendline('yes')

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

def delete_ssh_account():

    os.system('rm -rf ~/.ssh/known_hosts')
   
   
    


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
	os.system('scp /home/lilf/rpmbuild/RPMS/armv7tnhl/voipAdHocService-1.0.8-1.armv7tnhl.rpm developer@192.168.100.100:~')
	os.system('scp /home/lilf/rpmbuild/RPMS/armv7tnhl/voipLteService-1.0.8-1.armv7tnhl.rpm developer@192.168.100.100:~')
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
	elif orderStr == 'cp':
		cp_tophone()
	elif orderStr == 'reboot':
		reboot()
	elif orderStr == 'phonebook':
		build_phonebook()
	elif orderStr == 'wifi':
		build_wifi()
	elif orderStr == 'battle':
		build_battle()
	elif orderStr == 'videolib':
		#cp_videolib()
		build_video_lib()
	elif orderStr == 'adhocp2p':
		build_p2padhoc()
	elif orderStr == 'wifip2p':
		build_p2pwifi()
	elif orderStr == 'ltep2p':
		build_p2plte()
	elif orderStr == 'buildall':
		build_wifi()
		build_adhoc()
		build_lte()
		build_p2pwifi()
		build_p2padhoc()
		build_p2plte()
		build_ptt()
		build_video_lib()
		
		
