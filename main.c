// INTERRUPTS:
// System timer
// Arm side timer
// How do I enable an interupt:
// in 4480 we said interupts just happen but they are not just going to happen
// in hardware
// How do I clear all the garbage to read that an interupt happened
// What happens when the timer goes off or you get a ctrl c
//
// Check the arm manual to see what addresses are jumped to
//
// Research: types of interupts
//
// what regesters are going to be saved when i jump to the interrupt address
//
// Research: installing the interrupt vector, which is a vector with
// instructions called based on which kind of interrupts recieved
//
// Modes: mode changes on interrupt
//
// Research: irq interrupts (including timer) research how to find what type of
// irq was called and how to clear it before jumping back (use inline assembly)
//
// Manually install the interupt vector via main or another linked file (linked
// preferred)
//
// Figure out memcpy
//
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

#define LED_PIN 18u    // indicator led
#define SYNC_LED 23u   // synchronous exception (svc #0)
#define IRQ_LED 24u    // timer delivered as IRQ
#define FIQ_LED 25u    // timer delivered as FIQ (secure world timer)
#define SERROR_LED 12u // system error (fault)

#define D_LONG 18000000u
#define D_MED 7000000u
#define D_SHORT 2500000u

static void led_blink(unsigned int pin, int times, unsigned int on_cycles,
                      unsigned int off_cycles) {
  for (int i = 0; i < times; i++) {
    gpio_write(pin, GPSET);
    delay(on_cycles);
    gpio_write(pin, GPCLR);
    delay(off_cycles);
  }
}

static inline void mmio_write(unsigned long addr, unsigned int val) {
  *(volatile unsigned int *)addr = val;
}
static inline unsigned int mmio_read(unsigned long addr) {
  return *(volatile unsigned int *)addr;
}

// ARM generic timer
static inline unsigned long read_cntfrq(void) {
  unsigned long v;
  __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(v));
  return v;
}
static inline void write_cntp_tval(unsigned long v) {
  __asm__ volatile("msr cntp_tval_el0, %0" ::"r"(v));
}
static inline void write_cntp_ctl(unsigned long v) {
  __asm__ volatile("msr cntp_ctl_el0, %0" ::"r"(v));
}

// we run at EL2. The target EL of an *asynchronous* exception is chosen by
// HCR_EL2.{IMO,FMO,AMO}; left at their reset value of 0 these exceptions target
// EL1, and an exception can never be taken to an EL lower than the current one,
// so the timer interrupt just stays pending and is never delivered. Setting
// these bits makes IRQ/FIQ/SError be taken here at EL2. (Synchronous exceptions
// such as `svc` ignore this routing)
//
// The daifclr then clears both the I (irq) and F (fiq) masks.
static inline void enable_irq_fiq(void) {
  unsigned long hcr;
  __asm__ volatile("mrs %0, hcr_el2" : "=r"(hcr));
  hcr |= (1UL << 3)    // FMO - physical FIQ taken to EL2
         | (1UL << 4)  // IMO - physical IRQ taken to EL2
         | (1UL << 5); // AMO - physical SError taken to EL2
  __asm__ volatile("msr hcr_el2, %0" ::"r"(hcr));
  __asm__ volatile("isb");
  __asm__ volatile("msr daifclr, #3");
}

// --- GIC-400 (BCM2711 / Pi 4) ----------------------------------------------
#define GICD_BASE 0xFF841000UL
#define GICC_BASE 0xFF842000UL
#define GICD_CTLR (GICD_BASE + 0x000)
#define GICD_ISENABLER0 (GICD_BASE + 0x100)
#define GICD_IPRIORITYR (GICD_BASE + 0x400)
#define GICC_CTLR (GICC_BASE + 0x000)
#define GICC_PMR (GICC_BASE + 0x004)
#define GICC_IAR (GICC_BASE + 0x00C)
#define GICC_EOIR (GICC_BASE + 0x010)
#define TIMER_PPI 30u // EL1 physical timer (CNTP) interrupt id

static void gic_init(void) {
  mmio_write(GICD_CTLR, 1);
  mmio_write(GICC_PMR, 0xF0);
  mmio_write(GICC_CTLR, 1);

  ((volatile unsigned char *)GICD_IPRIORITYR)[TIMER_PPI] = 0xA0;

  mmio_write(GICD_ISENABLER0, (1u << TIMER_PPI));
}

static void timer_start(void) {
  unsigned long freq = read_cntfrq();
  write_cntp_tval(freq);
  write_cntp_ctl(1);
}

// acknowledge + clear a timer interrupt at the GIC and the timer itself.
// returns the acknowledged interrupt id (the IAR value).
static unsigned int timer_ack_and_stop(void) {
  unsigned int iar = mmio_read(GICC_IAR);
  unsigned int intid = iar & 0x3FFu;
  if (intid == TIMER_PPI) {
    write_cntp_ctl(0); // disable the timer so it stops re-asserting
  }
  if (intid < 1020u) {
    mmio_write(GICC_EOIR, iar); // end-of-interrupt for real ids
  }
  return intid;
}

volatile int sync_fired = 0;
volatile int irq_fired = 0;
volatile int fiq_fired = 0;

void handle_sync(unsigned long kind, unsigned long esr, unsigned long elr) {
  (void)kind;
  (void)esr;
  (void)elr;
  sync_fired = 1;
  gpio_write(SYNC_LED, GPSET); // sync exception -> light its dedicated LED
}

void handle_irq(unsigned long kind) {
  (void)kind;
  unsigned int intid = timer_ack_and_stop();
  if (intid == TIMER_PPI) {
    irq_fired = 1;
    gpio_write(IRQ_LED, GPSET); // IRQ -> light its dedicated LED
  }
}

void handle_fiq(unsigned long kind) {
  (void)kind;
  unsigned int intid = timer_ack_and_stop();
  if (intid == TIMER_PPI) {
    fiq_fired = 1;
    gpio_write(FIQ_LED, GPSET); // FIQ -> light its dedicated LED
  }
}

void handle_serror(unsigned long kind, unsigned long esr, unsigned long elr) {
  (void)kind;
  (void)esr;
  (void)elr;
  for (;;)
    led_blink(SERROR_LED, 1, D_SHORT, D_SHORT); // fault -> fast-blink its LED
}

// vector_table_init is defined in vectors.S; it writes VBAR_EL2.
extern void vector_table_init(void);

int main() {
  gpio_set_function(LED_PIN, GPIO_FUNC_OUTPUT);
  gpio_set_function(SYNC_LED, GPIO_FUNC_OUTPUT);
  gpio_set_function(IRQ_LED, GPIO_FUNC_OUTPUT);
  gpio_set_function(FIQ_LED, GPIO_FUNC_OUTPUT);
  gpio_set_function(SERROR_LED, GPIO_FUNC_OUTPUT);

  vector_table_init();
  led_blink(LED_PIN, 1, D_LONG, D_MED);

  // the sync handler lights SYNC_LED; a short indicator blink confirms the
  // sync_fired variable changed back in main
  __asm__ volatile("svc #0");
  if (sync_fired)
    led_blink(LED_PIN, 1, D_SHORT, D_LONG);

  gic_init();
  timer_start();
  enable_irq_fiq();

  // indicator blinks (short on, long off) while waiting; once the timer fires,
  // IRQ_LED or FIQ_LED lights up to show which path delivered it
  while (!irq_fired && !fiq_fired) {
    led_blink(LED_PIN, 1, D_SHORT, D_LONG);
  }

  // even on/off heartbeat on the indicator pin = "testing finished, still
  // alive"
  while (1) {
    led_blink(LED_PIN, 1, D_MED, D_MED);
  }
}
