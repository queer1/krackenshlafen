
#include "sniffer.h"
#include "pcap.h"

pcap_t *handle;		/* Session handle */
char *dev;		/* The device to sniff on */
char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
struct bpf_program fp;	/* The compiled filter */
char filter_exp[] = "port 23";	/* The filter expression */
bpf_u_int32 mask;	/* Our netmask */
bpf_u_int32 net;	/* Our IP */
struct pcap_pkthdr header;	/* The header that pcap gives us */
const u_char *packet;	/* The actual packet */

void sniffer_init(char* dev)
{
	/* Define the device */
	//dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
		printf("Couldn't find default device: %s\n", errbuf);
		return;
	}
	
	/* Open the session in promiscuous mode */
	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == NULL) {
		printf("Couldn't open device %s: %s\n", dev, errbuf);
		return;
	}
}
void sniffer_close()
{
	if (handle != NULL) {
		pcap_close(handle);
	}
}

const char * sniffer_get_next_packet(int *len)
{

	/* Grab a packet */
	const char* packet_locale = NULL;
	while (1)
	{	packet_locale = pcap_next(handle, &header) ;
		if (packet_locale != NULL)
		{
			break;
		}
	}
	/* Print its length */
	*len = header.len;
	/* And close the session */
	return packet_locale;
}

struct radiotap_header *sniffer_get_rt_header(packet)
{
	if(pcap_datalink(handle) == DLT_IEEE802_11_RADIO)
	{
		return packet;
	}
	else 
	{
		return "\x00\x00\x00\x00\x00\x00\x00\x00";
	}
}
