#ifndef MOUSE_DRV_H
#define MOUSE_DRV_H

#define VENDOR_ID_RAZER 0x1532
#define PRODUCT_ID_BASILISK_V2 0x0085

struct razer_mouse_device
{
    struct usb_device *usb_dev;
    struct mutex lock;
};

/* create sysfs file, free resources if failed */
#define CREATE_DEVICE_FILE(dev, type)  \
do                                     \
{                                      \
    if (device_create_file(dev, type)) \
    {                                  \
        goto exit_free;                \
    }                                  \
} while (0)

#endif /* MOUSE_DRV_H */