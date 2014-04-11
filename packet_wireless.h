#include <stdint.h>

struct frame_control {
uint8_t protocol:2;
uint8_t type:2;
uint8_t sub_type:4;
uint8_t to_ds:1;
uint8_t from_ds:1;
uint8_t more_flag:1;
uint8_t retry:1;
uint8_t power_management:1;
uint8_t more_Data:1;
uint8_t protected_frame:1;
uint8_t order:1;
};

struct wireless_header {
struct frame_control fc;
uint16_t duration;
uint8_t address1[6];
uint8_t address2[6];
uint8_t address3[6];
uint16_t seqence_number;
uint8_t address4[6];
uint16_t qos_control;
void *frame_body;
};

struct radiotap_header {
        u_int8_t        it_version;     /* set to 0 */
        u_int8_t        it_pad;
        u_int16_t       it_len;         /* entire length */
        u_int32_t       it_present;     /* fields present */
} __attribute__((__packed__));


struct tagged_parameter
{ 
	uint8_t number;
	uint8_t len;
};

struct beacon_management_frame
{
	unsigned char timestamp[8];
	uint16_t beacon_interval;
	uint16_t capability;
};  


enum fc_type
{
	MANAGMENT,
	CONTROL,
	DATA
};
 
enum fc_sub_type
{
	BEACON = 8,
	PROBE_RESPONCE = 5
};

