/* mbed OS IP stack API
 * Copyright (c) 2015-2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_IPSTACK_H
#define MBED_IPSTACK_H

#include "nsapi.h"
#include "emac_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * mbed OS API for IP stack abstraction
 *
 * This interface should be used by targets to initialize IP stack, create, bring up and bring down network interfaces.
 */

/** Initialize IP stack
 *
 * This function should be called before any of the network interfaces is added. It's up to target's setup code, to make
 * sure that the stack is initialized and all the existing interfaces are registered with the stack.
 * This function can be safely called multiple times, it will do nothing and return NSAPI_ERROR_OK if stack is already
 * initialized.
 *
 * @return    NSAPI_ERROR_OK on success, or error code
 */
void mbed_ipstack_init(void);

/** Register a network interface with the IP stack
 *
 * Connects EMAC layer with the IP stack and initializes all the required infrastructure.
 * This function should be called only once for each available interface.
 *
 * @param    emac       EMAC HAL implementation for this network interface
 * @param    default_if true if the interface should be treated as the default one
 * @return              NSAPI_ERROR_OK on success, or error code
 */
nsapi_error_t mbed_ipstack_add_netif(emac_interface_t *emac, bool default_if);

/** Connect network stack with the IP stack
 *
 * @param   emac        EMAC HAL implementation
 * @param   stack       Pointer to nsapi_stack_t to be set for this interface
 */
void mbed_ipstack_set_stack(emac_interface_t *emac, nsapi_stack_t *stack);

/** Connect the interface to the network
 *
 * Sets up a connection on specified network interface, using DHCP or provided network details. If the @a dhcp is set to
 * true all the remaining parameters are ignored.
 *
 * @param    emac       EMAC HAL implementation for this network interface
 * @param    dhcp       true if the network details should be acquired using DHCP
 * @param    ip         IP address to be used for the interface as "W:X:Y:Z" or NULL
 * @param    netmask    Net mask to be used for the interface as "W:X:Y:Z" or NULL
 * @param    gw         Gateway address to be used for the interface as "W:X:Y:Z" or NULL
 * @return              NSAPI_ERROR_OK on success, or error code
 */
nsapi_error_t mbed_ipstack_bringup(emac_interface_t *emac, bool dhcp, const char *ip,
                                   const char *netmask, const char *gw);

/** Disconnect interface from the network
 *
 * After this call the network interface is inactive, to use it again user needs to call @mbed_lwip_bringup again.
 *
 * @return    NSAPI_ERROR_OK on success, or error code
 */
nsapi_error_t mbed_ipstack_bringdown(emac_interface_t *emac);

/** Return MAC address of the network interface
 *
 * @param    emac       EMAC HAL implementation for this network interface
 * @return              MAC address as "V:W:X:Y:Z"
 */
char *mbed_ipstack_get_mac_address(emac_interface_t *emac);

/** Copies IP address of the network interface to user supplied buffer
 *
 * @param    emac       EMAC HAL implementation for this network interface
 * @param    buf        buffer to which IP address will be copied as "W:X:Y:Z"
 * @param    buflen     size of supplied buffer
 * @return              Pointer to a buffer, or NULL if the buffer is too small
 */
char *mbed_ipstack_get_ip_address(emac_interface_t *emac, char *buf, nsapi_size_t buflen);

/** Copies netmask of the network interface to user supplied buffer
 *
 * @param    emac       EMAC HAL implementation for this network interface
 * @param    buf        buffer to which netmask will be copied as "W:X:Y:Z"
 * @param    buflen     size of supplied buffer
 * @return              Pointer to a buffer, or NULL if the buffer is too small
 */
char *mbed_ipstack_get_netmask(emac_interface_t *emac, char *buf, nsapi_size_t buflen);

/** Copies gateway address of the network interface to user supplied buffer
 *
 * @param    emac       EMAC HAL implementation for this network interface
 * @param    buf        buffer to which gateway address will be copied as "W:X:Y:Z"
 * @param    buflen     size of supplied buffer
 * @return              Pointer to a buffer, or NULL if the buffer is too small
 */
char *mbed_ipstack_get_gateway(emac_interface_t *emac, char *buf, nsapi_size_t buflen);

#ifdef __cplusplus
}
#endif

#endif /* MBED_IPSTACK_H */
