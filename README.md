# btlid-c

NOTE: This only works on MacOS (Confirmed working on 11.5.1, but should work
on earlier versions 10.9+)

Disconnect bluetooth devices when the laptop lid is closed.

This is useful because Macbooks like to remain connected to bluetooth, which
sucks when you you want to connect your headphones with another device like your
phone.

Functionality
* When laptop lid closes, turn bluetooth off (if it's currently on)
* When laptop lid opens, return bluetooth to previous state before lid closed

Probably won't autoconnect when lid opens again, but that's a problem to fix
another time. Also would not recommend if you use a bluetooth keyboard/mouse
while lid is closed.

## Build & Install

Install, along with daemon on system start:
```sh
make
sudo make install
```

Uninstall:
```sh
sudo make uninstall
```
