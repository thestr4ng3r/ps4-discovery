
#ifndef PS4_DISCOVERY_COMMON_H
#define PS4_DISCOVERY_COMMON_H

#define PS4_DISCOVERY_PORT 987

#define PS4_DEVICE_DISCOVERY_PROTOCOL_VERSION "00020020"

enum ps4_discovery_cmd { PS4_DISCOVERY_CMD_SRCH };

struct ps4_discovery_packet
{
	enum ps4_discovery_cmd cmd;
	const char *protocol_version;
};

int ps4_discovery_init();
void ps4_discovery_free();

int ps4_discovery_parse_packet(struct ps4_discovery_packet *packet, char *str);
int ps4_discovery_write_packet(struct ps4_discovery_packet *packet, char *buffer, size_t buffer_len);

#endif //PS4_DISCOVERY_COMMON_H
