/* 
 * WCD9330 Tomtom Audio Mod Controls(TA-MOD)
 * 
 * 	Author: Cezar Rey Templonuevo <zeroblade1984@gmail.com>
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

#include <linux/module.h>
#include <linux/kobject.h>
#include <sound/soc.h>
#include <linux/mfd/wcd9xxx/wcd9330_registers.h>
#include "wcd9330_control.h"

/* Variables Section */
bool hp_digigain_con = false;
bool hp_toggle = false;
u32 hp_digigain = 0x02;

struct snd_soc_codec *wcd9330_codec;

static ssize_t hp_digigain_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	u32 usr = hp_digigain & TOMTOM_A_CDC_RXX_VOL_MASK;

	sprintf(buf, "Status: %s Value: %#04x\n", hp_digigain_con ? "on" : "off", usr);

	return strlen(buf);
}

static ssize_t hp_digigain_store(struct kobject *kobj, 
				 struct kobj_attribute *attr,
				 const char *buf, size_t count)
{
	u32 usr;

	if (sysfs_streq(buf, "on")) {
		hp_digigain_con = true;

		if (hp_toggle) {
			tomtom_write(wcd9330_codec, 
				TOMTOM_A_CDC_RX1_VOL_CTL_B2_CTL, hp_digigain);
			tomtom_write(wcd9330_codec, 
				TOMTOM_A_CDC_RX2_VOL_CTL_B2_CTL, hp_digigain);
		}

		return count;
	}

	if (sysfs_streq(buf, "off")) {
		hp_digigain_con = false;

		tomtom_write(wcd9330_codec,
			TOMTOM_A_CDC_RX1_VOL_CTL_B2_CTL,
			TOMTOM_A_CDC_RX_VOL_CTL_B2_CTL_DEF);

		tomtom_write(wcd9330_codec,
			TOMTOM_A_CDC_RX2_VOL_CTL_B2_CTL,
			TOMTOM_A_CDC_RX_VOL_CTL_B2_CTL_DEF);

		return count;
	}

	if (sscanf(buf, "%d", &usr)) {
		if (usr > TOMTOM_A_CDC_RXX_VOL_MASK)
			return -EINVAL;

		hp_digigain = usr;

		if (hp_toggle && hp_digigain_con) {
			tomtom_write(wcd9330_codec, 
				TOMTOM_A_CDC_RX1_VOL_CTL_B2_CTL, hp_digigain);

			tomtom_write(wcd9330_codec, 
				TOMTOM_A_CDC_RX2_VOL_CTL_B2_CTL, hp_digigain);
		}

		return count;
	}

	return count;
}
static struct kobj_attribute hp_digigain_interface = 
	__ATTR(hp_digigain, 0644, hp_digigain_show, hp_digigain_store);

/* sysfs parts */
static struct attribute *tomtom_control_attrs[] = {
	NULL,
};

static struct attribute_group tomtom_control_interface_group = {
	.attrs = tomtom_control_attrs,
};

static struct attribute *tomtom_headset_attrs[] = {
	&hp_digigain_interface.attr,
	NULL,
};

static struct attribute_group tomtom_headset_interface_group = {
	.attrs = tomtom_headset_attrs,
	.name  = "headset",
};

static struct kobject *tomtom_control_kobject;

static int __init tomtom_control_init(void)
{
	int ret;

	tomtom_control_kobject = kobject_create_and_add("tomtom", kernel_kobj);
	if (!tomtom_control_kobject) {
		pr_err("Tomtom Control: Failed to create kobject interface\n");
	}

	ret = sysfs_create_group(tomtom_control_kobject, &tomtom_control_interface_group);
	if (ret) {
		kobject_put(tomtom_control_kobject);
	}

	ret = sysfs_create_group(tomtom_control_kobject, &tomtom_headset_interface_group);
	if (ret) {
		pr_err("Tomtom Control: Failed to create sysfs group(s)\n");
	}

	return ret;
}

static void __exit tomtom_control_exit(void)
{
	kobject_put(tomtom_control_kobject);
	return;
}

module_init(tomtom_control_init);
module_exit(tomtom_control_exit);

MODULE_DESCRIPTION("Tomtom Audio Codec controller");
MODULE_AUTHOR("Cezar Rey Templonuevo");
MODULE_LICENSE("GPL v2");
