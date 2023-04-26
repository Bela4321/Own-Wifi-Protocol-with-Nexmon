/***************************************************************************
 *                                                                         *
 *          ###########   ###########   ##########    ##########           *
 *         ############  ############  ############  ############          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ###########   ####  ######  ##   ##   ##  ##    ######          *
 *          ###########  ####  #       ##   ##   ##  ##    #    #          *
 *                   ##  ##    ######  ##   ##   ##  ##    #    #          *
 *                   ##  ##    #       ##   ##   ##  ##    #    #          *
 *         ############  ##### ######  ##   ##   ##  ##### ######          *
 *         ###########    ###########  ##   ##   ##   ##########           *
 *                                                                         *
 *            S E C U R E   M O B I L E   N E T W O R K I N G              *
 *                                                                         *
 * This file is part of NexMon.                                            *
 *                                                                         *
 * Copyright (c) 2016 NexMon Team                                          *
 *                                                                         *
 * NexMon is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * NexMon is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with NexMon. If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                         *
 **************************************************************************/

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

int wl_send_hook(struct hndrte_dev *src, struct hndrte_dev *dev, struct sk_buff *p) {

    /**
     * YOUR CODE HERE
     * This function is always called when the user or kernel want to send a
     * frame. When you start your userland program and send a sensor value,
     * you can intercept it here and alter the data or adjust things for your
     * protocol.
     */

    // If we have an empty packet or no packet at all, we just do the firmware
    // do its job. Do not remove this, otherwise it could lead to the firmware
    // not working properly.
    if (p == 0 || p->data == 0) {
        return wl_send(src, dev, p);
    }

    struct ... *out_frame = (struct ...) p->data;

    // In this case the user space or kernel is communicating with the Wi-Fi firmware.
    if (out_frame->type == ntohs(ETH_P_CTRL)) {
        // We need the wl_info for sending the frame later on.
        struct wl_info *wl = (struct wl_info *) dev->softc;

        udpprintf(wl, "Test.");
    }

    return wl_send(src, dev, p);
}

__attribute__((at(0x202240, "", CHIP_VER_BCM43455c0, FW_VER_7_45_189)))
GenericPatch4(wl_send_hook, wl_send_hook + 1);
