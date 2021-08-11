#import <IOBluetooth/IOBluetooth.h>

#include <ctype.h>
#include <stdio.h>
#include <IOKit/IOMessage.h>
#include <IOKit/pwr_mgt/IOPMLib.h>

io_connect_t root_port;
bool presleep_bluetooth_enabled = true;

// Private methods from <IOBluetooth.h>
int IOBluetoothPreferenceGetControllerPowerState();
void IOBluetoothPreferenceSetControllerPowerState(int state);

int get_bluetooth_state() {
    return IOBluetoothPreferenceGetControllerPowerState();
}

void set_bluetooth_state(bool enabled) {
    printf("setting bluetooth state to: %s\n",
            enabled ? "enabled" : "disabled");
    IOBluetoothPreferenceSetControllerPowerState(enabled);
}

void sleep_callback(void *ref_con __attribute__((unused)),
        io_service_t service __attribute__((unused)),
        natural_t message_type,
        void *message_arg) {
    switch (message_type) {
    case kIOMessageCanSystemSleep:
        printf("received system can sleep callback\n");
        IOAllowPowerChange(root_port, (long)message_arg);
        break;
    case kIOMessageSystemWillSleep:
        printf("received system will sleep callback\n");
        presleep_bluetooth_enabled = get_bluetooth_state();
        if (presleep_bluetooth_enabled) {
            set_bluetooth_state(false);
        }
        IOAllowPowerChange(root_port, (long)message_arg);
        break;
    case kIOMessageSystemHasPoweredOn:
        printf("received system powered on callback\n");
        if (presleep_bluetooth_enabled) {
            set_bluetooth_state(true);
        }
        break;
    }
}

int main(int argc __attribute__((unused)),
        char **argv __attribute__((unused))) {
    IONotificationPortRef notify_port_ref;
    io_object_t notifier_obj;
    void *ref_con = NULL;

    root_port = IORegisterForSystemPower(ref_con, &notify_port_ref,
            sleep_callback, &notifier_obj);
    if (root_port == 0) {
        fprintf(stderr, "failed IORegisterForSystemPower\n");
        return 1;
    }

    // Add the notification port to the application runloop
    CFRunLoopAddSource(CFRunLoopGetCurrent(),
            IONotificationPortGetRunLoopSource(notify_port_ref),
            kCFRunLoopCommonModes);

    // Start run loop to receive sleep events
    printf("starting run loop\n");
    CFRunLoopRun();
    return 0;
}
