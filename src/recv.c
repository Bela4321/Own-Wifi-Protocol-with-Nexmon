#pragma NEXMON targetregion "patch"

#include <firmware_version.h>
#include <wrapper.h>	// wrapper definitions for functions that already exist in the firmware
#include <structs.h>	// structures that are used by the code in the firmware
#include <patcher.h>
#include <helper.h>
#include <sendframe.h>
#include <udptunnel.h>

#include "local_helpers.h"
#include "sbp.h"

void wlc_recvdata_hook(struct wlc_info *wlc, void *osh, void *rxh, struct sk_buff *p) {
    /**
     * YOUR CODE HERE
     * This function is always called when the Wi-Fi chip receives a data frame.
     * Management frames are filtered before, so you don't have to check for the
     * frame type.
     */

    // If we have an empty packet or no packet at all, we just do the firmware
    // do its job. Do not remove this, otherwise it could lead to the firmware
    // not working properly.
    if (p == 0 || p->data == 0) {
        wlc_recvdata(wlc, osh, rxh, p);
        return;
    }

    // If you parse an incoming frame here, you have to skip the first 6 bytes
    // due to the Wi-Fi's internal packet handling.
    struct ... = (struct ...) (p->data + 6);

    udpprintf(wlc->wl, "Received Frame.");

    wlc_recvdata(wlc, osh, rxh, p);
}

__attribute__((at(0x1b6bf6, "", CHIP_VER_BCM43455c0, FW_VER_7_45_189)))
BLPatch(wlc_recvdata_hook, wlc_recvdata_hook);
