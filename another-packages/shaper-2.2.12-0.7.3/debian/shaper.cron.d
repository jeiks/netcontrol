# This is needed in order for the TIME parameter in /etc/shaper/cbq-* to
# work.
*/10 * * * *	root	[ -d /etc/shaper ] && /etc/init.d/shaper timecheck
