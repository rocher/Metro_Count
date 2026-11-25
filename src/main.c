#include <stdarg.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

void winit_all()
{
    stdio_init_all();
    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
}

void wprintf(const char *fmt, ...)
{
    char buf[256];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    uart_puts(uart0, buf);
}

bool es_simulador_wokwi(void)
{
    const uint pin_deteccio = 28; // GP28
    gpio_init(pin_deteccio);
    gpio_set_dir(pin_deteccio, GPIO_IN);
    gpio_pull_down(pin_deteccio);
    sleep_ms(2);
    return gpio_get(pin_deteccio) != 0;
}

int main()
{
    winit_all();

    wprintf("Es simulador wokwi? %s\r\n", es_simulador_wokwi() ? "Sí" : "No");

    int i = 0;
    while (true)
    {
        wprintf("Hello, Wokwi (%d)\r\n", i);
        sleep_ms(1500);
        i++;
    }
}
