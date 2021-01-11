/* 
 * scout_can_parser.h
 * 
 * Created on: Aug 31, 2019 04:23
 * Description: 
 * 
 * Copyright (c) 2019 Ruixiang Du (rdu)
 */ 

#ifndef SCOUT_CAN_PARSER_H
#define SCOUT_CAN_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "ugv_sdk/scout/scout_protocol.h"

#include <linux/can.h>

bool DecodeScoutMsgFromCAN(const struct can_frame *rx_frame, ScoutMessage *msg);
void EncodeScoutMsgToCAN(const ScoutMessage *msg, struct can_frame *tx_frame);

uint8_t CalcScoutCANChecksum(uint16_t id, uint8_t *data, uint8_t dlc);

#ifdef __cplusplus
}
#endif

#endif /* SCOUT_CAN_PARSER_H */
