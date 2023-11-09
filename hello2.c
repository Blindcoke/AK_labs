#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <hello1.h>

MODULE_AUTHOR("Yehor Lashkul");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training - Part 2");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int count = 1;  
struct my_data {
	struct list_head list;
	ktime_t timestamp;
	ktime_t print_duration;
};

static LIST_HEAD(my_list_head);

module_param(count, uint, S_IRUGO);
MODULE_PARM_DESC(count, "Number of times to call print_hello from hello1");

static int __init hello2_init(void)

{
	pr_info("Initializing hello2 module\n");
	if (count == 0 || (count >= 5 && count <= 10)) {
		pr_warn("Invalid parameter value. Using the default value of 1.\n");
		count = 1;
	} else if (count > 10) {
		pr_err("Invalid parameter value. Module will not load.\n");
	return -EINVAL;
	}
	print_hello(count);
	return 0;
}


static void __exit hello2_exit(void)
{
	struct my_data *entry, *temp;
	list_for_each_entry_safe(entry, temp, &my_list_head, list) {
		pr_info("Час на друк: %lld ns\n", ktime_to_ns(entry->print_duration));
		list_del(&entry->list);
		kfree(entry);
		pr_info("Exiting hello2 module\n");
	}
}

module_init(hello2_init);
module_exit(hello2_exit);
