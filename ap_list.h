#define MAC_PRINT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_ADDR(mac) mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] 

struct AP_struct
{
	char SSID[32];
	unsigned char BSSID[6];
	unsigned char clients[100][6];
	int clients_count;
};
typedef struct AP_struct AP_struct;

void ap_list_add_SSID(const unsigned char * BSSID, unsigned char* SSID, int len);
void ap_init_list();
void print_ap_list();
void ap_add_client( AP_struct *ap,const unsigned char* mac);
void add_ap_list(const unsigned char * BSSID,const unsigned char* client);
void print_ap_list();

