CONFIG_MODULE_SIG=n # without signature

obj-m := razer_mouse_drv.o
razer_mouse_drv-y := mouse_drv.o set_led.o
ccflags-y += -g

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
install:
	$(MAKE) -C $(KDIR) M=$(PWD) module_install
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
