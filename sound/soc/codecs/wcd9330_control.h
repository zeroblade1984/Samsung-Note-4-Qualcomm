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

extern struct snd_soc_codec *wcd9330_codec;

extern bool hp_digigain_con;
extern bool hp_toggle;
extern u32 hp_digigain;

/* HP Digital Defaults */
#define TOMTOM_A_CDC_RXX_VOL_MASK		0x7F
#define TOMTOM_A_CDC_RX_VOL_CTL_B2_CTL_DEF	0x00

int tomtom_write(struct snd_soc_codec *codec, unsigned int reg,
				unsigned int value);
unsigned int tomtom_read(struct snd_soc_codec *codec,
				unsigned int reg);
