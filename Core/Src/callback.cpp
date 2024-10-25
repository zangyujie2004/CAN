//
// Created by Administrator on 24-10-12.
//

#include "can.h"
#include "main.h"

extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t RxData[8];
extern float motor_angle;
extern float motor_speed;
extern float motor_current;
extern float motor_temperature;


float linearMapping(int in, int in_min, int in_max, float out_min, float out_max) {
    return (out_max - out_min) / (in_max - in_min) * (in - in_min) + out_min;
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == hcan1.Instance) {
        // Check if the message was successfully received
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
            motor_angle = linearMapping((RxData[0] << 8) | RxData[1], 0, 8191, 0, 360);

            motor_speed = (RxData[2] << 8) | RxData[3];

            // Extract and map motor current (-20 to 20 A, with an offset of 16384)
            motor_current = linearMapping(((RxData[4] << 8) | RxData[5]) - 16384, -16384, 16384, -20, 20);

            motor_temperature = RxData[6];
        }
    }
}