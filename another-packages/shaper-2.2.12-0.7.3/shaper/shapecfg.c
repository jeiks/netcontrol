#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_shaper.h>
#include <sys/ioctl.h>

void usage(char *name)
{	
	fprintf(stderr,"%s attach <device> <device>\n%s speed <device> <speed>\n",
		name,name);
	exit(1);
}

void main(int argc, char *argv[])
{
	int s=socket(AF_INET, SOCK_DGRAM,0);
	if(s==-1)
	{
		perror("socket");
		exit(1);
	}
	if(argc!=4)
		usage(argv[0]);
	if(strcmp(argv[1],"attach")==0)
	{
		struct ifreq ifr;
		struct shaperconf *shc=(struct shaperconf *)&ifr.ifr_data;
		strcpy(ifr.ifr_name,argv[2]);
		strcpy(shc->ss_name,argv[3]);
		shc->ss_cmd=SHAPER_SET_DEV;
		if(ioctl(s,SIOCDEVPRIVATE,&ifr)==-1)
		{
			perror("shaper");
			exit(1);
		}
		exit(0);
	}
	if(strcmp(argv[1],"speed")==0)
	{
		int v;
		struct ifreq ifr;
		struct shaperconf *shc=(struct shaperconf *)&ifr.ifr_data;
		strcpy(ifr.ifr_name,argv[2]);
		sscanf(argv[3],"%d",&v);
		shc->ss_speed=v;
		shc->ss_cmd=SHAPER_SET_SPEED;
		if(ioctl(s,SIOCDEVPRIVATE,&ifr)==-1)
		{
			perror("shaper");
			exit(1);
		}
		exit(0);
	}
	usage(argv[0]);
}
