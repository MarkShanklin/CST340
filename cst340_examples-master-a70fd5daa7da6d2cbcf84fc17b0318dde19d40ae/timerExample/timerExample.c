#include <linux/init.h>
#include <linux/module.h>
// For High Resolution Timer
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

// Macros
#define MS_TO_NS(x) (x * 1E6L)

// Declare a structure for timer
static struct hrtimer hr_timer;

enum hrtimer_restart my_hrtimer_callback( struct hrtimer *timer )
{
	printk(KERN_INFO "Timer Example: Timer Callback called (%ld).\n", 
			jiffies);

	return HRTIMER_NORESTART;
}

static int __init mytimer_init(void)
{
	// Variables
	ktime_t ktime;
	unsigned long delay_in_ms = 200L;

	printk(KERN_INFO "Timer Example: Driver Installing\n");

	ktime = ktime_set( 0, MS_TO_NS(delay_in_ms) );
	hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	hr_timer.function = &my_hrtimer_callback;
	printk( "Timer Example: Starting timer to fire in %ldms (%ld)\n", 
			delay_in_ms, jiffies );
	hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL );

	return 0;
}

static void mytimer_exit(void)
{
	int ret;

	ret = hrtimer_cancel(&hr_timer);
	if (ret)
		printk(KERN_INFO "Timer Example: Timer was still in use...\n");

	printk(KERN_INFO "Timer Example: Driver Released\n");

	return;
}

module_init(mytimer_init);
module_exit(mytimer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Troy Scevers <troy.scevers@oit.edu>");
MODULE_DESCRIPTION("Timer Example Module");
MODULE_VERSION("timer");

