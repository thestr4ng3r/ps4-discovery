/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
