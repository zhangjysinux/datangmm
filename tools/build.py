import pexpect
import commands
import os,sys,time
import xml.dom.minidom
import linecache
orderStr = ''
user = "wangcy"
path = "new_work_space3"
def ReadSh(path):
    line = linecache.getline(path,3)
    listitem = line.split("=")
    print listitem
    return listitem[1].strip()
def ReadXmlVersion(path):
       print 'sopconfig path:='+path
       dom = xml.dom.minidom.parse(path)
       root = dom.documentElement
       #itemlist = root.getElementsByTagName('application')
       #print itemlist
       #item = itemlist[0]
       ver=root.getAttribute("syberos:versionName")
       print 'version:'+ver
       return ver

##################app complie
def build_phonebook():
	os.system('export CHROOT=/home/'+user+'/SyberOS-SDK/jail/kchroot && \
		   export PATH=$PATH:/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/lib/qt5/bin:/usr/bin && \
		   export QTDIR=/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/share/qt5 && \
                   rm -rf /home/'+user+'/sopbuild/build-phonebook-Device-Debug && \
                   mkdir /home/'+user+'/sopbuild/build-phonebook-Device-Debug &&\
                   cd /home/'+user+'/sopbuild/build-phonebook-Device-Debug &&\
	           /home/'+user+'/SyberOS-SDK/jail/kchroot qmake /home/'+user+'/'+path+'/app/phonebook/phonebook.pro -r -spec linux-g++-armv7l CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot make && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot buildpkg /home/'+user+'/'+path+'/app/phonebook/phonebook.pro')
	#'/usr/bin/ins-tool -siu com.sinux.phonebook-1.0.8.sop && rm -f /tmp/com.sinux.phonebook-1.0.8.sop'
	#/home/wangcy/SyberOS-SDK/jail/kchroot make clean in /home/wangcy/'+path+'/app/build-phonebook-Device-Debug && \

def install_phonebook_statements():
        ver = ReadXmlVersion('/home/'+user+'/'+path+'/app/phonebook/sopconfig.xml')
        str = 'scp /home/'+user+'/sopbuild/build-phonebook-Device-Debug/com.sinux.phonebook-'+ver+'.sop developer@192.168.100.100:~'
        print str
	os.system(str)
        statements = '/usr/bin/ins-tool -siu com.sinux.phonebook-'+ver+'.sop'
        print statements
        return statements

def build_LaserRange():
	os.system('export CHROOT=/home/'+user+'/SyberOS-SDK/jail/kchroot && \
		   export PATH=$PATH:/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/lib/qt5/bin:/usr/bin && \
		   export QTDIR=/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/share/qt5 && \
                   rm -rf /home/'+user+'/sopbuild/build-LaserRange-Device-Debug && \
                   mkdir /home/'+user+'/sopbuild/build-LaserRange-Device-Debug &&\
                   cd /home/'+user+'/sopbuild/build-LaserRange-Device-Debug &&\
	           /home/'+user+'/SyberOS-SDK/jail/kchroot qmake /home/'+user+'/'+path+'/app/LaserRange/LaserRange.pro -r -spec linux-g++-armv7l CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot make && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot buildpkg /home/'+user+'/'+path+'/app/LaserRange/LaserRange.pro')

def install_laserrange_statements():
        ver = ReadXmlVersion('/home/'+user+'/'+path+'/app/LaserRange/sopconfig.xml')
        str = 'scp /home/'+user+'/sopbuild/build-LaserRange-Device-Debug/com.sinux.laserrange-'+ver+'.sop developer@192.168.100.100:~'
        print str
	os.system(str)
        statements = '/usr/bin/ins-tool -siu com.sinux.laserrange-'+ver+'.sop'
        print statements
        return statements

def build_PhoneCall():
	os.system('export CHROOT=/home/'+user+'/SyberOS-SDK/jail/kchroot && \
		   export PATH=$PATH:/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/lib/qt5/bin:/usr/bin && \
		   export QTDIR=/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/share/qt5 && \
                   rm -rf /home/'+user+'/sopbuild/build-PhoneCall-Device-Debug && \
                   mkdir /home/'+user+'/sopbuild/build-PhoneCall-Device-Debug &&\
                   cd /home/'+user+'/sopbuild/build-PhoneCall-Device-Debug &&\
	           /home/'+user+'/SyberOS-SDK/jail/kchroot qmake /home/'+user+'/'+path+'/app/PhoneCall/PhoneCall.pro -r -spec linux-g++-armv7l CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot make && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot buildpkg /home/'+user+'/'+path+'/app/PhoneCall/PhoneCall.pro')
def install_phonecall_statements():
        ver = ReadXmlVersion('/home/'+user+'/'+path+'/app/PhoneCall/sopconfig.xml')
        str = 'scp /home/'+user+'/sopbuild/build-PhoneCall-Device-Debug/com.sinux.PhoneCall-'+ver+'.sop developer@192.168.100.100:~'
        print str
	os.system(str)
        statements = '/usr/bin/ins-tool -siu com.sinux.PhoneCall-'+ver+'.sop'
        print statements
        return statements

def build_Weather():
	os.system('export CHROOT=/home/'+user+'/SyberOS-SDK/jail/kchroot && \
		   export PATH=$PATH:/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/lib/qt5/bin:/usr/bin && \
		   export QTDIR=/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/share/qt5 && \
                   rm -rf /home/'+user+'/sopbuild/build-weather-Device-Debug && \
                   mkdir /home/'+user+'/sopbuild/build-weather-Device-Debug &&\
                   cd /home/'+user+'/sopbuild/build-weather-Device-Debug &&\
	           /home/'+user+'/SyberOS-SDK/jail/kchroot qmake /home/'+user+'/'+path+'/app/weather/weather.pro -r -spec linux-g++-armv7l CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot make && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot buildpkg /home/'+user+'/'+path+'/app/weather/weather.pro')

def install_weather_statements():
        ver = ReadXmlVersion('/home/'+user+'/'+path+'/app/weather/sopconfig.xml')
        str = 'scp /home/'+user+'/sopbuild/build-weather-Device-Debug/com.sinux.weather-'+ver+'.sop developer@192.168.100.100:~'
        print str
	os.system(str)
        statements = '/usr/bin/ins-tool -siu com.sinux.weather-'+ver+'.sop'
        print statements
        return statements

def build_Battlefield():
	os.system('export CHROOT=/home/'+user+'/SyberOS-SDK/jail/kchroot && \
		   export PATH=$PATH:/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/lib/qt5/bin:/usr/bin && \
		   export QTDIR=/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/share/qt5 && \
                   rm -rf /home/'+user+'/sopbuild/build-Battlefield-Device-Debug && \
                   mkdir /home/'+user+'/sopbuild/build-Battlefield-Device-Debug &&\
                   cd /home/'+user+'/sopbuild/build-Battlefield-Device-Debug &&\
	           /home/'+user+'/SyberOS-SDK/jail/kchroot qmake /home/'+user+'/'+path+'/app/Battlefield/Battlefield.pro -r -spec linux-g++-armv7l CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot make && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot buildpkg /home/'+user+'/'+path+'/app/Battlefield/Battlefield.pro')

def install_Battlefield_statements():
        ver = ReadXmlVersion('/home/'+user+'/'+path+'/app/Battlefield/sopconfig.xml')
        str = 'scp /home/'+user+'/sopbuild/build-Battlefield-Device-Debug/com.sinux.Battlefield-'+ver+'.sop developer@192.168.100.100:~'
        print str
	os.system(str)
        statements = '/usr/bin/ins-tool -siu com.sinux.Battlefield-'+ver+'.sop'
        print statements
        return statements

def build_MultiFunction():
	os.system('export CHROOT=/home/'+user+'/SyberOS-SDK/jail/kchroot && \
		   export PATH=$PATH:/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/lib/qt5/bin:/usr/bin && \
		   export QTDIR=/home/'+user+'/SyberOS-SDK/SDKQt/qt530/usr/share/qt5 && \
                   rm -rf /home/'+user+'/sopbuild/build-MultiFunction-Device-Debug && \
                   mkdir /home/'+user+'/sopbuild/build-MultiFunction-Device-Debug &&\
                   cd /home/'+user+'/sopbuild/build-MultiFunction-Device-Debug &&\
	           /home/'+user+'/SyberOS-SDK/jail/kchroot qmake /home/'+user+'/'+path+'/app/MultiFunction/MultiFunction.pro -r -spec linux-g++-armv7l CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot make && \
		   /home/'+user+'/SyberOS-SDK/jail/kchroot buildpkg /home/'+user+'/'+path+'/app/MultiFunction/MultiFunction.pro')

def install_MultiFunction_statements():
        ver = ReadXmlVersion('/home/'+user+'/'+path+'/app/MultiFunction/sopconfig.xml')
        str = 'scp /home/'+user+'/sopbuild/build-MultiFunction-Device-Debug/com.sinux.MultiFunction-'+ver+'.sop  developer@192.168.100.100:~'
        print str
	os.system(str)
        statements = '/usr/bin/ins-tool -siu com.sinux.MultiFunction-'+ver+'.sop'
        print statements
        return statements

#########service complie
def build_adhoc():
	os.system('cd ../service && \
	 	./build-voip-ad-hoc-service.sh')
	time.sleep(1)
def install_adh_statements():
        ver = ReadSh('/home/'+user+'/'+path+'/service/build-voip-ad-hoc-service.sh')
        str = 'scp /home/'+user+'/rpmbuild/RPMS/armv7tnhl/voipAdHocService-'+ver+'-1.armv7tnhl.rpm developer@192.168.100.100:~'
        print str
	os.system('scp /home/'+user+'/rpmbuild/RPMS/armv7tnhl/voipAdHocService-'+ver+'-1.armv7tnhl.rpm developer@192.168.100.100:~')
        statements = 'rpm -Uvh --force voipAdHocService-'+ver+'-1.armv7tnhl.rpm --nodeps'
        return statements

def build_lte():
	os.system('cd ../service/ && \
	 	./build-voip-lte-service.sh')
	time.sleep(1)

def install_lte_statements():
        ver = ReadSh('/home/'+user+'/'+path+'/service/build-voip-lte-service.sh')
        str = 'scp /home/'+user+'/rpmbuild/RPMS/armv7tnhl/voipLteService-'+ver+'-1.armv7tnhl.rpm developer@192.168.100.100:~'
        print str
	os.system('scp /home/'+user+'/rpmbuild/RPMS/armv7tnhl/voipLteService-'+ver+'-1.armv7tnhl.rpm developer@192.168.100.100:~')
        statements = 'rpm -Uvh --force voipLteService-'+ver+'-1.armv7tnhl.rpm --nodeps'
        return statements

def build_voipWifi():
	os.system('cd ../service/ && \
	 	./build-voip-wifi-service.sh')
	time.sleep(1)

def install_voipWifi_statements():
        ver = ReadSh('/home/'+user+'/'+path+'/service/build-voip-wifi-service.sh')
        str = 'scp /home/'+user+'/rpmbuild/RPMS/armv7tnhl/voipWifiService-'+ver+'-1.armv7tnhl.rpm developer@192.168.100.100:~'
        print str
	os.system('scp /home/'+user+'/rpmbuild/RPMS/armv7tnhl/voipWifiService-'+ver+'-1.armv7tnhl.rpm developer@192.168.100.100:~')
        statements = 'rpm -Uvh --force voipWifiService-'+ver+'-1.armv7tnhl.rpm --nodeps'
        return statements
        
def build_Watch():
	os.system('cd ../service/ && \
	 	./build-watchLauncherservice.sh')
	time.sleep(1)

def install_Watch_statements():
        ver = ReadSh('/home/'+user+'/'+path+'/service/build-watchLauncherservice.sh')
        str = 'scp /home/'+user+'/rpmbuild/RPMS/armv7tnhl/watchLauncherService-'+ver+'-1.armv7tnhl.rpm developer@192.168.100.100:~'
        print str
	os.system('scp /home/'+user+'/rpmbuild/RPMS/armv7tnhl/watchLauncherService-'+ver+'-1.armv7tnhl.rpm developer@192.168.100.100:~')
        statements = 'rpm -Uvh --force watchLauncherService-'+ver+'-1.armv7tnhl.rpm --nodeps'
        return statements

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
			cp -rf /home/wangcy/rpmbuild/RPMS/armv7tnhl/*.rpm ./allbuilds && \
			cp -rf ../service/build-dialadaptor-Device-Debug/*.sop ./allbuilds')
	os.system('cp -rf /home/'+user+'/rpmbuild/RPMS/armv7tnhl/*.rpm ./allbuilds')
	os.system('scp ./installvoip developer@192.168.100.100:~')
        os.system('scp ./allbuilds/* developer@192.168.100.100:~')

def login_root_mount():

    ssh = pexpect.spawn("ssh developer@192.168.100.100")

    index = ssh.expect([pexpect.TIMEOUT, 'yes/no', '.ssword:*', 'ssh-keygen -f "./home/wangcy/.ssh/known_hosts"'], 2)
    if index == 0:
        print "timeout............"
    elif index == 1:
        ssh.sendline("yes")
    elif index == 2:
        ssh.sendline("system")
    elif index == 3:
        ssh.sendline('ssh-keygen -f "/home/wangcy/.ssh/known_hosts"')

    ssh.sendline("su")

    if ssh.expect('Password:') == 0:
        ssh.sendline('system')

    ssh.sendline("pwd")

    if ssh.expect('$') == 0:
        ssh.sendline("mount -o remount,rw /")

    #ssh.sendline("ps -ef | grep voip")
    #ssh.send("\r\r\r")
    #ssh.sendline("ps -ef | grep Co")
    #ssh.send("\r\r\r")
    #ssh.sendline("journalctl -af | grep voipAd")
    #ssh.send("\r\r\r\r\r\r")

    ssh.interact()


def login_root_mount_install(statememts):

    ssh = pexpect.spawn("ssh developer@192.168.100.100")

    index = ssh.expect([pexpect.TIMEOUT, 'yes/no', '.ssword:*', 'ssh-keygen -f "./home/wangcy/.ssh/known_hosts"'], 2)
    if index == 0:
        print "timeout............"
    elif index == 1:
        ssh.sendline("yes")
    elif index == 2:
        ssh.sendline("system")
    elif index == 3:
        ssh.sendline('ssh-keygen -f "/home/wangcy/.ssh/known_hosts"')

    ssh.sendline("su")

    if ssh.expect('Password:') == 0:
        ssh.sendline('system')

    ssh.sendline("pwd")

    if ssh.expect('$') == 0:
        ssh.sendline("mount -o remount,rw /")

    ssh.sendline(statememts)

    #ssh.send("\r\r\r\r\r\r")

    ssh.interact()


def cp_login_root_mount():
	os.system('scp /home/wangcy/rpmbuild/RPMS/armv7tnhl/voipAdHocService-1.0.8-1.armv7tnhl.rpm developer@192.168.100.100:~')
	os.system('scp /home/wangcy/rpmbuild/RPMS/armv7tnhl/voipLteService-1.0.8-1.armv7tnhl.rpm developer@192.168.100.100:~')
	login_root_mount()

def reboot():
	ssh = pexpect.spawn("ssh developer@192.168.100.100")
	index = ssh.expect([pexpect.TIMEOUT, 'yes/no', '.ssword:*', 'ssh-keygen -f "./home/wangcy/.ssh/known_hosts"'], 2)
	if index == 0:
		print "timeout............"
	elif index == 1:
		ssh.sendline("yes")
	elif index == 2:
		ssh.sendline("system")
	elif index == 3:
		ssh.sendline('ssh-keygen -f "/home/wangcy/.ssh/known_hosts"')
	ssh.sendline("su")
	if ssh.expect('Password:') == 0:
		ssh.sendline('system')
	ssh.sendline("pwd")
	if ssh.expect('$') == 0:
		ssh.sendline("mount -o remount,rw /")
	ssh.sendline("toolbox reboot")
	ssh.interact()

def build_all():
        build_phonebook()#app
        build_LaserRange()
        build_PhoneCall()
        build_Weather()
        build_MultiFunction()
        build_Battlefield()
        build_adhoc()#service
        build_lte()
        build_Watch()
def install_all():
        statementslte = install_lte_statements()
        statementsadh = install_adh_statements()
        statementwatch = install_Watch_statements()

        statementphonebook = install_phonebook_statements()
        statementphonecallbook = install_phonecall_statements()
        statementweather = install_weather_statements()
        statementlaserrange = install_laserrange_statements()
        statementBattlefield = install_Battlefield_statements()
        statementMultiFunction = install_MultiFunction_statements()

        ssh = pexpect.spawn("ssh developer@192.168.100.100")
        index = ssh.expect([pexpect.TIMEOUT, 'yes/no', '.ssword:*', 'ssh-keygen -f "./home/wangcy/.ssh/known_hosts"'], 2)
        if index == 0:
             print "timeout............"
        elif index == 1:
             ssh.sendline("yes")
        elif index == 2:
             ssh.sendline("system")
        elif index == 3:
             ssh.sendline('ssh-keygen -f "/home/wangcy/.ssh/known_hosts"')
        ssh.sendline("su")
        if ssh.expect('Password:') == 0:
             ssh.sendline('system')
        ssh.sendline("pwd")
        if ssh.expect('$') == 0:
             ssh.sendline("mount -o remount,rw /")
        ssh.sendline(statementslte)
        time.sleep(10)
        ssh.sendline(statementsadh)
        time.sleep(10)
        ssh.sendline(statementwatch)
        
        ssh.sendline("su developer")
        time.sleep(10)
        ssh.sendline(statementphonebook)
        time.sleep(10)
        ssh.sendline(statementphonecall)
        time.sleep(10)
        ssh.sendline(statementweather)
        time.sleep(10)
        ssh.sendline(statementlaserrange)
        time.sleep(10)
        ssh.sendline(statementBattlefield)
        time.sleep(10)
        ssh.sendline(statementMultiFunction)
    #ssh.sendline("ps -ef | grep voip")
    #ssh.send("\r\r\r")
    #ssh.sendline("ps -ef | grep Co")
    #ssh.send("\r\r\r")
    #ssh.sendline("journalctl -af | grep voipAd")
    #ssh.send("\r\r\r\r\r\r")
        ssh.interact()
def build_test():
        #ReadXmlVersion('/home/wangcy/'+path+'/app/LaserRange/sopconfig.xml')
        #ReadSpec('/home/wangcy/'+path+'/service/build-voip-ad-hoc-service.sh')
        install_phonebook_statements()
        install_phonecall_statements()
        install_weather_statements()
        install_laserrange_statements()
if __name__ == '__main__':
    
	if len(sys.argv) <= 1:
                print ''
                print 'NAME BUILD'
                print ''
                print 'OPERATE TYPE:'
                print ''
                print '	login --1.login remote phone  2.su to root  3.mount -o remount rw /'
                print ''
                print '	logininstall --execute login and install'
                print ''
                print '	cplogin'
                print ''
                print '	reboot --reboot remote phone '
                print ''
                print '	cpall --copy all package to remote phone'
                print ''
                print 'APP TARGET:'
                print ''
                print '	phb --compile phonebook app target'
                print ''
                print '	lar --compile laserrange app target '
                print ''
                print '	phc --compile phonecall app target'
                print ''
                print '	wea --compile weather app target'
                print ''
                print 'SERVICE TARGET:'
                print ''
                print '	adh --compile voipAdHocService'
                print ''
                print '	lte --compile voipLteService'
                print ''
                print '	all --compile all target app and service'
                print ''
                print '	insall --install all target app and service'  
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
	elif orderStr == 'adh' :
		build_adhoc()
	elif orderStr == 'lte':
		build_lte()
	elif orderStr == 'pjsip':
		build_pjsip()
	elif orderStr == 'cpall':
		cp_all()
	elif orderStr == 'reboot':
		reboot()
	elif orderStr == 'phb':
		build_phonebook()
        elif orderStr == 'lar':
                build_LaserRange()
        elif orderStr == 'phc':
                build_PhoneCall()
        elif orderStr == 'wea':
                build_Weather()
        elif orderStr == 'bat':
                build_Battlefield()
        elif orderStr == 'mut':
                build_MultiFunction()
        elif orderStr == 'wat':
                build_Watch()
        elif orderStr == 'wif':
                build_voipWifi()
        elif orderStr == 'test':
                build_test()
        elif orderStr == 'all':
                build_all()
        elif orderStr == 'insall':
                install_all()
        elif orderStr == 'test':
                build_test()
