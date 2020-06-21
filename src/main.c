/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <stdio.h>
// #include <sys/printk.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   300

/* The devicetree node identifier for the "led0" alias. */
#define LEDS_NODE 	DT_ALIAS(leds)
#define LED0_GREEN 	DT_ALIAS(led0)
#define LED1_RED 	DT_ALIAS(led1)
#define LED2_BLUE 	DT_ALIAS(led2)
#define BUTTON0 	DT_ALIAS(sw0)

#if DT_NODE_HAS_STATUS(LED0_GREEN, okay)
#define LED0	DT_GPIO_LABEL(LED0_GREEN, gpios)
#define PIN0	DT_GPIO_PIN(LED0_GREEN, gpios)
#if DT_PHA_HAS_CELL(LED0_GREEN, gpios, flags)
#define FLAGS0	DT_GPIO_FLAGS(LED0_GREEN, gpios)
#endif
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN	0
#endif

#if DT_NODE_HAS_STATUS(LED1_RED, okay)
#define LED1	DT_GPIO_LABEL(LED1_RED, gpios)
#define PIN1	DT_GPIO_PIN(LED1_RED, gpios)
#if DT_PHA_HAS_CELL(LED1_RED, gpios, flags)
#define FLAGS1	DT_GPIO_FLAGS(LED1_RED, gpios)
#endif
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led1 devicetree alias is not defined"
#define LED1	""
#define PIN	1
#endif

#if DT_NODE_HAS_STATUS(LED2_BLUE, okay)
#define LED2	DT_GPIO_LABEL(LED2_BLUE, gpios)
#define PIN2	DT_GPIO_PIN(LED2_BLUE, gpios)
#if DT_PHA_HAS_CELL(LED2_BLUE, gpios, flags)
#define FLAGS2	DT_GPIO_FLAGS(LED2_BLUE, gpios)
#endif
#else
// A build error here means your board isn't set up to blink an LED. 
#error "Unsupported board: led2 devicetree alias is not defined"
#define LED2	""
#define PIN	2
#endif

#if DT_NODE_HAS_STATUS(BUTTON0, okay)
#define LABEL_BUTTON	DT_GPIO_LABEL(BUTTON0, gpios)
#define PIN_BUTTON		DT_GPIO_PIN(BUTTON0, gpios)
#if DT_PHA_HAS_CELL(BUTTON0, gpios, flags)
#define FLAGS_BUTTON	DT_GPIO_FLAGS(BUTTON0, gpios)
#endif
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define BUTTON0	""
#define PIN_BUTTON	3
#endif


#ifndef FLAGS0
#define FLAGS	0
#endif

#ifndef FLAGS1
#define FLAGS	0
#endif

#ifndef FLAGS2
#define FLAGS	0
#endif

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))

const gpio_pin_t leds_pin[] = 	{ 
	PIN0, 
	PIN1, 
	PIN2, 
};

const gpio_flags_t leds_flags[] = { 
	GPIO_OUTPUT_ACTIVE | FLAGS0, 
	GPIO_OUTPUT_ACTIVE | FLAGS1, 
	GPIO_OUTPUT_ACTIVE | FLAGS2,
};

const char* const leds_alias[] = {
	LED0, 
	LED1, 
	LED2, 
};

// const gpio_pin_t 	btn_pin[] 	= { PIN_BUTTON, };
// const gpio_flags_t 	btn_flags[] = { GPIO_INPUT | FLAGS_BUTTON, };
// const char* const 	btn_alias[] = { LABEL_BUTTON };

void main(void)
{
	struct device *leds_dev[2];
	// struct device *btn_dev;

	printk( "teste\r\n" );

	uint8_t led_mask = 1;
	int ret;

	uint8_t dev_count;

	for (dev_count = 0; dev_count < ARRAY_LEN(leds_pin); dev_count++)
	{
		leds_dev[dev_count] = device_get_binding(leds_alias[dev_count]);
		if (leds_dev == NULL) {
			return;
		}

		ret = gpio_pin_configure(leds_dev[dev_count], leds_pin[dev_count], leds_flags[dev_count]);
		if (ret < 0) {
			return;
		}
	}

	// btn_dev = device_get_binding( btn_alias[0] );
	// if ( NULL == btn_dev ){
	// 	return;
	// }

	// ret = gpio_pin_configure( btn_dev, btn_pin[0], btn_flags[0] );
	// if (ret < 0) {
	// 	return;
	// }


	while (1) 
	{
		// bool btn_val = gpio_pin_get( btn_dev, btn_pin[0] );

		// if (btn_val)
		// {
		// 	gpio_pin_set(leds_dev[0], leds_pin[0], 1);	
		// } 
		// else
		// {
		// 	gpio_pin_set(leds_dev[0], leds_pin[0], 0);	
		// }

		for (dev_count = 0; dev_count < ARRAY_LEN(leds_pin); dev_count++)
		{
			if (led_mask & (0x01 << dev_count))
			{
				gpio_pin_set(leds_dev[dev_count], leds_pin[dev_count], 1);	
			} 
			else
			{
				gpio_pin_set(leds_dev[dev_count], leds_pin[dev_count], 0);	
			}
		}

		led_mask <<= 1;
		if (led_mask & (1 << ARRAY_LEN(leds_pin)) )
		{
			led_mask = 1;
		}

		printf("teste\n");

		k_msleep(SLEEP_TIME_MS);
	}
}
