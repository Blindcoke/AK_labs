#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
MODULE_AUTHOR("Yehor Lashkul <elwhizzkids@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int count = 1;  // Доданий параметр зі значенням за замовчуванням 1
struct my_data {
	struct list_head list;
	ktime_t timestamp;
};
static LIST_HEAD(my_list_head);
module_param(count, uint, S_IRUGO); // Визначення параметра
MODULE_PARM_DESC(count, "Number of times to print 'Hello, world!'");

static void add_event_to_list(void)
{
	ktime_t current_time = ktime_get();
	struct my_data *entry = kmalloc(sizeof(struct my_data), GFP_KERNEL);
	if (!entry) {
		pr_err("Memory allocation failed\n");
	return;
    }
	entry->timestamp = current_time;
	list_add(&entry->list, &my_list_head);
}

static int __init hello_init(void)
{
	if (count == 0 || (count >= 5 && count <= 10)) {
		pr_warn("Invalid parameter value. Using the default value of 1.\n");
		count = 1;
	} else if (count > 10) {
		pr_err("Invalid parameter value. Module will not load.\n");
		return -EINVAL; // Повертаємо помилку, модуль не завантажиться
	}
	int i;
	for (i = 0; i < count; i++) {
		printk(KERN_EMERG "Hello, world!\n");
		add_event_to_list();
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct my_data *entry, *temp;
	list_for_each_entry_safe(entry, temp, &my_list_head, list) {
		printk(KERN_EMERG "Timestamp: %lld ns\n", ktime_to_ns(entry->timestamp));
		list_del(&entry->list);
		kfree(entry);
    }
}

module_init(hello_init);
module_exit(hello_exit);
