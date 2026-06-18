// a peripheral described in this document as being at legacy address
// 0x7Enn_nnnn is available in the 35-bit address space at 0x4_7Enn_nnnn, and
// visible to the ARM at 0x0_FEnn_nnnn if Low Peripheral mode is enabled.
#define GPIO_BASE 0xFE200000
#define GPFSELBASE ((volatile unsigned int *)(GPIO_BASE))
#define GPSETBASE ((volatile unsigned int *)(GPIO_BASE + 0x1c))
#define GPCLRBASE ((volatile unsigned int *)(GPIO_BASE + 0x28))

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
int main() {
  gpio_set_function(18, GPIO_FUNC_OUTPUT);
  while (1) {
    gpio_write(18, GPSET);
    delay(200000000);

    gpio_write(18, GPCLR);
    delay(200000000);
  }
}
