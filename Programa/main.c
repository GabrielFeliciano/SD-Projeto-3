// Imports
#include <__cross_studio_io.h>
#include "gpio/gpio.h"
#include "lcd/lcd.h"

// Macros
#define SYSCLK 8000000
#define ASYNCCLK 2000000

// Global Variables
int speed = 1;
unsigned int ticks = 0;

unsigned int relationClk = SYSCLK / ASYNCCLK;
unsigned int ticket_to_a_second(void) { return ticks / relationClk; }
unsigned int get_seconds (void) { return (ticket_to_a_second()) % 60; }
unsigned int get_minutes (void) { return (ticket_to_a_second() / 60) % 60; }
unsigned int get_hours (void) { return (ticket_to_a_second() / 60 / 24) % 24; }

unsigned char clock_character_1[];
unsigned char clock_character_2[];
unsigned char clock_character_3[];
unsigned char clock_character_4[];
int* clock_spritesheet[4];

unsigned char clock_character[] = {
    0b00000,
    0b01110,
    0b01110,
    0b10101,
    0b10101,
    0b10001,
    0b01110,
    0b00000
};

// Functions

void createCustomCharacter (unsigned char *pattern, const char location) {  
    lcd_cmd(0x40 + (location * 8)); // Send the Address of CGRAM
    for (int i = 0; i < 8; i++) {
        // debug_printf(pattern[i]);
        lcd_char(pattern[i]); // Pass the bytes of pattern on LCD 
    }
}

void createCustomSpritesheet (int pattern[], const char location, int size) {
    for (int i = 0; i < size; i++) {
        createCustomCharacter(clock_spritesheet, location + 0);
    }
}

char* get_time() {
    static char* buffer[8];
    sprintf(buffer, "%02d:%02d:%02d\n", get_hours(), get_minutes(), get_seconds());
    return buffer;
}

void preconfigure(void) {
    // SysTick_Config(SYSCLK / speed);
    SysTick_Config(ASYNCCLK);
    gpio_init();
    lcd_init();
}

unsigned short clock_animation_frame = 1;

void main(void) {
    preconfigure();

    lcd_cursor_pos(1, 1);
    createCustomCharacter(clock_character_1, 1);
    createCustomCharacter(clock_character_2, 2);
    createCustomCharacter(clock_character_3, 3);
    createCustomCharacter(clock_character_4, 4);

    while (1) {
        lcd_cursor_pos(6, 1);
        lcd_char(clock_animation_frame);
        lcd_print_str_xy(7, 1, "Clock");
        lcd_print_str_xy(5, 2, get_time());
    }
}

void SysTick_Handler (void) { 
    ticks++;
    clock_animation_frame = clock_animation_frame >= 4 
    ? 1 
    : clock_animation_frame + 1;
}

// Sprites

unsigned char clock_character_1[] = {
    0b00000,
    0b01110,
    0b01110,
    0b10101,
    0b10101,
    0b10001,
    0b01110,
    0b00000};

unsigned char clock_character_2[] = {
    0b00000,
    0b01110,
    0b01110,
    0b10001,
    0b10111,
    0b10001,
    0b01110,
    0b00000};

unsigned char clock_character_3[] = {
    0b00000,
    0b01110,
    0b01110,
    0b10001,
    0b10101,
    0b10101,
    0b01110,
    0b00000};

unsigned char clock_character_4[] = {
    0b00000,
    0b01110,
    0b01110,
    0b10001,
    0b11101,
    0b10001,
    0b01110,
    0b00000};

// unsigned char* clock_spritesheet[4] = {
//     clock_character_1,
//     clock_character_2,
//     clock_character_3,
//     clock_character_4
// };