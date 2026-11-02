/* 电控组第二次培训作业 —— 三灯流水灯（非阻塞）
 * 目标：STM32F103C8T6 最小系统板
 * 引脚：PB0 / PB1 / PB10（本板 PB2 未引出，PB2 即 BOOT1）
 * 时基：SysTick 1ms 轮询，主循环不阻塞
 */

/* ---- CubeMX User Label 的等价写法：代码只调宏，不写裸数字 ---- */
#define LED0_Pin        0u
#define LED0_GPIO_Port  GPIOB
#define LED1_Pin        1u
#define LED1_GPIO_Port  GPIOB
#define LED2_Pin        10u
#define LED2_GPIO_Port  GPIOB

typedef struct {
    volatile unsigned int CRL, CRH, IDR, ODR, BSRR, BRR, LCKR;
} GPIO_TypeDef;

#define GPIOB ((GPIO_TypeDef *)0x40010C00u)

#define RCC_APB2ENR (*(volatile unsigned int *)0x40021018u)
#define STK_CTRL    (*(volatile unsigned int *)0xE000E010u)
#define STK_LOAD    (*(volatile unsigned int *)0xE000E014u)
#define STK_VAL     (*(volatile unsigned int *)0xE000E018u)

#define STEP_MS 500u

int main(void)
{
    unsigned int ms = 0, last = 0, cur = 0;

    RCC_APB2ENR |= 1u << 3;                                   /* GPIOB 时钟 */
    GPIOB->CRL = (GPIOB->CRL & ~0xFFu) | 0x22u;               /* PB0/PB1 推挽 2MHz */
    GPIOB->CRH = (GPIOB->CRH & ~(0xFu << 8)) | (0x2u << 8);   /* PB10 推挽 2MHz */

    STK_LOAD = 7999u;   /* 8MHz HSI -> 1ms */
    STK_VAL  = 0u;
    STK_CTRL = 5u;      /* CLKSOURCE=1, ENABLE=1, 不开中断 */

    for (;;) {
        if (STK_CTRL & (1u << 16))   /* COUNTFLAG，读 CTRL 自动清零 */
            ms++;

        if (ms - last >= STEP_MS) {
            last += STEP_MS;
            GPIOB->ODR = (GPIOB->ODR & ~((1u << LED0_Pin) | (1u << LED1_Pin) | (1u << LED2_Pin)))
                       | (cur == 0 ? (1u << LED0_Pin)
                                  : (cur == 1 ? (1u << LED1_Pin) : (1u << LED2_Pin)));
            cur = (cur == 2) ? 0 : cur + 1;
        }
    }
}
