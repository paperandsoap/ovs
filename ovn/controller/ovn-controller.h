/* Copyright (c) 2015, 2016 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef OVN_CONTROLLER_H
#define OVN_CONTROLLER_H 1

#include "simap.h"
#include "ovn/lib/ovn-sb-idl.h"

/* Linux supports a maximum of 64K zones, which seems like a fine default. */
#define MAX_CT_ZONES 65535

struct controller_ctx {
    struct ovsdb_idl *ovnsb_idl;
    struct ovsdb_idl_txn *ovnsb_idl_txn;

    struct ovsdb_idl *ovs_idl;
    struct ovsdb_idl_txn *ovs_idl_txn;
};

/* Contains hmap_node whose hash values are the tunnel_key of datapaths
 * with at least one local port binding. It also stores the port binding of
 * "localnet" port if such a port exists on the datapath, which indicates
 * physical network should be used for inter-chassis communication through
 * the localnet port */
struct local_datapath {
    struct hmap_node hmap_node;
    const struct sbrec_port_binding *localnet_port;
};

struct local_datapath *get_local_datapath(const struct hmap *,
                                          uint32_t tunnel_key);

/* Contains hmap_node whose hash values are the tunnel_key of datapaths
 * with at least one logical patch port binding. */
struct patched_datapath {
    struct hmap_node hmap_node;
};

struct patched_datapath *get_patched_datapath(const struct hmap *,
                                              uint32_t tunnel_key);

const struct ovsrec_bridge *get_bridge(struct ovsdb_idl *,
                                       const char *br_name);

const struct sbrec_chassis *get_chassis(struct ovsdb_idl *,
                                        const char *chassis_id);

/* Must be a bit-field ordered from most-preferred (higher number) to
 * least-preferred (lower number). */
enum chassis_tunnel_type {
    GENEVE = 1 << 2,
    STT    = 1 << 1,
    VXLAN  = 1 << 0
};

uint32_t get_tunnel_type(const char *name);


#endif /* ovn/ovn-controller.h */
