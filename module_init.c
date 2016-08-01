#include <linux/module.h>
#include <linux/init.h>

#include <linux/stat.h>
#include <linux/moduleparam.h>

#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>

#define device_name "JTSB"
#define device_MINOR_NUM 2
#define numdev_MAJOR 0
#define numdev_MINOR 0
#define REGDEV_SIZE 3000


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("PG");

int  module_arg1 = numdev_MAJOR;
int  module_arg2 = numdev_MINOR;
int param_array[30];
int num_array;

module_param(module_arg1,int,S_IRUSR);
module_param(module_arg2,int,S_IRUSR);
module_param_array(param_array,int,&num_array,S_IRUSR);

struct reg_dev {
	char *date;
	unsigned int size;
	struct cdev cdev;
};

struct class *myclass;

struct reg_dev *my_device;

int  JTSB_open(struct inode *inode, struct file *file){
	printk(KERN_EMERG "JTSB_open is success!\n");
	return 0;
}


int  JTSB_release(struct inode *inode, struct file *file){
	printk(KERN_EMERG "JTSB_release is success!\n");
	return 0;
}

long  JTSB_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	printk(KERN_EMERG "JTSB_ioctl is success! cmd is %d,arg is %d \n",cmd,arg);
	return 0;
}

struct file_operations my_fops={
	.owner = THIS_MODULE,
	.open = JTSB_open,
	.release = JTSB_release,
	.unlocked_ioctl = JTSB_ioctl,
	
};

static void reg_init_cdev(struct reg_dev *dev,int index){
	int err;
	dev_t dev_no;
	dev_no=MKDEV(module_arg1,module_arg2+index);
	
	cdev_init(&dev->cdev,&my_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &my_fops;
	
	err=cdev_add(&dev->cdev,dev_no,1);
	if(err){
		printk(KERN_EMERG "cdev_add %d is fail!! %d\n",index,err);
	}
	else{
		printk(KERN_EMERG "cdev_add %d is seccuse!!\n",index);
	}
	
}

static int hello_init(void){
		int i;
		int j;
		int ret=0;
		dev_t num_dev;
		printk(KERN_EMERG "HELLO!!\n");
		printk(KERN_EMERG "module_arg1 is %d \n",module_arg1);
		printk(KERN_EMERG "module_arg2 is %d \n",module_arg2);
		for(i=0;i<num_array;i++)
			printk("param_array[%d] is %d\n",i,param_array[i]);
		
		if(module_arg1){
			num_dev = MKDEV(module_arg1,module_arg2);
			ret=register_chrdev_region(num_dev,device_MINOR_NUM,device_name);
			printk(KERN_EMERG "major device is %d\n",module_arg1);	
		}
		else{
			ret=alloc_chrdev_region(&num_dev,module_arg2,device_MINOR_NUM,device_name);
			module_arg1=MAJOR(num_dev);
			printk(KERN_EMERG "major device is %d\n",module_arg1);	
		}
		
		if(ret<0){
			printk(KERN_EMERG "register_chrdev_region %s fail!!\n",device_name);
		}
		
		myclass = class_create(THIS_MODULE,device_name);
		
		my_device = kmalloc(device_MINOR_NUM *sizeof(struct reg_dev),GFP_KERNEL);
		if(!my_device){
			ret = -ENOMEM;
			goto fail;
		}
		memset(my_device,0,device_MINOR_NUM *sizeof(struct reg_dev));
		for(j=0;j<device_MINOR_NUM;j++){
			my_device[j].date = kmalloc(REGDEV_SIZE,GFP_KERNEL);
			memset(&my_device[j],0,REGDEV_SIZE);
			reg_init_cdev(&my_device[j],j);
			
			device_create(myclass,NULL,MKDEV(module_arg1,module_arg2+j),NULL,device_name"%d",j);
			
		}
		printk(KERN_EMERG "init!!\n");
		return 0;
fail:
	unregister_chrdev_region(MKDEV(module_arg1,module_arg2),device_MINOR_NUM);
	printk(KERN_EMERG "Kmalloc is fail!!\n");
	return ret;
}

static void hello_exit(void){
		int i;
		printk(KERN_EMERG "GOODBYE!\n");
		for(i=0;i<device_MINOR_NUM;i++){
			cdev_del(&my_device[i].cdev);
			device_destroy(myclass,MKDEV(module_arg1,module_arg2+i));	
		}
		
		class_destroy(myclass);
		kfree(myclass);
		
		unregister_chrdev_region(MKDEV(module_arg1,module_arg2),device_MINOR_NUM);
}

module_init(hello_init);
module_exit(hello_exit);