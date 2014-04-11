#include <pcap.h>
#include <stdio.h>
#include "ap_list.h"
#include "sniffer.h"
#include "packet_wireless.h"

void sniff_map_and_print()
{
//	struct pcap_pkthdr header;
	const unsigned char *packet = NULL;
	struct radiotap_header *rt_header = NULL;
	struct wireless_header *frame_header = NULL;
	struct beacon_management_frame *beacon_frame = NULL;
	struct tagged_parameter *tagged_param = NULL;
	

	int i=0;
	int len = 0;
	/* Initialize network interface */
	while (1)//i++ < 900000)
	{
		packet = sniffer_get_next_packet(&len);

		/*	printf("FR ROUTER BSSID: "MAC_PRINT" SOURCE: "MAC_PRINT" DEST: "MAC_PRINT" SUB TYPE: %d \n", 
		  MAC_ADDR(frame_header->addr3), MAC_ADDR(frame_header->addr2), MAC_ADDR(frame_header->addr1), 
		frame_header->fc.sub_type); */

		if(packet != NULL)
		{
			rt_header = (struct radiotap_header *) sniffer_get_rt_header(packet);
			frame_header = (struct wireless_header *) (packet + rt_header->it_len);
			// (frame_header->fc.type == MANAGMENT || frame_header->fc.type == DATA ) && 

			if (frame_header->fc.to_ds == 1 &&  frame_header->fc.from_ds == 0)
			{
				ap_list_add_AP(frame_header->address1, frame_header->address2);
			}
			if (frame_header->fc.to_ds == 0  && frame_header->fc.from_ds == 1)
			{
				ap_list_add_AP(frame_header->address2, frame_header->address3);
			}
			if (frame_header->fc.to_ds == 0  && frame_header->fc.from_ds == 0)
			{
				//ap_list_add_AP(frame_header->addr3, frame_header->addr2);
			}


			if (  frame_header->fc.sub_type == BEACON && frame_header->fc.type == MANAGMENT)
			{
				unsigned char* SSID = NULL;
				unsigned short SSID_len = 0;
				//this is ugly, i'll clean it up, i promise
				tagged_param = (struct tagged_parameter *) (packet + rt_header->it_len+
										+24 +	sizeof(struct beacon_management_frame) );
				if (tagged_param->number == 0)
				{
					SSID = (unsigned char*) (tagged_param+ 1);
					ap_list_add_AP(frame_header->address3, NULL);
					ap_list_add_SSID(frame_header->address3, SSID, tagged_param->len);
					//printf("alabala %d\n", tagged_param->len);
				}
			}
		}
	}
}

int main(int argc, char **argv)
{


	/* Sniff and map the topology of ap and clients. */
	sniffer_init("mon0");
	ap_list_init();	
	sniff_map_and_print();

	return 0;
}

