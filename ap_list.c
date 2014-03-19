#include <stdio.h>
#include "ap_list.h"
#include <string.h>


AP_struct ap_list[100] = {0};
int ap_list_count = 0;

void ap_init_list()
{
	ap_list_count = 0;
}

void ap_add_client( AP_struct *ap,const unsigned char* mac)
{
	int i = 0;
	for (i=0; i< ap->clients_count;i++)
	{
		if (memcmp(mac, ap->clients[i], 6*sizeof(unsigned char)) == 0)
		{
			return;
		}
	}
	
	memcpy(ap->clients[ap->clients_count], mac, 6*sizeof(unsigned char));
	ap->clients_count++;
	return;
}

void add_ap_list(const unsigned char * BSSID, const unsigned char* client)
{
	int i = 0;
	for (i=0; i< ap_list_count;i++)
	{
		if (memcmp(BSSID, ap_list[i].BSSID, 6*sizeof(unsigned char)) == 0)
		{
			ap_add_client(&ap_list[i], client);
			return;
		}
	}
	memcpy(ap_list[ap_list_count].BSSID, BSSID, 6*sizeof(unsigned char));
	ap_list[ap_list_count].clients_count = 0;
	ap_add_client(&ap_list[ap_list_count], client);
	ap_list_count++;
	print_ap_list();
	return;
}

void ap_list_add_SSID(const unsigned char * BSSID, unsigned char* SSID, int len)
{
	int i = 0;
	for (i=0; i< ap_list_count;i++)
	{
		if (memcmp(BSSID, ap_list[i].BSSID, 6*sizeof(unsigned char)) == 0)
		{
			memcpy(&ap_list[i], SSID, len*sizeof(char));
			return;
		}
	}
}

void print_ap_list()
{
	int i=0, j=0;
	printf("-------------------------\n");
	for (i=0; i< ap_list_count;i++)
	{	
		printf("AP: %s "MAC_PRINT"\n", ap_list[i].SSID, MAC_ADDR(ap_list[i].BSSID));
		for(j=0;j<ap_list[i].clients_count;j++)
		{
			printf("\t CLIENT: "MAC_PRINT"\n", MAC_ADDR(ap_list[i].clients[j]));
		}
	}
	printf("-------------------------\n");
}

