# Firmware / HAL / drivers

Application code calls HAL (`gpio_write`, `uart_send`, `spi_transfer`, …). HAL calls drivers. Drivers call `ml_mcu_read32/write32` only.

Radio firmware (`firmware/transceiver`) programs RADIO MMIO and issues CMD TX/RX/CFG. Side effects go through the bridge callback installed at boot.
