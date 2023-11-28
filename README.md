# III-Robot机器人STM32驱动端

**目录**

1. 外设分配 [Peripherals](https://github.com/yunke120/III-Robot-STM32-CMAKE#Peripherals)
2. 通信方式 [MicroROS](https://github.com/yunke120/microros-test.git)
3. 参考链接 [Reference](https://github.com/yunke120/III-Robot-STM32-CMAKE#Reference)

更多说明参考手册[Manuals](https://github.com/yunke120/III-Robot-STM32-CMAKE/tree/main/Manuals)



## Peripherals

| 外设名称 |      对应引脚      |         说明          | 配置                  |
| :------: | :----------------: | :-------------------: | --------------------- |
|  USART1  |      PA9,PA10      |    与Raspberry通信    | 115200,8,1,N          |
|  USART2  |      PA2,PA3       |     电源管理模块      | 9600,8,1,N            |
|  USART3  |      PD8,PD9       |       调试串口        | 115200,8,1,N          |
|          |                    |                       |                       |
|   TIM1   |  Timebase Source   |   开启FreeRTOS需要    |                       |
|   TIM2   |      PA15,PB3      |      编码器接口       | 双边沿触发            |
|   TIM3   |      PA6,PA7       |      编码器接口       | 双边沿触发            |
|   TIM4   |      PB6,PB7       |      编码器接口       | 双边沿触发            |
|   TIM5   |      PA0,PA1       |      编码器接口       | 双边沿触发            |
|   TIM8   |  PC6,PC7,PC8,PC9   | PWM输出，调节电机转速 | arr=16799,psc=0,10KHZ |
|          |                    |                       |                       |
| ~~ADC1~~ |    ~~PB0(IN8)~~    |   ~~测量输入电压~~    | ~~DMA2 Stream0~~      |
|   ADC1   |     PC1(IN11)      |      测量MQ2输出      | DMA2 Stream0          |
|          |                    |                       |                       |
|          |     PA13,PA15      |      SWD下载接口      |                       |
|          |     PB12,PB13      |    右前轮控制引脚     |                       |
|          |      PA8,PC12      |    左前轮控制引脚     |                       |
|          |      PB14,PC0      |    右后轮控制引脚     |                       |
|          |     PD10,PD12      |    左后轮控制引脚     |                       |
|          |   PD13,PD14,PD15   |        三色LED        |                       |
|          |        PD11        |        蜂鸣器         |                       |
|          |  PD4,PD5,PD6,PD7   |      OLED(软SPI)      |                       |
|          |      PB8,PB9       |    MPU6050(软IIC)     |                       |
|   总计   | (18+40)/100=58/100 |                       |                       |



## Reference
 - [std_msgs Msg/Srv Documentation](https://docs.ros.org/en/melodic/api/std_msgs/html/index-msg.html)
 - [Handling messages memory in micro-ROS](https://micro.ros.org/docs/tutorials/advanced/handling_type_memory/)
 - [micro-ROS-demos](https://github.com/micro-ROS/micro-ROS-demos/tree/humble)

