
build:
	gcc -Wall -Wpointer-arith -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wcomments -Werror -D_REENTRANT -m32 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -g -O2 -fPIE -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security -Wall -g -Wall -O2 -m32 -fPIE -pie -Wl,-z -Wl,relro -Wl,-z -Wl,now  -lldap -llber -lm -lnsl -lcap -ldl squid_ldap_auth_and_ip.c -o squid_ldap_auth_and_ip

clean:
	rm -f squid_ldap_auth_and_ip

install: build
	install -D -m755 squid_ldap_auth_and_ip $(DESTDIR)/usr/share/netcontrol-controle-web-ip/squid_ldap_auth_and_ip
