/*
 * Copyright (c) 2017, thewisenerd <thewisenerd@protonmail.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef MSM_LED_FLASH_LM3646_DUMMY_H
#define MSM_LED_FLASH_LM3646_DUMMY_H

#include "../msm_led_flash.h"

extern struct msm_led_flash_ctrl_t fctrl;

extern struct msm_camera_i2c_reg_array lm3646_off_array[];
extern struct msm_camera_i2c_reg_array lm3646_release_array[];
extern struct msm_camera_i2c_reg_array lm3646_low_array[];
extern struct msm_camera_i2c_reg_array lm3646_high_array[];

struct __lm3646_dummy_data {
	enum msm_camera_led_config_t state;
	u8 brightness;
};

extern struct __lm3646_dummy_data lm3646_dummy_data;

#endif // MSM_LED_FLASH_LM3646_DUMMY_H
