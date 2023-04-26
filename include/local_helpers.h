#ifndef _HELPERS_H
#define _HELPERS_H

#include <firmware_version.h>   // definition of firmware version macros
#include <wrapper.h>            // wrapper definitions for functions that already exist in the firmware
#include <structs.h>            // structures that are used by the code in the firmware
#include <helper.h>             // useful helper functions
#include <patcher.h>            // macros used to craete patches such as BLPatch, BPatch, ...
#include <sendframe.h>          // sendframe functionality
#include <udptunnel.h>

/**
 * YOUR CODE HERE
 * This file shoudl contain generic helpers, e.g., the definition if Wi-Fi
 * frames, general typedefs, etc.
 */

#define HEADER_SIZE (sizeof(struct wifi_header) + sizeof(struct llc_header) + sizeof(struct ip_header) + sizeof(struct udp_header))
#define FRAME_SIZE 1500

#define ETH_P_CTRL 0x4e44

void send_frame(struct wlc_info *wlc, struct wifi_frame *frame, size_t frame_size);
void send_to_os(struct wl_info *wl, uint8 *payload, size_t payload_size);

int hex_to_num(char c);
int hwaddr_aton(const char *str_addr, uint8 *addr);
int inet_aton(const char *str_addr, uint32 *addr);

#endif // _HELPERS_H
