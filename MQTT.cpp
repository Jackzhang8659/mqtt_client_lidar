/*
 * MQTT.cpp
 *
 *  Created on: Apr 7, 2020
 *      Author: jackz
 */

#include <MQTT.h>

MQTT::MQTT()
{
    // TODO Auto-generated constructor stub

    SlNetIf_init(0);
    SlNetIf_add(SLNETIF_ID_1, "CC32xx",
                (const SlNetIf_Config_t *)&SlNetIfConfigWifi,
                SLNET_IF_WIFI_PRIO);

    SlNetSock_init(0);
    SlNetUtil_init(0);

    ClientId[13] = {'\0'};
    SetClientIdNamefromMacAddress();
    SecurityParams =
        {
             /* Initialize AP security params                                         */
             SECURITY_TYPE,
             (signed char *) SECURITY_KEY,
             strlen(SECURITY_KEY)
        };

    MQTTClient_Params MqttClient_params =
    {
         ClientId,
         MQTT_3_1,
         true,
         &Mqtt_ClientCtx
    };

    /*Initialize MQTT client lib                                             */
    MqttClient = MQTTClient_create(MqttClientCallback,
                                    &MqttClient_params);
    if(MqttClient == NULL)
    {
        /*lib initialization failed                                          */
        UART_PRINT("Failed to initilize MQTT Client\n\r");
    }
}

MQTT::~MQTT()
{
    // TODO Auto-generated destructor stub
}

int MQTT::connect()
{
    long lRetVal;
    char SSID_Remote_Name[32];
    int8_t Str_Length;

    memset(SSID_Remote_Name, '\0', sizeof(SSID_Remote_Name));
    Str_Length = strlen(SSID_NAME);

    if(Str_Length)
    {
        /*Copy the Default SSID to the local variable                        */
        strncpy(SSID_Remote_Name, SSID_NAME, Str_Length);
    }

    /*Reset The state of the machine                                         */
    Network_IF_ResetMCUStateMachine();

    /*Start the driver                                                       */
    lRetVal = Network_IF_InitDriver(ROLE_STA);
    if(lRetVal < 0)
    {
        UART_PRINT("Failed to start SimpleLink Device\n\r", lRetVal);
        return(-1);
    }

    /*Connect to the Access Point                                            */
    lRetVal = Network_IF_ConnectAP(SSID_Remote_Name, SecurityParams);
    if(lRetVal < 0)
    {
        UART_PRINT("Connection to an AP failed\n\r");
        return(-1);
    }
    return(1);
}

void MQTT::Publish(std::map<float,float> LidarMap){
    json j(LidarMap);
    std::string s = j.dump();
    char * publish_data = new char[s.size()+1];
    strcpy(publish_data,s.c_str());
    const char *publish_topic = { PUBLISH_TOPIC0 };
    MQTTClient_publish(MqttClient, (char*) publish_topic, strlen(
                                          (char*)publish_topic),
                                      (char*)publish_data,
                                      strlen((char*) publish_data), MQTT_QOS_2 |
                                      ((RETAIN_ENABLE) ? MQTT_PUBLISH_RETAIN : 0));
}

int32_t MQTT::SetClientIdNamefromMacAddress(){
    {
        int32_t ret = 0;
        uint8_t Client_Mac_Name[2];
        uint8_t Index;
        uint16_t macAddressLen = SL_MAC_ADDR_LEN;
        uint8_t macAddress[SL_MAC_ADDR_LEN];

        /*Get the device Mac address */
        ret = sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen,
                           &macAddress[0]);

        /*When ClientID isn't set, use the mac address as ClientID               */
        if(ClientId[0] == '\0')
        {
            /*6 bytes is the length of the mac address                           */
            for(Index = 0; Index < SL_MAC_ADDR_LEN; Index++)
            {
                /*Each mac address byte contains two hexadecimal characters      */
                /*Copy the 4 MSB - the most significant character                */
                Client_Mac_Name[0] = (macAddress[Index] >> 4) & 0xf;
                /*Copy the 4 LSB - the least significant character               */
                Client_Mac_Name[1] = macAddress[Index] & 0xf;

                if(Client_Mac_Name[0] > 9)
                {
                    /*Converts and copies from number that is greater than 9 in  */
                    /*hexadecimal representation (a to f) into ascii character   */
                    ClientId[2 * Index] = Client_Mac_Name[0] + 'a' - 10;
                }
                else
                {
                    /*Converts and copies from number 0 - 9 in hexadecimal       */
                    /*representation into ascii character                        */
                    ClientId[2 * Index] = Client_Mac_Name[0] + '0';
                }
                if(Client_Mac_Name[1] > 9)
                {
                    /*Converts and copies from number that is greater than 9 in  */
                    /*hexadecimal representation (a to f) into ascii character   */
                    ClientId[2 * Index + 1] = Client_Mac_Name[1] + 'a' - 10;
                }
                else
                {
                    /*Converts and copies from number 0 - 9 in hexadecimal       */
                    /*representation into ascii character                        */
                    ClientId[2 * Index + 1] = Client_Mac_Name[1] + '0';
                }
            }
        }
        return(ret);
    }
}

void MqttClientCallback(int32_t event,
                                   void * metaData,
                                   uint32_t metaDateLen,
                                   void *data,
                                   uint32_t dataLen)
{
    int32_t i = 0;

        switch((MQTTClient_EventCB)event)
        {
        case MQTTClient_OPERATION_CB_EVENT:
        {
            switch(((MQTTClient_OperationMetaDataCB *)metaData)->messageType)
            {
            case MQTTCLIENT_OPERATION_CONNACK:
            {
                uint16_t *ConnACK = (uint16_t*) data;
                UART_PRINT("CONNACK:\n\r");
                /* Check if Conn Ack return value is Success (0) or       */
                /* Error - Negative value                                 */
//                if(0 == (MQTTClientCbs_ConnackRC(*ConnACK)))
//                {
//                    UART_PRINT("Connection Success\n\r");
//                }
//                else
//                {
//                    UART_PRINT("Connection Error: %d\n\r", *ConnACK);
//                }
                break;
            }

            case MQTTCLIENT_OPERATION_EVT_PUBACK:
            {
                char *PubAck = (char *) data;
                UART_PRINT("PubAck:\n\r");
                UART_PRINT("%s\n\r", PubAck);
                break;
            }

            case MQTTCLIENT_OPERATION_SUBACK:
            {
                UART_PRINT("Sub Ack:\n\r");
                UART_PRINT("Granted QoS Levels are:\n\r");
                for(i = 0; i < dataLen; i++)
                {
//                    UART_PRINT("%s :QoS %d\n\r", topic[i],
//                              ((unsigned char*) data)[i]);
                }
                break;
            }

            case MQTTCLIENT_OPERATION_UNSUBACK:
            {
                char *UnSub = (char *) data;
                UART_PRINT("UnSub Ack \n\r");
                UART_PRINT("%s\n\r", UnSub);
                break;
            }

            default:
                break;
            }
            break;
        }
        case MQTTClient_RECV_CB_EVENT:
        {
            MQTTClient_RecvMetaDataCB *recvMetaData =
                (MQTTClient_RecvMetaDataCB *)metaData;
            uint32_t bufSizeReqd = 0;
            uint32_t topicOffset;
            uint32_t payloadOffset;

//            struct publishMsgHeader msgHead;
//
//            char *pubBuff = NULL;
//            struct msgQueue queueElem;
//
//            topicOffset = sizeof(struct publishMsgHeader);
//            payloadOffset = sizeof(struct publishMsgHeader) +
//                            recvMetaData->topLen + 1;
//
//            bufSizeReqd += sizeof(struct publishMsgHeader);
            bufSizeReqd += recvMetaData->topLen + 1;
            bufSizeReqd += dataLen + 1;
//            pubBuff = (char *) malloc(bufSizeReqd);
//
//            if(pubBuff == NULL)
//            {
//                UART_PRINT("malloc failed: recv_cb\n\r");
//                return;
//            }

//            msgHead.topicLen = recvMetaData->topLen;
//            msgHead.payLen = dataLen;
//            msgHead.retain = recvMetaData->retain;
//            msgHead.dup = recvMetaData->dup;
//            msgHead.qos = recvMetaData->qos;
//            memcpy((void*) pubBuff, &msgHead, sizeof(struct publishMsgHeader));

            /* copying the topic name into the buffer                        */
//            memcpy((void*) (pubBuff + topicOffset),
//                   (const void*)recvMetaData->topic,
//                   recvMetaData->topLen);
//            memset((void*) (pubBuff + topicOffset + recvMetaData->topLen),'\0',1);
//
//            /* copying the payload into the buffer                           */
//            memcpy((void*) (pubBuff + payloadOffset), (const void*) data, dataLen);
//            memset((void*) (pubBuff + payloadOffset + dataLen), '\0', 1);

            UART_PRINT("\n\rMsg Recvd. by client\n\r");
//            UART_PRINT("TOPIC: %s\n\r", pubBuff + topicOffset);
//            UART_PRINT("PAYLOAD: %s\n\r", pubBuff + payloadOffset);
            UART_PRINT("QOS: %d\n\r", recvMetaData->qos);

            if(recvMetaData->retain)
            {
                UART_PRINT("Retained\n\r");
            }

            if(recvMetaData->dup)
            {
                UART_PRINT("Duplicate\n\r");
            }

            /* filling the queue element details                              */
//            queueElem.event = 11;
//            queueElem.msgPtr = pubBuff;
//            queueElem.topLen = recvMetaData->topLen;

            /* signal to the main task                                        */
//            if(MQTT_SendMsgToQueue(&queueElem))
//            {
//                UART_PRINT("\n\n\rQueue is full\n\n\r");
//            }
            break;
        }
        case MQTTClient_DISCONNECT_CB_EVENT:
        {
//            gResetApplication = true;
            UART_PRINT("BRIDGE DISCONNECTION\n\r");
            break;
        }
        }
}
