/* 
 * tracer_can_parser.h
 * 
 * Created on: Apr 14, 2020 10:52
 * Description: 
 * 
 * Copyright (c) 2020 Ruixiang Du (rdu)
 */ 

#ifndef TRACER_CAN_PARSER_H
#define TRACER_CAN_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "ugv_sdk/tracer/tracer_protocol.h"

#include <linux/can.h>

bool DecodeTracerMsgFromCAN(const struct can_frame *rx_frame, TracerMessage *msg);
void EncodeTracerMsgToCAN(const TracerMessage *msg, struct can_frame *tx_frame);

uint8_t CalcTracerCANChecksum(uint16_t id, uint8_t *data, uint8_t dlc);

#ifdef __cplusplus
}
#endif

#endif /* TRACER_CAN_PARSER_H */
