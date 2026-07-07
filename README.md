# STM32F407xx Bare-Metal Peripheral Drivers

Register-level peripheral drivers for the STM32F407 (ARM Cortex-M4), written from scratch — **no HAL, no CMSIS device pack, no generated code**. Every memory map entry, register struct, bit-position macro, and driver routine was written by hand from the STM32F4xx reference manual (RM0090) and the STM32F407 datasheet, as a deep dive into how these peripherals actually work at the silicon level.

Developed and tested on the STM32F407G-DISC1 Discovery board.

## Drivers

| Peripheral | Files | Capabilities |
|---|---|---|
| **GPIO** | `stm32f407xx_gpio_driver.[ch]` | Input/output/alternate-function/analog modes, output type & speed, pull-up/pull-down, **EXTI interrupt modes** (rising / falling / both edges, routed through SYSCFG), NVIC enable + priority configuration |
| **SPI** | `stm32f407xx_spi_driver.[ch]` | Master/slave, full-duplex / half-duplex / simplex-RX bus configs, 8- and 16-bit frames, all baud prescalers, CPOL/CPHA, software or hardware slave management (SSM/SSI/SSOE), **blocking and interrupt-driven TX/RX**, overrun-error handling |
| **I2C** | `stm32f407xx_i2c_driver.[ch]` | Master **and slave** mode, standard/fast mode with CCR + TRISE timing computed from the *live* RCC clock tree (not hardcoded), repeated start, ACK management, **blocking and interrupt-driven transfers**, separate event and error IRQ handlers (SB, ADDR, BTF, STOPF, TXE, RXNE, BERR, ARLO, AF, OVR, timeout) |
| **USART** | *planned* | Next on the roadmap |

## Design

The code is layered the way a vendor driver library would be, but built from the ground up:

```
┌─────────────────────────────────────────────┐
│  Application        (Src/ examples)         │   configures a handle, calls the API,
├─────────────────────────────────────────────┤   overrides weak event callbacks
│  Driver layer       (Drivers/Src, /Inc)     │   handle + config structs, blocking APIs,
├─────────────────────────────────────────────┤   interrupt state machines
│  Device header      (stm32f407xx.h)         │   memory map, register structs, RCC
└─────────────────────────────────────────────┘   enable/reset macros, NVIC, IRQ numbers
```

- **Handle/config pattern** — each peripheral instance is driven through a handle struct pairing a register-map pointer with user configuration (`GPIO_Handle_t`, `SPI_Handle_t`, `I2C_Handle_t`).
- **Non-blocking transfers** — the `_IT` APIs save buffer state into the handle, enable the peripheral's interrupt sources, and return immediately; the ISR-side state machine feeds the data register and fires an application callback (`SPI_ApplicationEventCallback` / `I2C_ApplicationEventCallback`, declared `weak`) on completion or error.
- **Freestanding dependencies only** — the drivers include nothing beyond `<stdint.h>` and `<stddef.h>`; `printf` appears only in example applications (retargeted to ITM/SWO in `syscalls.c`).

Example of the API surface (from `001_led_toggle.c`):

```c
GPIO_Handle_t led = {0};
led.pGPIO_x                            = GPIOD;
led.GPIO_pin_config.GPIO_pin_number    = GPIO_PIN_12;
led.GPIO_pin_config.GPIO_pin_mode      = GPIO_MODE_OUT;
led.GPIO_pin_config.GPIO_pin_speed     = GPIO_OP_SPEED_HI;
led.GPIO_pin_config.GPIO_pin_op_type   = GPIO_OP_TYPE_PP;
GPIO_init(&led);

while (1) {
    GPIO_toggle_output_pin(GPIOD, GPIO_PIN_12);
    delay();
}
```

## Repository layout

```
├── Drivers/
│   ├── Inc/            device header + driver headers
│   └── Src/            driver implementations
├── Src/                example applications (one main() each) + syscalls/sysmem
├── Startup/            startup_stm32f407vgtx.s (vector table, reset handler)
├── STM32F407VGTX_FLASH.ld / _RAM.ld
└── .project / .cproject   STM32CubeIDE project files
```

## Examples

Each file in `Src/` is a self-contained application demonstrating one driver feature. The SPI and I2C examples talk to an Arduino Uno running a matching peer sketch (common ground, I2C lines with pull-ups).

| Example | Demonstrates |
|---|---|
| `001_led_toggle` | GPIO output — onboard LED (PD12) |
| `002_led_button` | GPIO input polling — user button (PA0) |
| `003_external_led_button` | External LED + button wiring |
| `004_interrupt_led` | EXTI interrupt: button edge triggers ISR, toggles LED |
| `SPI_out` | SPI blocking transmit |
| `SPI_out_arduino` | SPI master TX → Arduino slave |
| `SPI_rx_arduino` | Full command/response protocol over SPI (LED control, analog read, ID read) |
| `SPI_interrupt_arduino` | Interrupt-driven SPI reception |
| `010_i2c_master_tx_testing` | I2C master blocking TX |
| `011_i2c_master_read` | I2C master blocking RX — command / length / payload protocol using repeated start |
| `012_i2c_master_rx_it` | Same protocol, fully interrupt-driven |
| `013_I2C_slave` | **STM32 as I2C slave** — serves data to an Arduino master via command codes |
| `014_slave_strlen_Tx` | I2C slave streaming a longer payload with length prefix |

Pin assignments used by the examples:

| Bus | Pins |
|---|---|
| I2C1 | PB6 = SCL, PB7 = SDA (AF4, open-drain + pull-up, 100 kHz) |
| SPI2 | PB12 = NSS, PB13 = SCLK, PB14 = MISO, PB15 = MOSI (AF5) |
| Misc | PD12 onboard LED, PA0 user button |

## Building & running

The repo is an STM32CubeIDE project:

1. Clone, then in CubeIDE: **File → Import → Existing Projects into Workspace**.
2. Exactly one example provides `main()` at a time — all others are excluded from the build (right-click a file → **Resource Configurations → Exclude from Build** to switch). `013_I2C_slave.c` is currently active.
3. Build and flash to the Discovery board over its onboard ST-LINK.
4. Example `printf` output goes to ITM stimulus port 0 — view it in CubeIDE's **SWV ITM Data Console** (SWO enabled, core clock matching).

## Using the drivers in your own project

The driver layer has no dependency on the examples or the IDE: copy `Drivers/Inc` and `Drivers/Src` into any STM32F407 project, add `Drivers/Inc` to the include path, and include the driver header you need (each pulls in `stm32f407xx.h` itself).

## Roadmap

- USART driver (blocking + interrupt-driven, same layering)
- RCC driver — replace the fixed HSI/HSE assumptions in clock lookup with full PLL support
- DMA-backed transfers for SPI/I2C

## Acknowledgements

Built while working through the [FastBit Embedded Brain Academy](https://fastbitlab.com/) *Mastering Microcontroller and Embedded Driver Development* course — the drivers here were written by hand against RM0090 and the F407 datasheet as part of that coursework.

## License

MIT — see [LICENSE](LICENSE).
