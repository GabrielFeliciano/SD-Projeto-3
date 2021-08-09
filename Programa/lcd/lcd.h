
//LCD.h


/********************************************************************/
void lcd_cmd (unsigned char cmd);
void init_lcd (void);
void lcd_print_str_xy (unsigned char x, unsigned char y,unsigned char *dado);
void lcd_print_str (unsigned char *dado);
void lcd_print_int (unsigned int dado);
void lcd_print_int_xy (unsigned char x, unsigned char y,unsigned int dado);
void sendnibble(unsigned char dado);
void lcd_char (unsigned char dado);
void delay_lcd (void);