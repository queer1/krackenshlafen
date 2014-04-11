#include <pcap.h>
#include <stdio.h>
#include "sniffer.h"
#include "packet_wireless.h"


int main(int argc, char *argv[])
{
	const struct wireless_header *wifi_packet;
	const struct radiotap_header *rt_header;
	const unsigned char *packet;

	int len = 0;
	sniffer_init("mon0");
	packet = sniffer_get_next_packet(&len);


	rt_header = sniffer_get_rt_header(packet, len);

	wifi_packet = (packet+rt_header->it_len);
	
	sniffer_close();
	return 0;
}
