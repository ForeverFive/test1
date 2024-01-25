// SPDX-License-Identifier: GPL-2-Clause
// ���������� ����糿 GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
MODULE_AUTHOR("IO-15 IrynaSydorchenko");
// ����� ������ ������
MODULE_DESCRIPTION("Simple chrono counter for my lab");
// ���� ���� ������
MODULE_LICENSE("Dual BSD/GPL");
// ����� ������ ��� ������
static uint chrono_counter = 1;
// ���������� �� ����������� ����� chrono_counter
module_param(chrono_counter, uint, 0444);
// �����, �� chrono_counter � ���������� ������, ��������� ��� ����������
MODULE_PARM_DESC(chrono_counter, "counter");
// ������� ����� ��������� chrono_counter
struct my_struct {
	struct list_head tlist;
	ktime_t time;
};
// ���������� ��������� my_struct, ��� ������ ������ �� ���
static LIST_HEAD(my_list_head);
// ����������� ������ ������ my_list_head
static int __init hello_init(void)
{
	if (chrono_counter == 0) {
		printk(KERN_WARNING "Success! The parameter's 0\n");
		// ������� ����������� � ����� KERN_WARNING, ���� chrono_counter == 0
	} else if (chrono_counter >= 5 && chrono_counter <= 10) {
		printk(KERN_WARNING "Success! The parameter's between 5 and 10\n");
		// ������� ����������� � ����� KERN_WARNING, ���� chrono_counter >= 5 �� <= 10
	} else if (chrono_counter > 10) {
		printk(KERN_WARNING "Fail! The parameter's > 10)\n");
		// ������� ����������� � ����� KERN_WARNING, ���� chrono_counter > 10
		return -EINVAL;
		// ��������� ������� -EINVAL

	}
	int i;
	// ���������� ����� i
	for (i = 0; i < chrono_counter; i++) {
		struct my_struct *ms = NULL;
		// ���������� ��������� �� ��������� my_struct
		ms = kmalloc(sizeof(struct my_struct), GFP_KERNEL);
		// �������� ���'�� ��� ��������� my_struct � ������������� kmalloc
		ms->time = ktime_get();
		// ����� ��������� ���� �� ���� my_struct
		list_add_tail(&ms->tlist, &my_list_head);
		// ��������� �������� �� ������ �����
		printk(KERN_INFO "Hello, world!\n");
		// ��������� ����������� � ����� KERN_INFO
	}
	return 0;
	// ���������� 0, �� ������ ������ ���������� ������� �����������
}
static void __exit hello_exit(void)
{
	int count = 0;
	// ���������� ����� count
	struct my_struct *t, *tmp;
	// ���������� ��������� �� ��������� my_struct
	list_for_each_entry_safe(t, tmp, &my_list_head, tlist) {
		pr_info("i: %d; ktime = %lld\n", count++, ktime_to_ns(t->time));
		// ��������� ���������� ��� ������� ������ �� ��� � ������������
		list_del(&t->tlist);
		// ��������� �������� � ������
		kfree(t);
		// ��������� ������� ���'��
	}
	printk(KERN_INFO "Success! Module unloaded\n");
	// ��������� ����������� � ����� KERN_INFO
}
module_init(hello_init);
// �������� ������� ����������� ������ ��� ���� Linux
module_exit(hello_exit);
// �������� ������� ���������� ������ ��� ���� Linux
