/*Author:Satya Mehta
 * Kernel Timer module
 * Prints name and count every designated period
 * 
*/


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>

struct timer_list mytimer;

static char *n = "Satya";
static long time = 0; /*in seconds*/
int count = 0;

void myfunc(struct timer_list * s)
{
	count++;
	printk("My name is %s and count is %d\n", n, count);
	if(time == 0)
	{
		mytimer.expires = (unsigned long)(jiffies + HZ/2);
		add_timer(&mytimer);
	}
	else
	{
		
		mytimer.expires = (unsigned long)(jiffies + time*HZ);
		add_timer(&mytimer);
		
	}
}

static void timer_init(void)
{
	timer_setup(&mytimer, myfunc, 0);

	if(time == 0)
	{
		mytimer.expires = (unsigned long)(jiffies + HZ/2);
		add_timer(&mytimer);
	}
	else
	{
		
		mytimer.expires = (unsigned long)(jiffies + time*HZ);
		add_timer(&mytimer);
		
	}
	printk("Timer Started\r\n");
}

static int satya_init(void)
{
	printk("Timer module\r\n");
	timer_init();
	return 0;
}

static void satya_exit(void)
{
	printk("Module Exiting\r\n");
	del_timer_sync(&mytimer);	
}

module_init(satya_init);
module_exit(satya_exit);
module_param(n,charp, 0);
module_param(time, long, 0);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Satya Mehta");
MODULE_DESCRIPTION("Simple Timer Module");
MODULE_VERSION("1.0");
