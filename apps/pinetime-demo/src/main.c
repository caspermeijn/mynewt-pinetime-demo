/*
 * Copyright 2020 Casper Meijn <casper@meijn.net>
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

#include <assert.h>
#include <string.h>

#include "sysinit/sysinit.h"
#include "os/os.h"
#include "os/os_callout.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"
#include "sgm4056/sgm4056.h"
#include "console/console.h"
#ifdef ARCH_sim
#include "mcu/mcu_sim.h"
#endif

static int 
charge_control_data_callback(struct charge_control *chg_ctrl, void * arg,
        void *data, charge_control_type_t type) 
{
    if (type == CHARGE_CONTROL_TYPE_STATUS) {
        charge_control_status_t charger_status = *(charge_control_status_t*)(data);

        console_printf("Charger state is ");
        if (charger_status == CHARGE_CONTROL_STATUS_NO_SOURCE) {
            console_printf("no source detected\n");
        } else if (charger_status == CHARGE_CONTROL_STATUS_CHARGING) {
            console_printf("charging\n");
        } else if (charger_status == CHARGE_CONTROL_STATUS_CHARGE_COMPLETE) {
            console_printf("charge completed\n");
        } else {
            assert(false);
        }
    } else {
        assert(false);
    }
    console_flush();
    return 0;
}

struct charge_control *charge_control;

struct charge_control_listener charge_control_listener = {
    .ccl_type = CHARGE_CONTROL_TYPE_STATUS,
    .ccl_func = charge_control_data_callback,
};

static void 
charge_control_init(void)
{
    charger = charge_control_mgr_find_next_bytype(CHARGE_CONTROL_TYPE_STATUS, NULL);
    assert(charger);

    rc = charge_control_set_poll_rate_ms("charger", 10000);
    assert(rc == 0);

    rc = charge_control_register_listener(charger, &charger_listener);
    assert(rc == 0);
}

static void periodic_callback(struct os_event *ev);
static struct os_callout periodic_callout;

static void periodic_init()
{
    int rc;

    rc = hal_gpio_init_out(LCD_BACKLIGHT_HIGH_PIN, 1);
    assert(rc == 0);

    os_callout_init(&periodic_callout, os_eventq_dflt_get(),
                    periodic_callback, NULL);

    rc = os_callout_reset(&periodic_callout, 0);
    assert(rc == 0);
}

static void periodic_callback(struct os_event *ev)
{
    int rc;

    /* Toggle blinking led */
    hal_gpio_toggle(LED_BLINK_PIN); 

    /* Trigger in another second */
    rc = os_callout_reset(&periodic_callout, OS_TICKS_PER_SEC);
    assert(rc == 0);
}

int
main(int argc, char **argv)
{
    int rc;

#ifdef ARCH_sim
    mcu_sim_parse_args(argc, argv);
#endif

    sysinit();

    periodic_init();
    charge_control_init();

    while (1) {
       os_eventq_run(os_eventq_dflt_get());
    }
    
    assert(0);

    return rc;
}

