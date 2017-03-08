%define debug_packages %{nil}
%define debug_package %{nil}
%define _missing_doc_files_terminate_build 0
Name:		watchLauncherService
Version:	1.0.10
Release:	1
Summary:	blacklist

Group:		Applicaton/Communication
License:	BSD
URL:		www.syberos.com
Source0:	%{name}-%{version}.tar.gz		
BuildRoot:	%{_tmppath}/%{name}-root
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(syberos-qt-system) 
Requires: syberos-qt



%description
test servce start by systemd and dbus

%prep
%setup -q -n %{name}-%{version}/%{name}

%build
export QT_SELECT=5
qmake
make

%install
make INSTALL_ROOT=$RPM_BUILD_ROOT install
mkdir -p %{buildroot}/usr/lib/systemd/user/user-session.target.wants
mkdir -p %{buildroot}/etc/dbus-1/system.d
ln -s ../watchLauncherService.service  %{buildroot}/usr/lib/systemd/user/user-session.target.wants/watchLauncherService.service

%clean

%files
%defattr(-,service,systemapps,-)
%{_bindir}/watchLauncherService
#%{_datadir}/dbus-1/services/*.service
/usr/lib/systemd/user/*.service
/usr/lib/systemd/user/user-session.target.wants/*.service
#/etc/dbus-1/system.d/blacklistd.conf
