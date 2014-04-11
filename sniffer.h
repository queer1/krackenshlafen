

void sniffer_init(char* dev);
const char * sniffer_get_next_packet(int *len);
void sniffer_close();
struct radiotap_header *sniffer_get_rt_header(packet);



