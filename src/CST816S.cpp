/*****************************************************************************
 * | File      	:   CST816S.c
 * | Author      :   Waveshare team
 * | Function    :   Hardware underlying interface
 * | Info        :
 *                Used to shield the underlying layers of each master
 *                and enhance portability
 *----------------
 * |	This version:   V1.0
 * | Date        :   2022-12-02
 * | Info        :   Basic version
 *
 ******************************************************************************/
#include "CST816S.h"
#include "hardware/gpio.h"
#include <cstdint>


auto CST816S::I2C_Write(uint8_t reg, uint8_t value) -> void
{
    uint8_t data[2] = {reg, value};
    i2c_write_blocking(I2C_PORT, CST816_ADDR, data, 2, false);
}
auto CST816S::I2C_Read(uint8_t reg) -> uint8_t
{
    uint8_t buf;
    i2c_write_blocking(I2C_PORT,CST816_ADDR,&reg,1,true);
    i2c_read_blocking(I2C_PORT,CST816_ADDR,&buf,1,false);
    return buf;
}

auto CST816S::WhoAmI() -> uint8_t
{
    //printf("whoami");
    uint8_t cid = I2C_Read(CST816_ChipID);
    //printf(" = %02x\n",cid);
    return ( cid == 0xB5);
}

auto CST816S::Reset() -> void
{
    gpio_put(Touch_RST_PIN, 0);
    sleep_ms(120);
    gpio_put(Touch_RST_PIN, 1);
    sleep_ms(120);
}

auto CST816S::Read_Revision() -> uint8_t
{
    return I2C_Read(CST816_FwVersion);
}

auto CST816S::Wake_up() -> void
{
    gpio_put(Touch_RST_PIN, 0);
    sleep_ms(10);
    gpio_put(Touch_RST_PIN, 1);
    sleep_ms(50);
    I2C_Write(CST816_DisAutoSleep, 0x01);
}

auto CST816S::Stop_Sleep() -> void
{
    I2C_Write(CST816_DisAutoSleep, 0x01);
}

auto CST816S::Set_Mode(uint8_t mode) -> void
{
    if (mode == CST816S_Point_Mode)
    {
        //
        I2C_Write(CST816_IrqCtl, 0x41);
    }
    else if (mode == CST816S_Gesture_Mode)
    {
        I2C_Write(CST816_IrqCtl, 0X11);
        I2C_Write(CST816_MotionMask, 0x01);
    }
    else
    {
        I2C_Write(CST816_IrqCtl, 0X71);
    }

}



auto CST816S::init(uint8_t mode) -> uint8_t
{
    uint8_t bRet, Rev;
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(DEV_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(DEV_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(DEV_SDA_PIN);
    gpio_pull_up(DEV_SCL_PIN);
    gpio_init(Touch_INT_PIN);
    gpio_pull_up(Touch_INT_PIN);
    gpio_set_dir(Touch_INT_PIN,GPIO_IN);
    gpio_set_irq_enabled_with_callback(Touch_INT_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                       true, gpio_irq_callback);
    gpio_init(Touch_RST_PIN);
    gpio_set_dir(Touch_RST_PIN,GPIO_OUT);
    Reset();
    //printf("CST816T reset done\n");
    bRet = WhoAmI();
    if (bRet)
    {
        //printf("Success:Detected CST816T.\r\n");
        Rev = Read_Revision();
        //printf("CST816T Revision = %d\r\n", Rev);
        Stop_Sleep();
    }
    else
    {
        printf("Error: Not Detected CST816T.\r\n");
        return false;
    }

    Set_Mode(mode);
    Touch_CTS816.x_point = 0;
    Touch_CTS816.y_point = 0;
    I2C_Write(CST816_IrqPluseWidth, 0x01);
    I2C_Write(CST816_NorScanPer, 0x01);

    Touch_CTS816.mode = static_cast<CST816S_Mode>(mode);

    return true;
}

auto CST816S::get_Point() -> CST816S_Touch
{
    uint8_t x_point_h, x_point_l, y_point_h, y_point_l;
    // CST816S_Wake_up();

    x_point_h = I2C_Read(CST816_XposH);
    x_point_l = I2C_Read(CST816_XposL);
    y_point_h = I2C_Read(CST816_YposH);
    y_point_l = I2C_Read(CST816_YposL);

    Touch_CTS816.x_point = ((x_point_h & 0x0f) << 8) + x_point_l;
    Touch_CTS816.y_point = ((y_point_h & 0x0f) << 8) + y_point_l;
    Touch_CTS816.interrupt = interrupt_happened;
    interrupt_happened = false;
    return Touch_CTS816;
}

auto CST816S::get_Gesture(void) -> uint8_t
{
    uint8_t gesture;
    gesture=I2C_Read(CST816_GestureID);
    return gesture;
}
