#include <RPLidar.h>

RPLidar::RPLidar()
    : uart(NULL)
{
    _currentMeasurement.distance = 0;
    _currentMeasurement.angle = 0;
    _currentMeasurement.quality = 0;
    _currentMeasurement.startBit = 0;

    UART_Params_init(&uartParams);
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readTimeout = UART_WAIT_FOREVER;
    uartParams.writeTimeout = UART_WAIT_FOREVER;
    uartParams.readCallback = NULL;
    uartParams.writeCallback = NULL;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;
    uartParams.dataLength = UART_LEN_8;
    uartParams.stopBits = UART_STOP_ONE;
    uartParams.parityType = UART_PAR_NONE;
    uartParams.readTimeout = 100;
//    uartParams.writeDataMode = UART_DATA_BINARY;
//    uartParams.readDataMode = UART_DATA_BINARY;
//    uartParams.readReturnMode = UART_RETURN_FULL;
//    uartParams.readEcho = UART_ECHO_OFF;
//    uartParams.baudRate = 115200;

    PWM_Params_init(&pwmParams);
    pwmParams.dutyUnits = PWM_DUTY_FRACTION;
    pwmParams.dutyValue = 0;
    pwmParams.periodUnits = PWM_PERIOD_US;
    pwmParams.periodValue = 100;

}


RPLidar::~RPLidar()
{
    end();
}

// open the given serial interface and try to connect to the RPLIDAR
void RPLidar::begin()
{
    PWM_init();
//    UART_init();
    uart = UART_open(CONFIG_UART_1, &uartParams);
    motor = PWM_open(CONFIG_PWM_0, &pwmParams);
    PWM_start(motor);
    if (!isOpen()) {
      begin();
    }
}

// close the currently opened serial interface
void RPLidar::end()
{
    if (isOpen()) {
        UART_close(uart);
        uart = NULL;
    }
}


// check whether the serial interface is opened
bool RPLidar::isOpen()
{

    return uart!=NULL;
}

// ask the RPLIDAR for its health info
u_result RPLidar::getHealth(rplidar_response_device_health_t & healthinfo, __u32 timeout)
{
    char currentbyte = -1;
    __u32 currentTs = _millis();
    __u32 remainingtime;
  
    __u8 *infobuf = (__u8 *)&healthinfo;
    __u8 recvPos = 0;

    rplidar_ans_header_t response_header;
    u_result  ans;


    if (!isOpen()) return RESULT_OPERATION_FAIL;

    {
        if (IS_FAIL(ans = _sendCommand(RPLIDAR_CMD_GET_DEVICE_HEALTH, NULL, 0))) {
            return ans;
        }

        if (IS_FAIL(ans = _waitResponseHeader(&response_header, timeout))) {
            return ans;
        }

        // verify whether we got a correct header
        if (response_header.type != RPLIDAR_ANS_TYPE_DEVHEALTH) {
            return RESULT_INVALID_DATA;
        }

        if ((response_header.size) < sizeof(rplidar_response_device_health_t)) {
            return RESULT_INVALID_DATA;
        }
        
        while ((remainingtime=_millis() - currentTs) <= timeout) {
            UART_read(uart,&currentbyte,1);
            if (currentbyte < 0) continue;
            
            infobuf[recvPos++] = currentbyte;

            if (recvPos == sizeof(rplidar_response_device_health_t)) {
                return RESULT_OK;
            }
        }
    }
    return RESULT_OPERATION_TIMEOUT;
}

// ask the RPLIDAR for its device info like the serial number
u_result RPLidar::getDeviceInfo(rplidar_response_device_info_t & info, __u32 timeout )
{
    char currentbyte = -1;
    __u8  recvPos = 0;
    __u32 currentTs = _millis();
    __u32 remainingtime;
    __u8 *infobuf = (__u8*)&info;
    rplidar_ans_header_t response_header;
    u_result  ans;

    if (!isOpen()) return RESULT_OPERATION_FAIL;

    {
        if (IS_FAIL(ans = _sendCommand(RPLIDAR_CMD_GET_DEVICE_INFO,NULL,0))) {
            return ans;
        }

        if (IS_FAIL(ans = _waitResponseHeader(&response_header, timeout))) {
            return ans;
        }

        // verify whether we got a correct header
        if (response_header.type != RPLIDAR_ANS_TYPE_DEVINFO) {
            return RESULT_INVALID_DATA;
        }

        if (response_header.size < sizeof(rplidar_response_device_info_t)) {
            return RESULT_INVALID_DATA;
        }

        while ((remainingtime=_millis() - currentTs) <= timeout) {
            UART_read(uart,&currentbyte,1);
            if (currentbyte<0) continue;    
            infobuf[recvPos++] = currentbyte;

            if (recvPos == sizeof(rplidar_response_device_info_t)) {
                return RESULT_OK;
            }
        }
    }
    
    return RESULT_OPERATION_TIMEOUT;
}

// stop the measurement operation
u_result RPLidar::stop()
{
    if (!isOpen()) return RESULT_OPERATION_FAIL;
    u_result ans = _sendCommand(RPLIDAR_CMD_STOP,NULL,0);
    stopMotor();
    return ans;
}

// start the measurement operation
u_result RPLidar::startScan(bool force, __u32 timeout)
{
    u_result ans;

    if (!isOpen()) return RESULT_OPERATION_FAIL;
    
    stop(); //force the previous operation to stop

    cleanBuffer(); // clean all rx buffer

    startMotor(); // start motor

    {
        ans = _sendCommand(force?RPLIDAR_CMD_FORCE_SCAN:RPLIDAR_CMD_SCAN, NULL, 0);
        if (IS_FAIL(ans)) return ans;

        // waiting for confirmation
        rplidar_ans_header_t response_header;
        if (IS_FAIL(ans = _waitResponseHeader(&response_header, timeout))) {
            return ans;
        }

        // verify whether we got a correct header
        if (response_header.type != RPLIDAR_ANS_TYPE_MEASUREMENT) {
            return RESULT_INVALID_DATA;
        }

        if (response_header.size < sizeof(rplidar_response_measurement_node_t)-1) {
            return RESULT_INVALID_DATA;
        }
    }

    int count;
    do{
        count = getUcount();
    }
    while(count<5);

    return RESULT_OK;
}

// wait for one sample point to arrive
u_result RPLidar::waitPoint(__u32 timeout)
{
    _handleUartBuffer();
    char currentbyte;
   __u32 currentTs = _millis();
//   __u32 remainingtime;
   rplidar_response_measurement_node_t node;
//   __u8 *nodebuf = (__u8*)&node;
   __u8 recvPos = 0;

   while ((_millis() - currentTs) <= timeout) {
       if(UART_read(uart,&currentbyte,1) <1) {
           continue;
       }

        switch (recvPos) {
            case 0: // expect the sync bit and its reverse in this byte          {
                {
                    __u8 tmp = (currentbyte>>1);
                    if ( (tmp ^ currentbyte) & 0x1 ) {
                        // pass
                        node.sync_quality=currentbyte;
                    } else {
                        continue;
                    }

                }
                break;
            case 1: // expect the highest bit to be 1
                {
                    if (currentbyte & RPLIDAR_RESP_MEASUREMENT_CHECKBIT) {
                        // pass
                        node.angle_q6_checkbit=currentbyte;
                    } else {
                        recvPos = 0;
                        continue;
                    }
                }
                break;
            case 2:
                {
                    __u16 cu = currentbyte;
                    node.angle_q6_checkbit+=(cu<<8);
                }
                break;
            case 3:
                {
                    node.distance_q2=currentbyte;
                }
                break;
            case 4:
                {
                    __u16 cu = currentbyte;
                    node.distance_q2+=(cu<<8);
                }
                break;
          }
//          nodebuf[recvPos++] = currentbyte;
          recvPos++;

          if (recvPos == sizeof(rplidar_response_measurement_node_t)-1) {
              // store the data ...
              _currentMeasurement.distance = node.distance_q2/4.0f;
              _currentMeasurement.angle = (node.angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f;
              _currentMeasurement.quality = (node.sync_quality>>RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
              _currentMeasurement.startBit = (node.sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT);
              return RESULT_OK;
          }
        
   }

   return RESULT_OPERATION_TIMEOUT;
}



u_result RPLidar::_sendCommand(__u8 cmd, const void * payload, size_t payloadsize)
{

    rplidar_cmd_packet_t pkt_header;
    rplidar_cmd_packet_t * header = &pkt_header;
    __u8 checksum = 0;

    if (payloadsize && payload) {
        cmd |= RPLIDAR_CMDFLAG_HAS_PAYLOAD;
    }

    header->syncByte = RPLIDAR_CMD_SYNC_BYTE;
    header->cmd_flag = cmd;

    // send header first
//    _bined_serialdev->write( (uint8_t *)header, 2);
    UART_write(uart,(uint8_t *)header,2);

    if (cmd & RPLIDAR_CMDFLAG_HAS_PAYLOAD) {
        checksum ^= RPLIDAR_CMD_SYNC_BYTE;
        checksum ^= cmd;
        checksum ^= (payloadsize & 0xFF);

        // calc checksum
        for (size_t pos = 0; pos < payloadsize; ++pos) {
            checksum ^= ((__u8 *)payload)[pos];
        }

        // send size
        __u8 sizebyte = payloadsize;
//        _bined_serialdev->write((uint8_t *)&sizebyte, 1);
        UART_write(uart,(uint8_t *)&sizebyte,1);

        // send payload
//        _bined_serialdev->write((uint8_t *)&payload, sizebyte);
        UART_write(uart,(uint8_t *)&payload,sizebyte);

        // send checksum
//        _bined_serialdev->write((uint8_t *)&checksum, 1);
        UART_write(uart,(uint8_t *)&checksum,1);

    }

    return RESULT_OK;
}

u_result RPLidar::_waitResponseHeader(rplidar_ans_header_t * header, __u32 timeout)
{
    char currentbyte;
    __u8  recvPos = 0;
    __u32 currentTs = _millis();
//    __u32 remainingtime;
//    __u8 *headerbuf = (__u8*)header;
    while ((_millis() - currentTs) <= timeout) {
        
        if(UART_read(uart,&currentbyte,1) <1) {
            continue;
        }
        switch (recvPos) {
        case 0:
            if (currentbyte != RPLIDAR_ANS_SYNC_BYTE1) {
                continue;
            }
            header->syncByte1=currentbyte;
            break;
        case 1:
            if (currentbyte != RPLIDAR_ANS_SYNC_BYTE2) {
                recvPos = 0;
                continue;
            }
            header->syncByte2=currentbyte;
            break;
        case 2:
            header->size = currentbyte;
            break;
        case 3:
            header->size += (currentbyte<<8);
            break;
        case 4:
            header->size += (currentbyte<<16);
            break;
        case 5:
            header->size += (currentbyte<<24);
            header->subType = (currentbyte>>6);
            break;
        case 6:
            header->type = currentbyte;
            return RESULT_OK;
        }
        recvPos++;

//        if (recvPos == sizeof(rplidar_ans_header_t)) {
//            return RESULT_OK;
//        }
  

    }

    return RESULT_OPERATION_TIMEOUT;
}

TickType_t RPLidar::_millis()
{
    return xTaskGetTickCount();
}

void RPLidar::_handleUartBuffer()
{
    int count = getUcount();
    if(count<5){
        startScan();
    }
    else if(count>3000){
        stop();
        startMotor();
    }
}

void RPLidar::startMotor()
{
    PWM_setDuty(motor, PWM_DUTY_FRACTION_MAX);
}

void RPLidar::stopMotor()
{
    PWM_setDuty(motor, 0);
}

int RPLidar::getUcount()
{
    int count;
    UART_control(uart,UART_CMD_GETRXCOUNT,&count);
    return count;
}

void RPLidar::cleanBuffer()
{
    int count;
    count = getUcount();
    while(count>0){
        char temp[count];
        UART_read(uart,&temp,sizeof(temp));
        count = getUcount();
    }
}

//void RPLidar::readPointToQueue(LIDAR_QUEUE queue){
//    if(IS_OK(waitPoint())){
//        float Distance = getCurrentPoint().distance; //distance value in mm unit
//        uint8_t  quality  = getCurrentPoint().quality; //quality of the current measurement
//        float angle    = getCurrentPoint().angle; //anglue value in degree
//        //                if(angle<360) continue;
//        if(Distance<=0||angle > 1 || quality <=0 ) return;
//        int  startBit = getCurrentPoint().startBit; //whether this point is belong to a new scan
//        queue.send(Distance, angle);
//    }
//    else{
//        startScan();
//    }
//}
