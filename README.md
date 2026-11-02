# 电控组第二次培训作业

- **王宇浩**
- ***20253007225***

## 实现思路

三颗 LED 接在 PB0 / PB1 / PB10（本板 PB2 未引出），配成推挽输出。
每 500ms 只点亮一盏，其余熄灭，循环 0 → 1 → 2 → 0。
引脚不写裸数字，统一走 `LED0_Pin` / `LED0_GPIO_Port` 这类宏。

## 加分项：非阻塞延时

没有用 `HAL_Delay()`，也没有开定时器中断。SysTick 只当"现在几点"用：

```c
STK_LOAD = 7999u;   /* 8MHz HSI -> 1ms */
STK_VAL  = 0u;
STK_CTRL = 5u;      /* CLKSOURCE=1, ENABLE=1, 不开中断 */

if (STK_CTRL & (1u << 16))   /* COUNTFLAG，读 CTRL 自动清零 */
    ms++;
```

主循环只比较时间差，到点才动一次，其余时间立刻返回：

```c
if (ms - last >= STEP_MS) {
    last += STEP_MS;
    GPIOB->ODR = (GPIOB->ODR & ~((1u << LED0_Pin) | (1u << LED1_Pin) | (1u << LED2_Pin)))
               | (cur == 0 ? (1u << LED0_Pin)
                          : (cur == 1 ? (1u << LED1_Pin) : (1u << LED2_Pin)));
    cur = (cur == 2) ? 0 : cur + 1;
}
```

三个好处：不阻塞、不加硬件、`unsigned` 无符号相减天然抗溢出。

## 工程与验收

- CLion + arm-none-eabi-gcc（CMake 工程，用 CLion 打开本目录即可）
- 烧录：串口 ISP（CH340，115200 8E1），BOOT0=1 进 bootloader，烧完拨回 0
- 验收：CLion 编译通过 → 烧录 → 三灯 500ms 流水
