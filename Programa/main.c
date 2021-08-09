#include <__cross_studio_io.h>
#include "gpio/gpio.h"
#include "lcd/lcd.h"
int cont=0;
void main (void)
{
  gpio_init();
  lcd_init();
  lcd_print_str_xy(1,1,"OLA TURMA !");
  lcd_print_str_xy(5,2,"!@#$%&*");
  while(1)
  {
    lcd_print_int_xy(13,1,cont);
    cont++;
    if(cont==500)
    {
      lcd_print_str_xy(1,1,"                ");
      lcd_print_str_xy(1,2,"                ");
    }
  }
}