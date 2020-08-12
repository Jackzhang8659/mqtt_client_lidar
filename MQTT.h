/*
 * MQTT.h
 *
 *  Created on: Apr 7, 2020
 *      Author: jackz
 */

#ifndef MQTT_H_
#define MQTT_H_

/* Simplelink includes                                                        */
#include <ti/drivers/net/wifi/simplelink.h>

/* SlNetSock includes                                                        */
#include <ti/drivers/net/wifi/slnetifwifi.h>

/* MQTT Library includes                                                     */
#include <ti/net/mqtt/mqttclient.h>

/* Common interface includes                                                 */
#include "network_if.h"
#include "uart_term.h"

/* JSON Libary for C++      */
#include "json.hpp"
using json = nlohmann::json;

#include <map>
#include <string>

/* MQTT Version */
#define MQTT_3_1                true

/* Server Address and port */
#define SERVER_ADDRESS          ""
#define PORT_NUMBER              1883

/* Retain Flag. Used in publish message.                                     */
#define RETAIN_ENABLE            1

/* Defining Publish Topic Values                                             */
#define PUBLISH_TOPIC0           "/cc32xx/ButtonPressEvtSw2"

#define SLNET_IF_WIFI_PRIO       (5)


class MQTT
{
public:
    MQTT();
    virtual ~MQTT();
    int connect();
    void Publish(std::map<float,float> LidarMap);


private:
    MQTTClient_Handle MqttClient;
    MQTTClient_ConnParams Mqtt_ClientCtx =
    {
        MQTTCLIENT_NETCONN_IP4,
        SERVER_ADDRESS,
        PORT_NUMBER, 0, 0, 0,
        NULL
    };

    char ClientId[13];

    SlWlanSecParams_t SecurityParams;

    MQTTClient_Params MqttClient_params;

    int32_t SetClientIdNamefromMacAddress();
};

void MqttClientCallback(int32_t event,
                        void * metaData,
                        uint32_t metaDateLen,
                        void *data,
                        uint32_t dataLen);

#endif /* MQTT_H_ */
