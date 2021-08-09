/********************************************************************/
/*  FUNCOES CONTROLE DE DISPLAY LCD 16X2                            */
/* DEVICE:STM32F103Cx                                               */
/*                                                                  */
/* HARDWARE :                                             	    */
/* LCD_D4 = PB.0                                                    */
/* LCD_D5 = PB.1                                                    */
/* LCD_D6 = PB.2                                                    */
/* LCD_D7 = PB.3                                                    */
/* LCD_RS = PB.4                                                    */
/* LCD_EN = PB.5                                                    */
/********************************************************************/

#include "../gpio/gpio.h"
#include "lcd.h"

//display LCD
#define LCD_EN_ON       gpio_pin_write(GPIOB,5,1)
#define LCD_EN_OFF      gpio_pin_write(GPIOB,5,0)
#define LCD_RS_ON       gpio_pin_write(GPIOB,4,1)
#define LCD_RS_OFF      gpio_pin_write(GPIOB,4,0)
#define LCD_D0_ON       gpio_pin_write(GPIOB,0,1)
#define LCD_D0_OFF      gpio_pin_write(GPIOB,0,0)
#define LCD_D1_ON       gpio_pin_write(GPIOB,1,1)
#define LCD_D1_OFF      gpio_pin_write(GPIOB,1,0)
#define LCD_D2_ON       gpio_pin_write(GPIOB,2,1)
#define LCD_D2_OFF      gpio_pin_write(GPIOB,2,0)
#define LCD_D3_ON       gpio_pin_write(GPIOB,3,1)
#define LCD_D3_OFF      gpio_pin_write(GPIOB,3,0)
#define LCD_DATA_OFF    gpio_pin_write(GPIOB,0,0);gpio_pin_write(GPIOB,1,0);gpio_pin_write(GPIOB,2,0);gpio_pin_write(GPIOB,3,0)
#define LCD_DATA_DIR    gpio_pin_mode(GPIOB,0,gpio_mode_output_PP_2MHz);gpio_pin_mode(GPIOB,1,gpio_mode_output_PP_2MHz);gpio_pin_mode(GPIOB,2,gpio_mode_output_PP_2MHz);gpio_pin_mode(GPIOB,3,gpio_mode_output_PP_2MHz)
#define LCD_CTRL_DIR    gpio_pin_mode(GPIOB,4,gpio_mode_output_PP_2MHz);gpio_pin_mode(GPIOB,5,gpio_mode_output_PP_2MHz)

/********************************************************************/

/*******************************FUNCOES******************************/
/***********ROTINA PARA TRABALHAR COM 4 BITS NO DISPLAY**************/
void sendnibble(unsigned char dado)
{
  LCD_DATA_OFF;
  if ((dado&0x01)==0x01) LCD_D0_ON;   //atualiza valor de dado
  if ((dado&0x02)==0x02) LCD_D1_ON;   //atualiza valor de dado
  if ((dado&0x04)==0x04) LCD_D2_ON;   //atualiza valor de dado
  if ((dado&0x08)==0x08) LCD_D3_ON;   //atualiza valor de dado
  LCD_EN_ON;
  delay_lcd();
  LCD_EN_OFF;
}
/********************************************************************/
/******************INICIALIZA O DISPLAY******************************/
void lcd_init (void)
{
  LCD_DATA_DIR;
  LCD_CTRL_DIR;
  LCD_EN_OFF;
  LCD_RS_OFF;
  sendnibble(0x30>>4);
  delay_lcd();
  sendnibble(0x30>>4);
  delay_lcd();
  sendnibble(0x30>>4);
  delay_lcd();
  sendnibble(0x20>>4);
  delay_lcd();
  lcd_cmd(0x28);
  delay_lcd();
  lcd_cmd(0x08);
  delay_lcd();
  lcd_cmd(0x0C);
  delay_lcd();
  lcd_cmd(0x01);
  delay_lcd();
}
/********************************************************************/
/*******************ATRASO PARA CONFIGURAR O DISPLAY*****************/
void delay_lcd (void)
{
  unsigned int i;
  for (i=0;i<200;i++)
  {
    i=i;
  }
}
/********************************************************************/
/********ENVIA UMA MENSAGEM PARA O DISPLAY NA POSICAO X,Y************/
void lcd_print_str_xy (unsigned char x, unsigned char y,unsigned char *dado)
{
  unsigned char pos;
  pos=x-1;
  if (y==1) 
  {
    pos=pos+0x80;
    lcd_cmd(pos);
    lcd_print_str(dado); 
  }
  else if(y==2)  
  {
    pos=pos+0xc0;
    lcd_cmd(pos);
    lcd_print_str(dado); 
  }
             
}
/********************************************************************/
/*******************ENVIA UMA MENSAGEM AO DISPLAY********************/
void lcd_print_str (unsigned char *dado)
{
  while (*dado != 0)  
  {
    lcd_char(*dado);
    dado++;
  }
}
/********************************************************************/
/*****************ENVIA UM INTEIRO PARA O DISPLAY********************/
void lcd_print_int (unsigned int dado)
{
  if(dado>=10000) lcd_char((dado/10000)+0x30);
  if(dado>=1000) lcd_char(((dado%10000)/1000)+0x30);
  if(dado>=100)  lcd_char((((dado%10000)%1000)/100)+0x30);
  if(dado>=10)   lcd_char(((((dado%10000)%1000)%100)/10)+0x30);
  lcd_char(((((dado%10000)%1000)%100)%10)+0x30);
}
/********************************************************************/
/*********ENVIA UM INTEIRO PARA O DISPLAY NAS POSICOES X e Y*********/
void lcd_print_int_xy (unsigned char x,unsigned char y,unsigned int dado)
{
  unsigned char pos;
  pos=x-1;
  if (y==1) 
  {
    pos=pos+0x80;
    lcd_cmd(pos);
  }
  else      
  {
    pos=pos+0xc0;
    lcd_cmd(pos);
  }
  lcd_print_int(dado);
}

/**********************ENVIA UM DADO AO DISPLAY**********************/
void lcd_char (unsigned char dado)
{
  LCD_RS_ON;
  sendnibble(dado>>4);
  sendnibble(dado&0x0F);
}
/********************************************************************/
/****************ENVIA UM COMANDO PARA O DISPLAY*********************/
void lcd_cmd (unsigned char cmd)
{
  LCD_RS_OFF;
  sendnibble(cmd>>4);           //uso de 4 bits apenas
  sendnibble(cmd&0x0F);         //escreve 4 bits
}
/********************************************************************/
/**********************ESCOLHE MODO CURSOR **********************/
void lcd_cursor_mode (unsigned char modo)
{
  if(modo==0) lcd_cmd(0x0c);   //apagado
  else if(modo==1) lcd_cmd(0x0F);     //ligado
}
/********************************************************************/
/**********************ESCOLHE POSICAO CURSOR **********************/
void lcd_cursor_pos (unsigned char x,unsigned char y)
{
  unsigned char pos;
  pos=x-1;
  if (y==1) 
  {
    pos=pos+0x80;
    lcd_cmd(pos);
  }
  else      
  {
    pos=pos+0xc0;
    lcd_cmd(pos);
  }  
}
/********************************************************************/