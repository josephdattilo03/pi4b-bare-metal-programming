// a peripheral described in this document as being at legacy address
// 0x7Enn_nnnn is available in the 35-bit address space at 0x4_7Enn_nnnn, and
// visible to the ARM at 0x0_FEnn_nnnn if Low Peripheral mode is enabled.
#define GPIO_BASE 0xFE200000
#define GPFSELBASE ((volatile unsigned int *)(GPIO_BASE))
#define GPSETBASE ((volatile unsigned int *)(GPIO_BASE + 0x1c))
#define GPCLRBASE ((volatile unsigned int *)(GPIO_BASE + 0x28))

// Mini UART (UART1 / AUX) on the BCM2711 — base at 0xFE215000.
// With enable_uart=1 in config.txt the core clock is locked to 250MHz.
#define AUX_BASE (GPIO_BASE + 0x15000)
#define AUX_ENABLES ((volatile unsigned int *)(AUX_BASE + 0x04))
#define AUX_MU_IO ((volatile unsigned int *)(AUX_BASE + 0x40))
#define AUX_MU_IER ((volatile unsigned int *)(AUX_BASE + 0x44))
#define AUX_MU_IIR ((volatile unsigned int *)(AUX_BASE + 0x48))
#define AUX_MU_LCR ((volatile unsigned int *)(AUX_BASE + 0x4c))
#define AUX_MU_MCR ((volatile unsigned int *)(AUX_BASE + 0x50))
#define AUX_MU_LSR ((volatile unsigned int *)(AUX_BASE + 0x54))
#define AUX_MU_CNTL ((volatile unsigned int *)(AUX_BASE + 0x60))
#define AUX_MU_BAUD ((volatile unsigned int *)(AUX_BASE + 0x68))

typedef enum {
  GPIO_FUNC_INPUT = 0b000,  // 0
  GPIO_FUNC_OUTPUT = 0b001, // 1
  GPIO_FUNC_ALT0 = 0b100,   // 4
  GPIO_FUNC_ALT1 = 0b101,   // 5
  GPIO_FUNC_ALT2 = 0b110,   // 6
  GPIO_FUNC_ALT3 = 0b111,   // 7
  GPIO_FUNC_ALT4 = 0b011,   // 3
  GPIO_FUNC_ALT5 = 0b010    // 2
} GPIOFunction;

typedef enum { GPCLR, GPSET } GPIOWriteOption;

void gpio_set_function(unsigned int pin, GPIOFunction func) {
  unsigned int reg_index = pin / 10;
  unsigned int bit_pos = (pin % 10) * 3;
  unsigned int val = GPFSELBASE[reg_index];

  val &= ~(0b111u << bit_pos);
  val |= ((unsigned int)func & 0b111u) << bit_pos;
  GPFSELBASE[reg_index] = val;
}
void delay(unsigned int cycles) {
  for (volatile unsigned int i = 0; i < cycles; i++)
    ;
}

void gpio_write(unsigned int pin, GPIOWriteOption option) {
  unsigned int reg_index = pin / 32;
  unsigned int bit_pos = pin % 32;

  if (option) {
    GPSETBASE[reg_index] = (1u << bit_pos);
  } else {
    GPCLRBASE[reg_index] = (1u << bit_pos);
  }
}
void uart_init(void) {
  // GPIO14 = TXD1, GPIO15 = RXD1 require ALT5.
  gpio_set_function(14, GPIO_FUNC_ALT5);
  gpio_set_function(15, GPIO_FUNC_ALT5);

  *AUX_ENABLES |= 1; // enable the mini UART
  *AUX_MU_CNTL = 0;  // disable TX/RX while we configure
  *AUX_MU_IER = 0;   // no interrupts
  *AUX_MU_LCR = 3;   // 8-bit mode
  *AUX_MU_MCR = 0;
  *AUX_MU_IIR = 0xc6;  // clear the FIFOs
  *AUX_MU_BAUD = 270;  // 115200 baud at a 250MHz core clock
  *AUX_MU_CNTL = 3;    // enable TX + RX
}

void uart_putc(char c) {
  while (!(*AUX_MU_LSR & 0x20)) // wait until the TX FIFO can accept a byte
    ;
  *AUX_MU_IO = (unsigned int)c;
}

void uart_puts(const char *s) {
  while (*s) {
    if (*s == '\n')
      uart_putc('\r');
    uart_putc(*s++);
  }
}

int main() {
  uart_init();
  uart_puts("hello from main\n");

  gpio_set_function(18, GPIO_FUNC_OUTPUT);

  while (1) {
    gpio_write(18, GPSET);
    uart_puts("alive, gpio18 high\n");
    delay(20000000);

    gpio_write(18, GPCLR);
    uart_puts("alive, gpio18 low\n");
    delay(20000000);
  }
}
