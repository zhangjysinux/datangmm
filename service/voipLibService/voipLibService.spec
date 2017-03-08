%define debug_packages %{nil}
%define debug_package %{nil}
%define _missing_doc_files_terminate_build 0
Name:		voipLibService
Version:	2.0.4
Release:	1
Summary:	blacklist

Group:		Applicaton/Communication
License:	BSD
URL:		www.sinux.com
Source0:	%{name}-%{version}.tar.gz		
BuildRoot:	%{_tmppath}/%{name}-root
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)


%description
voip lib servce start by systemd and dbus

%prep
%setup -q -n %{name}-%{version}/%{name}

%build
export QT_SELECT=5
qmake
make

%install
make INSTALL_ROOT=$RPM_BUILD_ROOT install
mkdir -p %{RPM_BUILD_ROOT}/usr/lib/
cp -Raf  ./videolib/*  ${RPM_BUILD_ROOT}/usr/lib/
mkdir -p %{buildroot}/usr/lib/systemd/user/user-session.target.wants
mkdir -p %{buildroot}/etc/dbus-1/system.d
ln -s ../voipLibService.service  %{buildroot}/usr/lib/systemd/user/user-session.target.wants/voipLibService.service

%clean

%files
%defattr(-,system,system,-)
%{_bindir}/voipLibService
#%{_datadir}/dbus-1/services/*.service
/usr/lib/systemd/user/*.service
/usr/lib/systemd/user/user-session.target.wants/*.service
/usr/lib
#/etc/dbus-1/system.d/blacklistd.conf
