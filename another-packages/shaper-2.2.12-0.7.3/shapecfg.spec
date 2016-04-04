Summary: A configuration tool for setting traffic bandwidth parameters.
Name: shapecfg
Version: 2.2.12
Release: 5
Copyright: GPL
Group: System Environment/Base
Source0: shaper.36.tar.gz
Source1: README.shaper
Source2: cbq.sh
Source3: README.cbq
Patch: shapercfg-2.0.36-glibc.patch
Buildroot: %{_tmppath}/shaper-%{version}-root
Requires: kernel >= 2.0.36 , iproute
ExclusiveArch: i386

%description
The Shapecfg program configures and adjusts traffic shaper bandwidth
limiters. Traffic shaping means setting parameters or limit to which
network traffic should conform--setting limitations on bandwidth
consumption.

To use Shapecfg, you must have also installed the kernel which
supports the shaper module (kernel versions 2.0.36 or later and late
2.1.x kernels).

Install the shapecfg package if you want to set traffic bandwidth
parameters, and if you have the appropriate kernel.

%prep
%setup -q -n shaper
%patch0 -p1 -b .glibc

%build
make CFLAGS="$RPM_OPT_FLAGS"

%install
mkdir -p $RPM_BUILD_ROOT/sbin
install -s -m 755 shapecfg $RPM_BUILD_ROOT/sbin/shapecfg
install -m 755 %{SOURCE2} $RPM_BUILD_ROOT/sbin/cbq
install -d -m 755 $RPM_BUILD_ROOT/etc/sysconfig/cbq
cat <<EOF > $RPM_BUILD_ROOT/etc/sysconfig/cbq/cbq-0000.example
DEVICE=eth0,10Mbit,1Mbit
RATE=128Kbit
WEIGHT=10Kbit
PRIO=5
RULE=192.168.1.0/24
EOF

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc $RPM_SOURCE_DIR/README.shaper $RPM_SOURCE_DIR/README.cbq
/sbin/shapecfg
/sbin/cbq
%dir /etc/sysconfig/cbq
%config /etc/sysconfig/cbq/*

%changelog
* Wed Jul 12 2000 Prospector <bugzilla@redhat.com>
- automatic rebuild

* Mon Jul 10 2000 Trond Eivind Glomsrød <teg@redhat.com>
- added requirement for iproute (bug #6595)
- use %%{_tmppath}

* Sun Jun 18 2000 Matt Wilson <msw@redhat.com>
- rebuilt for next release

* Tue Sep 21 1999 Cristian Gafton <gafton@redhat.com>
- add a sample config for the cbq script

* Thu Sep 09 1999 Cristian Gafton <gafton@redhat.com>
- add the cbq script

* Fri Mar 26 1999 Michael Maher <mike@redhat.com>
- added some documentation.

* Sun Mar 21 1999 Cristian Gafton <gafton@redhat.com> 
- auto rebuild in the new build environment (release 3)

* Wed Feb  3 1999 Bill Nottingham <notting@redhat.com>
- build against glibc2.1

* Fri Oct 02 1998 Cristian Gafton <gafton@redhat.com>
- exclusive arch i386 for now
