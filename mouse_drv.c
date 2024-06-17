#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/hid.h>
#include <linux/usb.h>

#include "mouse_drv.h"
#include "set_led.h"
//#define DEBUG

static ssize_t attr_write_test(struct device *dev, struct device_attribute* attr, const char *buf, size_t count){
    return attr_write_test_common(dev,attr,buf,count,LED_LOGO);
}
static ssize_t attr_write_logo_breath_effect(struct device *dev, struct device_attribute* attr, const char *buf, size_t count){
    return attr_write_breath_effect_common(dev,attr,buf,count,LED_LOGO);
}
static ssize_t attr_write_scroll_wheel_breath_effect(struct device *dev, struct device_attribute* attr, const char *buf, size_t count){
    return attr_write_breath_effect_common(dev,attr,buf,count,LED_SCROLL_WHEEL);
}

static ssize_t attr_write_logo_brightness(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    return attr_write_brightness_common(dev, attr, buf, count, LED_LOGO);
}
static ssize_t attr_write_scroll_wheel_brightness(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    return attr_write_brightness_common(dev, attr, buf, count, LED_SCROLL_WHEEL);
}

static DEVICE_ATTR(test, 0220, NULL, attr_write_test);
static DEVICE_ATTR(logo_breath_effect, 0220, NULL, attr_write_logo_breath_effect);
static DEVICE_ATTR(scroll_wheel_breath_effect, 0220, NULL, attr_write_scroll_wheel_breath_effect);
static DEVICE_ATTR(logo_brightness, 0220, NULL, attr_write_logo_brightness);
static DEVICE_ATTR(scroll_wheel_brightness, 0220, NULL, attr_write_scroll_wheel_brightness);

static int mouse_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
    int ret = 0;
    struct usb_interface *intf = to_usb_interface(hdev->dev.parent);
    struct usb_device *usb_dev = interface_to_usbdev(intf);
    struct razer_mouse_device *dev = NULL;

    #ifdef DEBUG
    /* check which interface are we using */
    printk(KERN_INFO " USB info %d now probed: %04X ,  %04X\n",intf->cur_altsetting->desc.bInterfaceNumber, id->vendor, id->product);
	printk(KERN_INFO " interface protocol : %d\n", intf->cur_altsetting->desc.bInterfaceProtocol);
    if(intf->cur_altsetting->desc.bInterfaceProtocol == 2)
	    printk(KERN_INFO "mouse interface\n");
    else if(intf->cur_altsetting->desc.bInterfaceProtocol == 1)
        printk(KERN_INFO "kbd interface\n");    
    #endif

    dev = kzalloc(sizeof(struct razer_mouse_device), GFP_KERNEL);
    if (dev == NULL)
    {
        dev_err(&intf->dev, "out of memory\n");
        return -ENOMEM;
    }

    /* Init the custom struct */
    mutex_init(&dev->lock);
    dev->usb_dev = usb_dev;

    /* create sysfs file */
    if (intf->cur_altsetting->desc.bInterfaceProtocol == USB_INTERFACE_PROTOCOL_MOUSE)
    {
        CREATE_DEVICE_FILE(&hdev->dev, &dev_attr_test);
        CREATE_DEVICE_FILE(&hdev->dev, &dev_attr_logo_breath_effect);
        CREATE_DEVICE_FILE(&hdev->dev, &dev_attr_scroll_wheel_breath_effect);
        CREATE_DEVICE_FILE(&hdev->dev, &dev_attr_logo_brightness);       
        CREATE_DEVICE_FILE(&hdev->dev, &dev_attr_scroll_wheel_brightness);
    }

    /* save custom mouse structure's data to hdev->dev->driver_data */
    hid_set_drvdata(hdev, dev);

    /* parse the report descriptor, and set the hid_device */
    ret = hid_parse(hdev);
    if (ret)
    {
        hid_err(hdev, "parse failed\n");
        goto exit_free;
    }
    /* start the device and call hid_connect(connect to the hid subsystem), call hid_hw_stop in disconnect function */
    ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
    if (ret)
    {
        hid_err(hdev, "hw start failed\n");
        goto exit_free;
    }

    dev_info(&intf->dev,"Probe done!");

    return 0;

exit_free:
    kfree(dev);
    return ret;
}

static void mouse_disconnect(struct hid_device *hdev)
{
    struct razer_mouse_device *dev;
    struct usb_interface *intf = to_usb_interface(hdev->dev.parent);

    dev = hid_get_drvdata(hdev);

    if (intf->cur_altsetting->desc.bInterfaceProtocol == USB_INTERFACE_PROTOCOL_MOUSE)
    {
        device_remove_file(&hdev->dev, &dev_attr_test);
        device_remove_file(&hdev->dev, &dev_attr_logo_breath_effect);
        device_remove_file(&hdev->dev, &dev_attr_scroll_wheel_breath_effect);
        device_remove_file(&hdev->dev, &dev_attr_logo_brightness);
        device_remove_file(&hdev->dev, &dev_attr_scroll_wheel_brightness);        
    }

    hid_hw_stop(hdev);
    
    kfree(dev);
    dev_info(&intf->dev, "Razer Mouse disconnected\n");
}


static const struct hid_device_id mouse_id_table[] = {
    {HID_USB_DEVICE(VENDOR_ID_RAZER, PRODUCT_ID_BASILISK_V2)},
    {},
};
/* generate module name alias */
MODULE_DEVICE_TABLE(hid, mouse_id_table);

/* hid driver contents */
static struct hid_driver razer_mouse_driver = {
    .name = "razer_mouse",
    .id_table = mouse_id_table,
    .probe = mouse_probe,
    .remove = mouse_disconnect,
};
/* register & unregister, this macro includes module_init & module_exit */
module_hid_driver(razer_mouse_driver);

MODULE_AUTHOR("ihLin <sean110456@gmail.com>");
MODULE_DESCRIPTION("Razer basilisk v2 mouse driver");
MODULE_LICENSE("GPL");
