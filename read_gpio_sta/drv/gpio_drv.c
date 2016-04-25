

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/gpio.h>
#include <mach/mux.h>

//#include "gpio_drv.h"

#define SUCCESS 0
#define DEVICE_NAME "button"

#define DEF_GPIO_DIR_OUT    0x01
#define DEF_GPIO_DIR_IN     0x06
#define DEF_GPIO_SET_DATA   0x03
#define DEF_GPIO_CLR_DATA   0x04
#define DEF_GPIO_GET_DATA   0x05

static DEFINE_SPINLOCK(gpio_lock);
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static int device_release(struct inode *, struct file *);

static int Major;		/* Major number assigned to our device driver */
static int wait_flag;
static struct class * chardev_class;
static int Device_Open = 0;	/* Is device open?  
				 * Used to prevent multiple access to device */

static volatile char key_values [] = {'0'};


int device_ioctl(struct inode *inode,	/* see include/linux/fs.h */
		 struct file *file,	/* ditto */
		 unsigned int cmd,	/* number and param for ioctl */
		 unsigned long arg);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.ioctl = device_ioctl,
	.read = device_read,
	.open = device_open,
	.release = device_release
};

int gpio_dir_input(unsigned gpio)
{
    struct gpio_controller * __iomem g;
    u32 mask;
    u32 temp;

    g = __gpio_to_controller(gpio);
    if(!g)
    {
        return -EINVAL;
    }

    spin_lock(&gpio_lock);
    mask = __gpio_mask(gpio);
    temp = __raw_readl(&g->dir);
    printk("temp = %x, mask = %x\n", temp, mask);
    
    temp |= mask; // set to 1
    printk("dir to be set = %x\n", temp);
    
    __raw_writel(temp, &g->dir); // write dir setting to hardware

    temp = __raw_readl(&g->dir);
    printk("dir = %x, mask = %x\n", temp, mask);
    
    spin_unlock(&gpio_lock);

    return SUCCESS;
}

int gpio_dir_output(unsigned gpio)
{
    struct gpio_controller * __iomem g;
    u32 mask;
    u32 temp;

    g = __gpio_to_controller(gpio);
    if(!g)
    {
        return -EINVAL;
    }

    spin_lock(&gpio_lock);
    mask = __gpio_mask(gpio);
    temp = __raw_readl(&g->dir);
    temp &= ~mask; // set to 0
    __raw_writel(temp, &g->dir); // write dir setting to hardware
    spin_unlock(&gpio_lock);

    return SUCCESS;
}

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
    int num_temp;

    Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}

    //davinci_cfg_reg(DM365_GPIO26);
//    gpio_request(0, "button");
//    num_temp = __gpio_to_irq(0);

//    printk("gpio_to_irq(0) = %x\n", num_temp);
    

//	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
//	printk(KERN_INFO "the driver, create a dev file with\n");
//	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
//	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
//	printk(KERN_INFO "the device file.\n");
//	printk(KERN_INFO "Remove the device file and module when done.\n");

	printk(KERN_INFO "I was assigned major number %d.\n", Major);
        
        chardev_class = class_create(THIS_MODULE, "leo368_leds_class");
        device_create(chardev_class, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME);

	return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	/* 
	 * Unregister the device 
	 */
	//int ret = unregister_chrdev(Major, DEVICE_NAME);
	//if (ret < 0)
	//	printk(KERN_ALERT "Error in unregister_chrdev: %d\n", ret);

	unregister_chrdev(Major, DEVICE_NAME);

	device_destroy(chardev_class,MKDEV(Major, 0));
	class_destroy(chardev_class);
}

#if 0
int gpio_get_settings(int gpio, unsigned long reg_val, char *reg_type)
{
	/* Verify if the bit is set right or not */
	int mask = 0, set_value = 0;
	mask = 1 << (gpio % 32);
	set_value = (reg_val & mask) >> (gpio % 32);
	printk("<1> The value was set to %d for gpio %d in %s register.\n\n",
	       set_value, gpio, reg_type);

	return set_value;
}


void gpio_get_in_data_reg(int gio)
{
	volatile unsigned long *reg;
	int blank = (gio >> 5) & 0x3;
	int offset = 0;

	offset = 0x20 + blank * 0x28;

	printk("offset = %x\n", offset);

	reg = (unsigned long *)IO_ADDRESS(DAVINCI_GPIO_BASE + offset);

    printk("<1> GPIO IN_DATA bank 0 and 1:\t 0x%lx\n",*reg);

	gpio_get_settings(gio, *reg, "IN_DATA");
}
#endif

static int device_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
    unsigned long err;
    int ret;

    ret = gpio_get_value(26);

    if(ret & 0x4000000)
    {
        key_values[0] = '0';
    }
    else
    {
        key_values[0] = '1';
    }

    if(wait_flag)

    //printk("key_values[0]=%c\n",key_values[0]);
    //printk("ret = %x, key_values[0]=%c\n", ret, key_values[0]);
    
    err = copy_to_user(buff, (const void *)key_values, min(sizeof(key_values), count));

    key_values[0]='0';

    return err ? -EFAULT : min(sizeof(key_values), count);
}

/* 
 * This function is called whenever a process tries to do an ioctl on our
 * device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called
 * and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */
int device_ioctl(struct inode *inode,	/* see include/linux/fs.h */
		 struct file *file,	/* ditto */
		 unsigned int cmd,	/* number and param for ioctl */
		 unsigned long arg)
{
    int ret = SUCCESS;

    //printk("cmd=%d\n",cmd);
    switch (cmd)
    {
        case DEF_GPIO_DIR_OUT:
                printk("***dir out***cmd=%d,arg=%d\n", cmd, (unsigned)arg);
                gpio_dir_output(arg);
                break;
        case DEF_GPIO_DIR_IN:
                printk("***dir in***cmd=%d,arg=%d\n", cmd, (unsigned)arg);
                gpio_dir_input(arg);
                break;
        case DEF_GPIO_SET_DATA:
        case DEF_GPIO_CLR_DATA:
                printk("*** cmd=%d\n", cmd);
                if(cmd == DEF_GPIO_SET_DATA)
                {
                    gpio_set_value(arg, 1);
                }
                else
                {
                    gpio_set_value(arg, 0);
                }
                break;
        case DEF_GPIO_GET_DATA:
                printk("\t*** cmd=%d\n", cmd);
                ret = gpio_get_value(arg);
                //ret = gpio_get_in_data_reg(arg);
                //gpio_get_in_data_reg(arg);
                //printk("\t*** ret=%d\n", ret);
                break;
    }

    return ret;
}

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;		/* We're now ready for our next caller */

	/* 
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module. 
	 */
	module_put(THIS_MODULE);

	return 0;
}




// MODULE_LICENSE("GPL"); // 不加会出现 Unknown symbol __class_create
MODULE_LICENSE("Dual BSD/GPL");


/*
// 查看信息
$ modinfo chardev.ko

// 查看major号是否被占用
$ cat /proc/devices

// 加载内核模块
$ sudo insmod chardev.ko

// 查看内核打印信息, 获取动态分配的 major number
$ dmesg

// 列出已加载内核模块
$ lsmod

// 测试设备操作
$ sudo cat /dev/chardev
$ sudo echo "hi" > /dev/chardev

// 为了让普通用户操作该设备，添加udev规则
$ sudo vim /etc/udev/rules.d/99-my-udev.rules
KERNEL=="chardev", NAME="chardev", OWNER="root", MODE="0666"
$ ls -l /dev/chardev
$ cat /dev/chardev

// ls /sys/class/会看到我们创建的类，ls /sys/class/led_class/会看到我们在类下创建的设备

交叉编译
$ make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi-

*/

