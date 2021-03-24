/*
 Name:		guitest.ino
 Created:	3/12/2021 12:40:32 PM
 Author:	paulh
*/


#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP32Encoder.h>
#include "setup.h"
#include "network.h"
#include "vfo.h"
#include "io.h"

// the loop function runs over and over again until power down or reset
void loop() {
    while (1)
    {
        if (net_queue)
        { // Check if we need to start or stop wifi
            uint8_t message;
            if (xQueueReceive(net_queue, &message, 0))
            {
                if (message == 0)
                {
                    // stopwifi
                    vfo_network.network_stop();
                }
                if (message == 1)
                {
                    // startwifi
                    vfo_network.begin(TRXNET_SERVER);
                }
            }
        }
        CheckCAT();
        check_rx_tx();
        vfo_network.network_loop();
        delay(5);
    }
}
