/**
 * YOUR CODE HERE
 * This file should contain the implementations of the things defined in
 * local_helpers.h
 */

#pragma NEXMON targetregion "patch"

#include "local_helpers.h"
#include "sbp.h"

int hex_to_num(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
  }
	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
  }
	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
  }
	return -1;
}

int hwaddr_aton(const char *str_addr, uint8 *addr) {
	int i;

	for (i = 0; i < 6; i++) {
		int a, b;

		a = hex_to_num(*str_addr++);
		if (a < 0) {
			return -1;
    }

		b = hex_to_num(*str_addr++);
		if (b < 0) {
			return -1;
    }

		*addr++ = (a << 4) | b;

		if (i < 5 && *str_addr++ != ':') {
			return -1;
    }
	}

	return 0;
}


int inet_aton(const char *str_addr, uint32 *addr) {
  int dots = 0;
  uint32 acc = 0;

  do {
	  char cc = *str_addr;

    switch (cc) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        acc = acc * 10 + (cc - '0');
        break;

      case '.':
        if (++dots > 3) {
          return -1;
        }

      case '\0':
        if (acc > 255) {
          return -1;
        }
        *addr = *addr << 8 | acc;
        acc = 0;
        break;

      default:
        return -1;
    }
  } while (*str_addr++) ;

  if (dots < 3) {
	  *addr <<= 8 * (3 - dots);
  }

  *addr = htonl(*addr);

  return 0;
}

void send_to_os(struct wl_info *wl, uint8 *payload, size_t payload_size) {
    struct sk_buff *p = pkt_buf_get_skb(wl->wlc->osh, sizeof(struct ethernet_header) + payload_size);

    if (p != 0) {
        skb_pull(p, sizeof(struct ethernet_header));

        memcpy(p->data, payload, payload_size);
        p->len = payload_size;

        skb_push(p, sizeof(send_to_os_template));
        memcpy(p->data, &send_to_os_template, sizeof(send_to_os_template));

        wl->dev->chained->funcs->xmit(wl->dev, wl->dev->chained, p);
    }
}

// This function sends a ready wifi frame.
// Everything has to be prepared except size and checksum of the IP and UDP layers.
void send_frame(struct wlc_info *wlc, struct wifi_frame *frame, size_t frame_size) {
  size_t sk_buff_size = frame_size + SK_BUFF_PREFIX_SIZE;
  struct sk_buff *p = pkt_buf_get_skb(wlc->osh, sk_buff_size);

  skb_pull(p, sk_buff_size);
  skb_push(p, frame_size);

  memcpy(p->data, frame, frame_size);
  p->len = frame_size;

  sendframe(wlc, p, 1, 0);
}
