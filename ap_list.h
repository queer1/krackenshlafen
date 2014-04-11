#define MAC_PRINT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_ADDR(mac) mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] 

struct AP_struct
{
	char SSID[32];
	unsigned char BSSID[6];
	unsigned char clients[100][6];
	int clients_count;
};

enum ap_list_return {
	CLIENT_ADDED,
	CLIENT_ALREADY_EXISTS
};
typedef struct AP_struct AP_struct;

void ap_list_add_SSID(const unsigned char * BSSID, unsigned char* SSID, int len);
void ap_list_init();
void ap_list_print();
enum ap_list_return ap_list_add_client( AP_struct *ap,const unsigned char* mac);
void ap_list_add_AP(const unsigned char * BSSID,const unsigned char* client);

