/* Copyright (c) 2017, thewisenerd <thewisenerd@protonmail.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/etherdevice.h>

#define DRIVER_AUTHOR "thewisenerd <thewisenerd@protonmail.com>"
#define DRIVER_DESC "User-defined MAC Address support for Qualcomm WCNSS Pronto"

#define MAC_ADDRESS_STR "%02x:%02x:%02x:%02x:%02x:%02x"
#define WLAN_MAC_ADDR_SIZE ( 6 )

typedef struct
{
	char bytes[ WLAN_MAC_ADDR_SIZE ];
} macaddr_t;

static macaddr_t macaddr = {{0}};
static unsigned random = 0;

static struct kobject *kobj;

void* get_wcnss_pronto_usermac(void)
{
	if (random)
		eth_random_addr((char *)&macaddr.bytes);

	return &macaddr;
}
EXPORT_SYMBOL(get_wcnss_pronto_usermac);

static ssize_t wcnss_pronto_usermac_macaddr_store(struct kobject *obj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	char tmacAddr[WLAN_MAC_ADDR_SIZE];

	pr_debug("%s: Receive MAC Addr From user space: %s\n", __func__, buf);

	if (WLAN_MAC_ADDR_SIZE != sscanf(buf, MAC_ADDRESS_STR,
		 (unsigned *)&tmacAddr[0], (unsigned *)&tmacAddr[1],
		 (unsigned *)&tmacAddr[2], (unsigned *)&tmacAddr[3],
		 (unsigned *)&tmacAddr[4], (unsigned *)&tmacAddr[5])) {

		pr_err("%s: Failed to Copy MAC\n", __func__);
		return -EINVAL;
	}

	memcpy((void *)macaddr.bytes, tmacAddr, sizeof(macaddr_t));

	pr_info("%s: Write MAC Addr:" MAC_ADDRESS_STR "\n", __func__,
		macaddr.bytes[0], macaddr.bytes[1],
		macaddr.bytes[2], macaddr.bytes[3],
		macaddr.bytes[4], macaddr.bytes[5]);

	return count;
}

static ssize_t wcnss_pronto_usermac_macaddr_show(struct kobject *obj,
		struct kobj_attribute *attr, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, MAC_ADDRESS_STR "\n",
		macaddr.bytes[0], macaddr.bytes[1],
		macaddr.bytes[2], macaddr.bytes[3],
		macaddr.bytes[4], macaddr.bytes[5]);
}

static struct kobj_attribute macaddr_attr = __ATTR(	\
	macaddr,					\
	S_IRUGO | S_IWUSR,				\
	wcnss_pronto_usermac_macaddr_show,		\
	wcnss_pronto_usermac_macaddr_store		\
);

static ssize_t wcnss_pronto_usermac_random_store(struct kobject *obj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned val = 0;

	if ( 1 != sscanf(buf, "%u", &val) ) {
		pr_err("%s: failed to read value!", __func__);
		return -EINVAL;
	}

	random = !!val;

	return count;
}

static ssize_t wcnss_pronto_usermac_random_show(struct kobject *obj,
		struct kobj_attribute *attr, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%u\n", random);
}

static struct kobj_attribute randomize_attr = __ATTR(	\
	randomize,					\
	S_IRUGO | S_IWUSR,				\
	wcnss_pronto_usermac_random_show,		\
	wcnss_pronto_usermac_random_store		\
);

static struct attribute *wcnss_pronto_usermac_attr[] = {
	&macaddr_attr.attr,
	&randomize_attr.attr,
	NULL,
};

static struct attribute_group wcnss_pronto_usermac_grp = {
	.attrs = wcnss_pronto_usermac_attr,
};

static int __init wcnss_pronto_usermac_init(void)
{
	int ret = 0;

	kobj = kobject_create_and_add("wlan", NULL);

	if (unlikely(!kobj)) {
		pr_err("%s: unable to create kobject\n", __func__);
		return -ENOMEM;
	}

	ret = sysfs_create_group(kobj, &wcnss_pronto_usermac_grp);
	if (unlikely(ret)) {
		pr_err("%s: failed to create attributes\n", __func__);
		goto destroy_kobj;
	}
	return 0;

destroy_kobj:
	kobject_put(kobj);

	return ret;
}

static void __exit wcnss_pronto_usermac_exit(void)
{
	sysfs_remove_group(kobj, &wcnss_pronto_usermac_grp);
	kobject_put(kobj);
}

module_init(wcnss_pronto_usermac_init);
module_exit(wcnss_pronto_usermac_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
