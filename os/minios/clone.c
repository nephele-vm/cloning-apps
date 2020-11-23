/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Authors: Costin Lupu <costin.lupu@cs.pub.ro>
 *
 * Copyright (c) 2020, University Politehnica of Bucharest. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <mini-os/types.h>
#include <mini-os/unistd.h>
#include <mini-os/xmalloc.h>
#include <mini-os/errno.h>
#include <xen/xen.h>
#include <common/log.h>
#include <common/cmdline.h>
#include <common/clone.h>


int os_clone(unsigned int nr_children)
{
    domid_t *child_domids;
    int rc;

    DEBUG("cloning children=%d\n", nr_children);

    if (!nr_children) {
        rc = -EINVAL;
        goto out;
    }

    child_domids = xmalloc_array(domid_t, nr_children);
    if (!child_domids) {
        rc = -ENOMEM;
        goto out;
    }

    rc = clone(nr_children, child_domids);
    if (rc == 0) /* parent */
        ;
    else if (rc == 1) { /* child */
        do_fork = 0; /* only parent forks */
        do_clone = 0;
    } else {
        ERROR("Error calling clone() rc=%d\n", rc);
        rc = -1;
    }

    xfree(child_domids);

out:
    return rc;
}

unsigned int os_get_self_id(void)
{
    return xenbus_get_self_id();
}