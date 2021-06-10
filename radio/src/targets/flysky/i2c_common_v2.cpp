/*
 * This file is part of the libopencm3 project.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "i2c_common_v2.h"

#define RCC_BASE			(PERIPH_BASE_AHB1 + 0x1000)
#define _RCC_REG(i)		MMIO32(RCC_BASE + ((i) >> 5))
#define _RCC_BIT(i)		(1 << ((i) & 0x1f))

#define _REG_BIT(base, bit)		(((base) << 5) + (bit))

enum rcc_periph_clken {
	/* AHB peripherals */
	RCC_DMA		= _REG_BIT(0x14, 0),
	RCC_DMA1	= _REG_BIT(0x14, 0), /* Compatibility alias */
	RCC_DMA2	= _REG_BIT(0x14, 1),
	RCC_SRAM	= _REG_BIT(0x14, 2),
	RCC_FLTIF	= _REG_BIT(0x14, 4),
	RCC_CRC		= _REG_BIT(0x14, 6),
	RCC_GPIOA	= _REG_BIT(0x14, 17),
	RCC_GPIOB	= _REG_BIT(0x14, 18),
	RCC_GPIOC	= _REG_BIT(0x14, 19),
	RCC_GPIOD	= _REG_BIT(0x14, 20),
	RCC_GPIOE	= _REG_BIT(0x14, 21),
	RCC_GPIOF	= _REG_BIT(0x14, 22),
	RCC_TSC		= _REG_BIT(0x14, 24),

	/* APB2 peripherals */
	RCC_SYSCFG_COMP	= _REG_BIT(0x18, 0),
	RCC_USART6	= _REG_BIT(0x18, 5),
	RCC_USART7	= _REG_BIT(0x18, 6),
	RCC_USART8	= _REG_BIT(0x18, 7),
	RCC_ADC		= _REG_BIT(0x18, 9),
	RCC_ADC1	= _REG_BIT(0x18, 9), /* Compatibility alias */
	RCC_TIM1	= _REG_BIT(0x18, 11),
	RCC_SPI1	= _REG_BIT(0x18, 12),
	RCC_USART1	= _REG_BIT(0x18, 14),
	RCC_TIM15	= _REG_BIT(0x18, 16),
	RCC_TIM16	= _REG_BIT(0x18, 17),
	RCC_TIM17	= _REG_BIT(0x18, 18),
	RCC_DBGMCU	= _REG_BIT(0x18, 22),

	/* APB1 peripherals */
	RCC_TIM2	= _REG_BIT(0x1C, 0),
	RCC_TIM3	= _REG_BIT(0x1C, 1),
	RCC_TIM6	= _REG_BIT(0x1C, 4),
	RCC_TIM7	= _REG_BIT(0x1C, 5),
	RCC_TIM14	= _REG_BIT(0x1C, 8),
	RCC_WWDG	= _REG_BIT(0x1C, 11),
	RCC_SPI2	= _REG_BIT(0x1C, 14),
	RCC_USART2	= _REG_BIT(0x1C, 17),
	RCC_USART3	= _REG_BIT(0x1C, 18),
	RCC_USART4	= _REG_BIT(0x1C, 19),
	RCC_USART5	= _REG_BIT(0x1C, 20),
	RCC_I2C1	= _REG_BIT(0x1C, 21),
	RCC_I2C2	= _REG_BIT(0x1C, 22),
	RCC_USB		= _REG_BIT(0x1C, 23),
	RCC_CAN		= _REG_BIT(0x1C, 25),
	RCC_CAN1	= _REG_BIT(0x1C, 25), /* Compatibility alias */
	RCC_CRS		= _REG_BIT(0x1C, 27),
	RCC_PWR		= _REG_BIT(0x1C, 28),
	RCC_DAC		= _REG_BIT(0x1C, 29),
	RCC_DAC1	= _REG_BIT(0x1C, 29), /* Compatibility alias */
	RCC_CEC		= _REG_BIT(0x1C, 30),

	/* Advanced peripherals */
	RCC_RTC		= _REG_BIT(0x20, 15),/* BDCR[15] */
};

enum rcc_periph_rst {
	/* APB2 peripherals */
	RST_SYSCFG	= _REG_BIT(0x0C, 0),
	RST_ADC		= _REG_BIT(0x0C, 9),
	RST_ADC1	= _REG_BIT(0x0C, 9), /* Compatibility alias */
	RST_TIM1	= _REG_BIT(0x0C, 11),
	RST_SPI1	= _REG_BIT(0x0C, 12),
	RST_USART1	= _REG_BIT(0x0C, 14),
	RST_TIM15	= _REG_BIT(0x0C, 16),
	RST_TIM16	= _REG_BIT(0x0C, 17),
	RST_TIM17	= _REG_BIT(0x0C, 18),
	RST_DBGMCU	= _REG_BIT(0x0C, 22),

	/* APB1 peripherals */
	RST_TIM2	= _REG_BIT(0x10, 0),
	RST_TIM3	= _REG_BIT(0x10, 1),
	RST_TIM6	= _REG_BIT(0x10, 4),
	RST_TIM7	= _REG_BIT(0x10, 5),
	RST_TIM14	= _REG_BIT(0x10, 8),
	RST_WWDG	= _REG_BIT(0x10, 11),
	RST_SPI2	= _REG_BIT(0x10, 14),
	RST_USART2	= _REG_BIT(0x10, 17),
	RST_USART3	= _REG_BIT(0x10, 18),
	RST_USART4	= _REG_BIT(0x10, 19),
	RST_I2C1	= _REG_BIT(0x10, 21),
	RST_I2C2	= _REG_BIT(0x10, 22),
	RST_USB		= _REG_BIT(0x10, 23),
	RST_CAN		= _REG_BIT(0x10, 25),
	RST_CAN1	= _REG_BIT(0x10, 25), /* Compatibility alias */
	RST_CRS		= _REG_BIT(0x10, 27),
	RST_PWR		= _REG_BIT(0x10, 28),
	RST_DAC		= _REG_BIT(0x10, 29),
	RST_DAC1	= _REG_BIT(0x10, 29), /* Compatibility alias */
	RST_CEC		= _REG_BIT(0x10, 30),

	/* Advanced peripherals */
	RST_BACKUPDOMAIN = _REG_BIT(0x20, 16),/* BDCR[16] */

	/* AHB peripherals */
	RST_GPIOA	= _REG_BIT(0x28, 17),
	RST_GPIOB	= _REG_BIT(0x28, 18),
	RST_GPIOC	= _REG_BIT(0x28, 19),
	RST_GPIOD	= _REG_BIT(0x28, 20),
	RST_GPIOE	= _REG_BIT(0x28, 21),
	RST_GPIOF	= _REG_BIT(0x28, 22),
	RST_TSC		= _REG_BIT(0x28, 24),
};

void rcc_periph_reset_pulse(enum rcc_periph_rst rst)
{
	_RCC_REG(rst) |= _RCC_BIT(rst);
	_RCC_REG(rst) &= ~_RCC_BIT(rst);
}

/*---------------------------------------------------------------------------*/
/** @brief I2C Reset.
 *
 * The I2C peripheral and all its associated configuration registers are placed
 * in the reset condition. The reset is effected via the RCC peripheral reset
 * system.
 *
 * @param[in] i2c Unsigned int32. I2C peripheral identifier @ref i2c_reg_base.
 */

void i2c_reset(uint32_t i2c)
{
	switch (i2c) {
	case I2C1:
		rcc_periph_reset_pulse(RST_I2C1);
		break;
	case I2C2:
		rcc_periph_reset_pulse(RST_I2C2);
		break;
	default:
		break;
	}
}

/*---------------------------------------------------------------------------*/
/** @brief I2C Peripheral Enable.
 *
 * @param[in] i2c Unsigned int32. I2C register base address @ref i2c_reg_base.
 */

void i2c_peripheral_enable(uint32_t i2c)
{
	I2C_CR1(i2c) |= I2C_CR1_PE;
}

/*---------------------------------------------------------------------------*/
/** @brief I2C Peripheral Disable.
 *
 * This must not be reset while in Master mode until a communication has
 * finished. In Slave mode, the peripheral is disabled only after communication
 * has ended.
 *
 * @param[in] i2c Unsigned int32. I2C register base address @ref i2c_reg_base.
 */

void i2c_peripheral_disable(uint32_t i2c)
{
	I2C_CR1(i2c) &= ~I2C_CR1_PE;
}

/*---------------------------------------------------------------------------*/
/** @brief I2C Send Start Condition.
 *
 * If in Master mode this will cause a restart condition to occur at the end of
 * the current transmission. If in Slave mode, this will initiate a start
 * condition when the current bus activity is completed.
 *
 * @param[in] i2c Unsigned int32. I2C register base address @ref i2c_reg_base.
 */

void i2c_send_start(uint32_t i2c)
{
	I2C_CR2(i2c) |= I2C_CR2_START;
}

/*---------------------------------------------------------------------------*/
/** @brief I2C Send Data.
 *
 * @param[in] i2c Unsigned int32. I2C register base address @ref i2c_reg_base.
 * @param[in] data Unsigned int8. Byte to send.
 */

void i2c_send_data(uint32_t i2c, uint8_t data)
{
	I2C_TXDR(i2c) = data;
}

/*---------------------------------------------------------------------------*/
/** @brief I2C Get Data.
 *
 * @param[in] i2c Unsigned int32. I2C register base address @ref i2c_reg_base.
 */
uint8_t i2c_get_data(uint32_t i2c)
{
	return I2C_RXDR(i2c) & 0xff;
}

void i2c_enable_analog_filter(uint32_t i2c)
{
	I2C_CR1(i2c) &= ~I2C_CR1_ANFOFF;
}

void i2c_disable_analog_filter(uint32_t i2c)
{
	I2C_CR1(i2c) |= I2C_CR1_ANFOFF;
}

/**
 * Set the I2C digital filter.
 * These bits are used to configure the digital noise filter on SDA and
 * SCL input. The digital filter will filter spikes with a length of up
 * to dnf_setting * I2CCLK clocks
 * @param i2c peripheral of interest
 * @param dnf_setting 0 to disable, else 1..15 i2c clocks
 */
void i2c_set_digital_filter(uint32_t i2c, uint8_t dnf_setting)
{
	I2C_CR1(i2c) = (I2C_CR1(i2c) & ~(I2C_CR1_DNF_MASK << I2C_CR1_DNF_SHIFT)) |
		(dnf_setting << I2C_CR1_DNF_SHIFT);
}

/* t_presc= (presc+1)*t_i2cclk */
void i2c_set_prescaler(uint32_t i2c, uint8_t presc)
{
	I2C_TIMINGR(i2c) = (I2C_TIMINGR(i2c) & ~I2C_TIMINGR_PRESC_MASK) |
			   (presc << I2C_TIMINGR_PRESC_SHIFT);
}

void i2c_set_data_setup_time(uint32_t i2c, uint8_t s_time)
{
	I2C_TIMINGR(i2c) = (I2C_TIMINGR(i2c) & ~I2C_TIMINGR_SCLDEL_MASK) |
			   (s_time << I2C_TIMINGR_SCLDEL_SHIFT);
}

void i2c_set_data_hold_time(uint32_t i2c, uint8_t h_time)
{
	I2C_TIMINGR(i2c) = (I2C_TIMINGR(i2c) & ~I2C_TIMINGR_SDADEL_MASK) |
			   (h_time << I2C_TIMINGR_SDADEL_SHIFT);
}

void i2c_set_scl_high_period(uint32_t i2c, uint8_t period)
{
	I2C_TIMINGR(i2c) = (I2C_TIMINGR(i2c) & ~I2C_TIMINGR_SCLH_MASK) |
			   (period << I2C_TIMINGR_SCLH_SHIFT);
}

void i2c_set_scl_low_period(uint32_t i2c, uint8_t period)
{
	I2C_TIMINGR(i2c) = (I2C_TIMINGR(i2c) & ~I2C_TIMINGR_SCLL_MASK) |
			   (period << I2C_TIMINGR_SCLL_SHIFT);
}

void i2c_enable_stretching(uint32_t i2c)
{
	I2C_CR1(i2c) &= ~I2C_CR1_NOSTRETCH;
}

void i2c_disable_stretching(uint32_t i2c)
{
	I2C_CR1(i2c) |= I2C_CR1_NOSTRETCH;
}

void i2c_set_7bit_addr_mode(uint32_t i2c)
{
	I2C_CR2(i2c) &= ~I2C_CR2_ADD10;
}

void i2c_set_7bit_address(uint32_t i2c, uint8_t addr)
{
	I2C_CR2(i2c) = (I2C_CR2(i2c) & ~I2C_CR2_SADD_7BIT_MASK) |
		       ((addr & 0x7F) << I2C_CR2_SADD_7BIT_SHIFT);
}

void i2c_set_write_transfer_dir(uint32_t i2c)
{
	I2C_CR2(i2c) &= ~I2C_CR2_RD_WRN;
}

void i2c_set_read_transfer_dir(uint32_t i2c)
{
	I2C_CR2(i2c) |= I2C_CR2_RD_WRN;
}

void i2c_set_bytes_to_transfer(uint32_t i2c, uint32_t n_bytes)
{
	I2C_CR2(i2c) = (I2C_CR2(i2c) & ~I2C_CR2_NBYTES_MASK) |
		       (n_bytes << I2C_CR2_NBYTES_SHIFT);
}

void i2c_enable_autoend(uint32_t i2c)
{
	I2C_CR2(i2c) |= I2C_CR2_AUTOEND;
}

void i2c_disable_autoend(uint32_t i2c)
{
	I2C_CR2(i2c) &= ~I2C_CR2_AUTOEND;
}

bool i2c_nack(uint32_t i2c)
{
	return (I2C_ISR(i2c) & I2C_ISR_NACKF);
}

bool i2c_transmit_int_status(uint32_t i2c)
{
	return (I2C_ISR(i2c) & I2C_ISR_TXIS);
}

bool i2c_transfer_complete(uint32_t i2c)
{
	return (I2C_ISR(i2c) & I2C_ISR_TC);
}

bool i2c_received_data(uint32_t i2c)
{
	return (I2C_ISR(i2c) & I2C_ISR_RXNE);
}

/**
 * Run a write/read transaction to a given 7bit i2c address
 * If both write & read are provided, the read will use repeated start.
 * Both write and read are optional
 * @param i2c peripheral of choice, eg I2C1
 * @param addr 7 bit i2c device address
 * @param w buffer of data to write
 * @param wn length of w
 * @param r destination buffer to read into
 * @param rn number of bytes to read (r should be at least this long)
 */
void i2c_transfer7(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn, uint8_t *r, size_t rn)
{
	/*  waiting for busy is unnecessary. read the RM */
	if (wn) {
		i2c_set_7bit_address(i2c, addr);
		i2c_set_write_transfer_dir(i2c);
		i2c_set_bytes_to_transfer(i2c, wn);
		if (rn) {
			i2c_disable_autoend(i2c);
		} else {
			i2c_enable_autoend(i2c);
		}
		i2c_send_start(i2c);

		while (wn--) {
			bool wait = true;
			while (wait) {
				if (i2c_transmit_int_status(i2c)) {
					wait = false;
				}
				while (i2c_nack(i2c)); /* FIXME Some error */
			}
			i2c_send_data(i2c, *w++);
		}
		/* not entirely sure this is really necessary.
		 * RM implies it will stall until it can write out the later bits
		 */
		if (rn) {
			while (!i2c_transfer_complete(i2c));
		}
	}

	if (rn) {
		/* Setting transfer properties */
		i2c_set_7bit_address(i2c, addr);
		i2c_set_read_transfer_dir(i2c);
		i2c_set_bytes_to_transfer(i2c, rn);
		/* start transfer */
		i2c_send_start(i2c);
		/* important to do it afterwards to do a proper repeated start! */
		i2c_enable_autoend(i2c);

		for (size_t i = 0; i < rn; i++) {
			while (i2c_received_data(i2c) == 0);
			r[i] = i2c_get_data(i2c);
		}
	}
}


/**
 * Set the i2c communication speed.
 * NOTE: 1MHz mode not yet implemented!
 * Min clock speed: 8MHz for FM, 2Mhz for SM,
 * @param i2c peripheral, eg I2C1
 * @param speed one of the listed speed modes @ref i2c_speeds
 * @param clock_megahz i2c peripheral clock speed in MHz. Usually, rcc_apb1_frequency / 1e6
 */
void i2c_set_speed(uint32_t i2c, enum i2c_speeds speed, uint32_t clock_megahz)
{
	int prescaler;
	switch(speed) {
	case i2c_speed_fmp_1m:
		/* FIXME - add support for this mode! */
		break;
	case i2c_speed_fm_400k:
		/* target 8Mhz input, so tpresc = 125ns */
		prescaler = clock_megahz / 8 - 1;
		i2c_set_prescaler(i2c, prescaler);
		i2c_set_scl_low_period(i2c, 10-1); // 1250ns
		i2c_set_scl_high_period(i2c, 4-1); // 500ns
		i2c_set_data_hold_time(i2c, 3); // 375ns
		i2c_set_data_setup_time(i2c, 4-1); // 500ns
		break;
	default:
		/* fall back to standard mode */
	case i2c_speed_sm_100k:
		/* target 4Mhz input, so tpresc = 250ns */
		prescaler = (clock_megahz / 4) - 1;
		i2c_set_prescaler(i2c, prescaler);
		i2c_set_scl_low_period(i2c, 20-1); // 5usecs
		i2c_set_scl_high_period(i2c, 16-1); // 4usecs
		i2c_set_data_hold_time(i2c, 2); // 0.5usecs
		i2c_set_data_setup_time(i2c, 5-1); // 1.25usecs
		break;
	}
}

/**@}*/
