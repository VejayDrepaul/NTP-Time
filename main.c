#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define NTP_SERVER "216.239.35.0" // time.google.com
#define NTP_PORT 123
#define NTP_PACKET_SIZE 48
#define NTP_UNIX_EPOCH_DIFF 2208988800UL
typedef struct {
    uint8_t li_vn_mode; // stores leap indicator, version number, mode
    uint8_t stratum;
    uint8_t poll_interval;
    uint8_t precision; 
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t ref_id; // refrence identifier
	uint32_t ref_time_sec;
	uint32_t ref_time_frac;
	uint32_t origin_time_sec;
	uint32_t origin_time_frac;
	uint32_t recv_time_sec;
	uint32_t recv_time_frac;
	uint32_t trans_time_sec;
	uint32_t trans_time_frac;
} ntp_info; 

int main(int argc, char *argv[]) {
	ntp_info ntp_packet;
	time_t ntp_time;
	struct sockaddr_in server_addr;
	
	int sock_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock_desc < 0) {
		perror("Failed to create UDP socket!\n");
		return -1;
	}

	// filling server information 
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;  //IPv4
	server_addr.sin_port = htons(NTP_PORT);
	if (inet_pton(AF_INET, NTP_SERVER, &(server_addr.sin_addr)) <= 0) {
		perror("Invalid server address!\n");
		close(sock_desc);
		return -1;
	}

	// preperation of ntp_packet
	memset(&ntp_packet, 0, sizeof(ntp_packet));
	ntp_packet.li_vn_mode = 0x1B;

	// send ntp packet to server
	if (sendto(sock_desc, &ntp_packet, sizeof(ntp_packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Sending info to server failed!\n");
		close(sock_desc);
		return -1;
	}

	// receive ntp packet from server
	if (recv(sock_desc, &ntp_packet, sizeof(ntp_packet), 0) < 0) {
		perror("Failed to recieve NTP packet!\n");
		close(sock_desc);
		return -1;
	}

	ntp_time = ntohl(ntp_packet.trans_time_sec) - NTP_UNIX_EPOCH_DIFF; 

	printf("%s", ctime(&ntp_time));

	close(sock_desc);

	return 0;
}
