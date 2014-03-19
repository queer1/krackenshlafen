#include "wpscrack.h"
#include "ap_list.h"

void printMAC (unsigned char *mac)
{
	printf(MAC_PRINT"\n", MAC_ADDR(mac));
}

void sniff_map_and_print()
{
	struct pcap_pkthdr header;
	const u_char *packet = NULL;
	struct radio_tap_header *rt_header = NULL;
	struct dot11_frame_header *frame_header = NULL;
	struct beacon_management_frame *beacon_frame = NULL;
	struct tagged_parameter *tagged_param = NULL;
	

	if(!get_iface())
	{
		return;
	}
int i=0;
	/* Initialize network interface */
	set_handle(capture_init(get_iface()));
	if(get_handle() != NULL)
	{
		while (1)//i++ < 900000)
		{
			packet = next_packet(&header);

			/*	printf("FR ROUTER BSSID: "MAC_PRINT" SOURCE: "MAC_PRINT" DEST: "MAC_PRINT" SUB TYPE: %d \n", 
			  MAC_ADDR(frame_header->addr3), MAC_ADDR(frame_header->addr2), MAC_ADDR(frame_header->addr1), 
			frame_header->fc.sub_type); */

			if(packet != NULL)
			{
				rt_header = (struct radio_tap_header *) radio_header(packet, header.len);
				frame_header = (struct dot11_frame_header *) (packet + rt_header->len);
				// (frame_header->fc.type == MANAGMENT || frame_header->fc.type == DATA ) && 

				if (frame_header->fc.to_ds == 1 &&  frame_header->fc.from_ds == 0)
				{
					add_ap_list(frame_header->addr1, frame_header->addr2);
				}
				if (frame_header->fc.to_ds == 0  && frame_header->fc.from_ds == 1)
				{
					add_ap_list(frame_header->addr2, frame_header->addr3);
				}
				if (frame_header->fc.to_ds == 0  && frame_header->fc.from_ds == 0)
				{
					//add_ap_list(frame_header->addr3, frame_header->addr2);
				}


				if (  frame_header->fc.sub_type == BEACON && frame_header->fc.type == MANAGMENT)
				{
					unsigned char* SSID = NULL;
					unsigned short SSID_len = 0;

					tagged_param = (struct tagged_parameter *) (packet + rt_header->len+
											+24 +	sizeof(struct beacon_management_frame) );
					if (tagged_param->number == 0)
					{
						SSID = (unsigned char*) (tagged_param+ 1);
						ap_list_add_SSID(frame_header->addr3, SSID, tagged_param->len);
						//printf("alabala %d\n", tagged_param->len);
					}
				}
			}
		}
	}
	print_ap_list();
}

int main(int argc, char **argv)
{
	int ret_val = EXIT_FAILURE;
//	struct wps_data *wps = NULL;

	globule_init();
	sql_init();
	init_default_settings();

	fprintf(stderr, "\nKrakenmapen v%s WiFi Analytics Tool\n", PACKAGE_VERSION);
	fprintf(stderr, "Copyright (c) 2013, Smith Solutions, Lyubomir Dicnchev <lyubomir.dinchev@gmail.com>\n\n");

	/* Process the command line arguments * no args for now /
	if(process_arguments(argc, argv) == EXIT_FAILURE)
	{
		ret_val = usage(argv[0]);
		goto end;
	} */

	/* Double check usage */
	if(!get_iface())
	{
		set_iface("mon0");
	}

	/* If no MAC address was provided, get it ourselves */
	if(memcmp(get_mac(), NULL_MAC, MAC_ADDR_LEN) == 0)
	{
		if(!read_iface_mac())
		{
			fprintf(stderr, "[-] Failed to retrieve a MAC address for interface '%s'!\n", get_iface());
		}
	}

	/* Initialize signal handlers */
	sigint_init();
	sigalrm_init();

	/* Mark the start time */

	/* Sniff and map the topology of ap and clients. */
	ap_init_list();	
	sniff_map_and_print();

	/* Mark the end time */

	globule_deinit();
	return ret_val;
}

