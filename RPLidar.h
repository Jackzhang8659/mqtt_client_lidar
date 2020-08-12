#ifndef RPLIDAR_HPP_
#define RPLIDAR_HPP_

#include <ti/drivers/UART.h>
#include <ti/drivers/PWM.h>

#include "RPLidarDriver/inc/rptypes.h"
#include "RPLidarDriver/inc/rplidar_cmd.h"
#include <FreeRTOS.h>
#include <task.h>
#include "ti_drivers_config.h"
#include <time.h>

struct RPLidarMeasurement
{
    float distance;
    float angle;
    uint8_t quality;
    bool  startBit;
};

class RPLidar
{
public:
    enum {
        RPLIDAR_SERIAL_BAUDRATE = 115200,
        RPLIDAR_DEFAULT_TIMEOUT = 500,
    };

    RPLidar();
    ~RPLidar();

    // open the given serial interface and try to connect to the RPLIDAR
    void begin();

    // close the currently opened serial interface
    void end();

    // check whether the serial interface is opened
    bool isOpen();

    // ask the RPLIDAR for its health info
    u_result getHealth(rplidar_response_device_health_t & healthinfo, __u32 timeout = RPLIDAR_DEFAULT_TIMEOUT);

    // ask the RPLIDAR for its device info like the serial number
    u_result getDeviceInfo(rplidar_response_device_info_t & info, __u32 timeout = RPLIDAR_DEFAULT_TIMEOUT);

    // stop the measurement operation
    u_result stop();

    // start the measurement operation
    u_result startScan(bool force = false, __u32 timeout = RPLIDAR_DEFAULT_TIMEOUT*2);

    // wait for one sample point to arrive
    u_result waitPoint(__u32 timeout = RPLIDAR_DEFAULT_TIMEOUT);

    // retrieve currently received sample point
    const RPLidarMeasurement & getCurrentPoint()
    {
        return _currentMeasurement;
    }

    // start motor
    void startMotor();

    // stop motor
    void stopMotor();

    // return uart buffer count
    int getUcount();

    // clean uart buffer
    void cleanBuffer();

//    void readPointToQueue(LIDAR_QUEUE queue);


protected:
    u_result _sendCommand(__u8 cmd, const void * payload, size_t payloadsize);
    u_result _waitResponseHeader(rplidar_ans_header_t * header, __u32 timeout);
    void _handleUartBuffer();
    TickType_t _millis();

protected:
    RPLidarMeasurement _currentMeasurement;

    UART_Params uartParams;
    UART_Handle uart;

    PWM_Params pwmParams;
    PWM_Handle motor;

};
#endif /* RPLIDAR_HPP_ */
