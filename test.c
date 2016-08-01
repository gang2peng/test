#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
#include <mach/gpio-exynos4.h>

#define DRIVER_NAME "hello_ctl"
#define DEVICE_NAME "hello_ctl123"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("PG");

int hello_open (struct inode *inode, struct file *file){
	printk(KERN_EMERG "\nhello open !\n");
	return 0;
}

int hello_release (struct inode *inode, struct file *file){
	printk(KERN_EMERG "\nhello release !\n");
	return 0;
}

long hello_ioctl (struct file *file, unsigned int cmd, unsigned long arg){
	
	
	printk(KERN_EMERG "\ncmd is %d,arg is %ld\n",cmd,arg);
	if(cmd >1){
		printk("KERN_EMERG cmd is 0 or 1\n");
	}
	if(arg >1){
		printk("KERN_EMERG cmd is 0 or 1\n");
	}
	
	gpio_set_value(EXYNOS4_GPL2(0),cmd);
	
	return 0;
}


struct file_operations hello_ops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.unlocked_ioctl = hello_ioctl,
};


struct miscdevice hello_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &hello_ops,
};

int hello_probe(struct platform_device *pdv){
	int ret;
	printk(KERN_EMERG "\tinitialized\n");
	
	
	ret = gpio_request(EXYNOS4_GPL2(0),"LEDS");
	if(ret<0){
		printk(KERN_EMERG "gpio_request EXYNOS4_GPL2 failed");
		return ret;
	}
	
	s3c_gpio_cfgpin(EXYNOS4_GPL2(0),S3C_GPIO_OUTPUT);
	gpio_set_value(EXYNOS4_GPL2(0),0);
	misc_register(&hello_dev);
	
	return 0;
}

int hello_remove(struct platform_device *pdv){
	printk(KERN_EMERG "\nREMOVE!\n");
	misc_deregister(&hello_dev);
	return 0;
}

void hello_shutdown(struct platform_device *pdv){

}

int hello_suspend(struct platform_device *pdv,pm_message_t state){
	return 0;
}

int hello_resume(struct platform_device *pdv){
	return 0;
}

struct platform_driver hello_driver = {
	.probe = hello_probe,
	.remove = hello_remove,
	.shutdown = hello_shutdown,
	.suspend = hello_suspend,
	.resume = hello_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};


static int hello_init(void){
	int DriverState;
	printk(KERN_EMERG "HELLO INIT !!\n");
	DriverState = platform_driver_register(&hello_driver);
	printk(KERN_EMERG "\nDriverState = %d\n",DriverState);
	return 0;
}

static void hello_exit(void){
	platform_driver_unregister(&hello_driver);
	printk(KERN_EMERG "EXIT INIT !!\n");
}

module_init(hello_init);
module_exit(hello_exit);