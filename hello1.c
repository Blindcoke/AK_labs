#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <hello1.h>

MODULE_AUTHOR("Yehor Lashkul <elwhizzkids@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training - Part 2");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int count = 1;

module_param(count, uint, S_IRUGO);
MODULE_PARM_DESC(count, "Number of times to print 'Hello, world!'");

struct my_data {
	struct list_head list;
	ktime_t timestamp;
	ktime_t print_duration;
};

static LIST_HEAD(my_list_head);

void print_hello(unsigned int count)
{
	int i;
	for (i = 0; i < count; i++) {
		ktime_t before = ktime_get();
		pr_info("Hello, world!\n");
		ktime_t after = ktime_get();
		struct my_data *entry = kmalloc(sizeof(struct my_data), GFP_KERNEL);
	if (!entry) {
		pr_err("Memory allocation failed\n");
	    return;
	}
	entry->print_duration = ktime_sub(after, before);
	list_add(&entry->list, &my_list_head);
	}
}

static int __init hello1_init(void)
{
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

static void __exit hello1_exit(void)
{
	struct my_data *entry, *temp;
	list_for_each_entry_safe(entry, temp, &my_list_head, list) {
		pr_info("Час на друк: %lld ns\n", ktime_to_ns(entry->print_duration));
		list_del(&entry->list);
		kfree(entry);
	}
}

module_init(hello1_init);
module_exit(hello1_exit);
EXPORT_SYMBOL(print_hello);

