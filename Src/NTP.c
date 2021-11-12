#include "SDK30.H"
#include "IP_.h"

#include "HTTP.h"

int NTPDate()
{
	char *hostname = "200.160.0.8"; 	//"163.117.202.33";
	int portno = 123;
	int maxlen = 1024;
	int GMT = -3;  			//GMT -3 = Fuso horário de Brasilia

	int i,s;
	unsigned char msg[48]={010,0,0,0,0,0,0,0,0};
	unsigned long  buf[maxlen];
	struct sockaddr_in server_addr;
	struct sockaddr saddr;
	long tmit;
	DATE dt;
	int horalocal;
	int ret = FALSE;

	s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	memset( &server_addr, 0, sizeof( server_addr ));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = __inet_addr(hostname);
	server_addr.sin_port = htons(portno);

	/*
	 * build a message.  Our message is all zeros except for a one in the
	 * protocol version field
	 * msg[] in binary is 00 001 000 00000000
	 * it should be a total of 48 bytes long
	*/

	// send the data
	i = sendto(s,msg,sizeof(msg),0,(struct sockaddr *)&server_addr,sizeof(server_addr));

	// get the data back
	i = recvfrom(s, buf, 48, MSG_WAITALL, &saddr, (int *)sizeof(saddr));

	//We get 12 long words back in Network order
	/*
	for(i=0;i<12;i++)
	    printf("%d\t%-8x\n",i,ntohl(buf[i]));
	*/

	/*
	 * The high word of transmit time is the 10th word we get back
	 * tmit is the time in seconds not accounting for network delays which
	 * should be way less than a second if this is a local NTP server
	 */

	tmit = ntohl(buf[10]);    //# get transmit time

	/*
	 * Convert time to unix standard time NTP is number of seconds since 0000
	 * UT on 1 January 1900 unix time is seconds since 0000 UT on 1 January
	 * 1970 There has been a trend to add a 2 leap seconds every 3 years.
	 * Leap seconds are only an issue the last second of the month in June and
	 * December if you don't try to set the clock then it can be ignored but
	 * this is importaint to people who coordinate times with GPS clock sources.
	 */

	tmit -= 2208988800U;

	longto_d(tmit, &dt);

	char strhoralocal[] = { dt.hour[0], dt.hour[1], '\0' };
	horalocal = atoi(strhoralocal);

	horalocal += GMT;
	if (horalocal < 0) horalocal = 24 - horalocal;

	sprintf(strhoralocal, "%02d", horalocal);

	dt.hour[0] = strhoralocal[0];
	dt.hour[1] = strhoralocal[1];

	if (SV_write_date(&dt) == 0)
		ret = TRUE;

	return ret;
}
