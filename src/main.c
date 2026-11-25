#include <stdarg.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

bool IS_SIMULATION = false;
const int SIMULATION_PIN = 28; // GP28

bool is_wokwi(void)
{
    gpio_init(SIMULATION_PIN);
    gpio_set_dir(SIMULATION_PIN, GPIO_IN);
    gpio_pull_down(SIMULATION_PIN);
    sleep_ms(2);
    return gpio_get(SIMULATION_PIN) != 0;
}

void winit_all()
{
    stdio_init_all();
    IS_SIMULATION = is_wokwi();

    // if (IS_SIMULATION)
    {
        // init uart0 with 115200 baud rate, 8 data bits, 1 stop bit, no
        // parity
        uart_init(uart0, 115200);
        uart_set_translate_crlf(uart0, true);
        uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
        // connect uart0 to GPIO pins 0 and 1
        gpio_set_function(0, GPIO_FUNC_UART);
        gpio_set_function(1, GPIO_FUNC_UART);
    }
}

void wprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    if (IS_SIMULATION)
    {
        char buf[256];
        vsnprintf(buf, sizeof(buf), fmt, args);
        uart_puts(uart0, buf);
    }
    else
    {
        printf(fmt, args);
    }

    va_end(args);
}

int main()
{
    winit_all();

    wprintf("Es simulador wokwi? %s\n", IS_SIMULATION ? "Sí" : "No");

    int i = 0;
    while (true)
    {
        wprintf("Hello, Wokwi (%d)\n", i);
        sleep_ms(1500);
        i++;
    }
}
