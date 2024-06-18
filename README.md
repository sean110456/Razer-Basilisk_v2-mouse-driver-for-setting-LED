# Razer Basilisk_v2 mouse driver for setting LED
This is a linux module for razer basillisk_v2 gaming mouse, enabling functions for setting LED through sysfs.

## Install
```
make
insmod razer_mouse_drv.ko
```

## Usage
The module creates sysfs attribute files in the directory of your sys device file (the path may be like /sys/bus/usb/devices/usb1/1-7/1-7:1.0)

FILES: logo_breath_effect, scroll_wheel_breath_effect, logo_brightness, scroll_wheel_brightness, test respectively.

Write bytes to those files to change the LED mode of your mouse.

e.g. Write 3 bytes(RGB) to make the logo LED show breath effect with color red
```
echo -en '\xff\x00\x00'> logo_breath_effect
```
For more details, check the comments of set_led.c.
