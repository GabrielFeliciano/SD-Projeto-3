/**
 * @file    gpio.c
 * @author  Łukasz Łaguna
 * @brief   GPIO driver.
 *
 * @details The functions are used to configure the GPIO in the STM32F103
 * microcontroller.
 */

#include "gpio.h"

void gpio_pin_mode(GPIO_TypeDef * gpio, uint8_t pin, gpio_mode_t mode)
{
    pin *= 4;

    if (pin < 32)
    {
        gpio->CRL &= ~((uint32_t)(0x0f << pin));
        gpio->CRL |= (uint32_t)(mode << pin);
    }
    else
    {
        pin -= 32;
        gpio->CRH &= ~((uint32_t)(0x0f << pin));
        gpio->CRH |= (uint32_t)(mode << pin);
    }
}

void gpio_pin_write(GPIO_TypeDef * gpio, uint8_t pin, gpio_pupd_t pupd)
{
    if (pupd)
    {
        gpio->ODR |= (uint32_t)(1<<pin);
    }
    else
    {
        gpio->ODR &= ~(uint32_t)(1<<pin);
    }
}

uint8_t gpio_pin_read(GPIO_TypeDef *gpio, uint8_t pin)
{
  if(gpio->IDR&(uint32_t)(1<<pin)) 
  {
    return 1;
  }
  else  
  {
    return 0;
  }
}


void gpio_init(void)
{
    // Enable clock for GPIO ports 
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //clock GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //clock GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //clock GPIOC
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN; //clock GPIOD

    //Config output pins
    // Led GPIOC 13
    gpio_pin_mode(GPIOB, 0, gpio_mode_output_PP_2MHz);

    //Config input pins
    // GPIOA 0
    gpio_pin_mode(GPIOA, 0, gpio_mode_input_pupd);  //input pull-up pull-down
    gpio_pin_write(GPIOA, 0, 1);          //input pull-up

}