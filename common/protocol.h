#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/type.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define TCP_PORT 50000
#define UDP_PROT 50001
#define IP_SERVER "0.0.0.0"
#define MAGIC 0x44524E53
#define MAX_PAYLOAD 8192


typedef enum {
    /*AUTH*/
    MSG_REQ_REGISTER        = 0x0001,
    MSG_REQ_LOGIN           = 0x0002,
    MSG_REQ_LOGOUT          = 0x0003,
    
    /*DEVICE*/
    MSG_REQ_DEV_ADD         = 0x0101,
    MSG_REQ_DEV_EDIT        = 0x0102,
    MSG_REQ_DEV_DELETE      = 0x0103,
    MSG_REQ_DEV_LIST        = 0x0104,
    MSG_REQ_DEV_INFO        = 0x0105,
    
    /*FLIGHT & WAYPOINT*/
    MSG_REQ_FL_NEW          = 0x0201,
    MSG_REQ_FL_EDIT         = 0x0202,
    MSG_REQ_FL_DELETE       = 0x0203,
    MSG_REQ_FL_LIST         = 0x0204,
    MSG_REQ_FL_INFO         = 0x0205,

    MSG_REQ_WP_ADD          = 0x0211,
    MSG_REQ_WP_EDIT         = 0x0212,
    MSG_REQ_WP_DELETE       = 0x0213,
    MSG_REQ_WP_REORDER      = 0x0214,
    
    /*CMD*/
    MSG_REQ_CMD_ONOFF       = 0x0301,
    MSG_REQ_CMD_TAKEOFF     = 0x0302,
    MSG_REQ_CMD_LAN         = 0x0303,
    MSG_REQ_CMD_EXEC_FL     = 0x0304,
    MSG_REQ_CMD_STOP        = 0x0305,
    MSG_REQ_CMD_RTL         = 0x0306,

    /*DRONE -> SERVER*/
    MSG_DRONE_HELLO         = 0x0401,
    MSG_DRONE_ACK           = 0x0402,
    MSG_REQ_SUBCRIBE        = 0x0405,

    /*SERVER -> DRONE*/
    MSG_DRONE_CMD_ONOFF     = 0x0501,
    MSG_DRONE_CMD_TAKEOFF   = 0x0502,
    MSG_DRONE_CMD_LAND      = 0x0503,
    MSG_DRONE_CMD_EXEC_FL   = 0x0504,
    MSG_DRONE_CMD_STOP      = 0x0505,
    MSG_DRONE_CMD_RTL       = 0x0506,
    
    /*HEARTBEAT*/
    MSG_PING                = 0x0601,
    MSG_PONG                = 0x0602,
    MSG_DRONE_HEARTBEAT     = 0x0603,
    MSG_SERVER_HEARTBEAT    = 0x0604

    /*SERVER -> CLIENT*/
    MSG_RES_OK              = 0x8000,
    MSG_RES_DEV_LIST        = 0x8101,
    MSG_RES_DEV_INFO        = 0x8102,
    MSG_RES_DL_LIST         = 0x8201,
    MSG_RES_FL_INFO         = 0x8202,
    MSG_RES_ERROR           = 0x9000
} msg_t;


typedef enum{
    ERR_AUTH_FAIL           = 1,
    ERR_NOT_LOGGED_IN       = 2,
    ERR_NOT_PERMISSION      = 3,
    ERR_NOT_FOUND           = 4,
    ERR_ALREADY_EXISTS      = 5,
    ERR_INVALID_ARGS        = 6,
    ERR_DRONE_OFFLINE       = 7,
    ERR_DRONE_BUSY          = 8,
    ERR_IO                  = 10,
    ERR_INTERNAL            = 99
} err_code_t;

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint16_t type;
    uint16_t flags;
    uint32_t req_id;
    uint32_t length;
}frame_header_t;

typedef struct __attribute__((packed)){
    char username[32];
    char password[64];
} register_req_t, login_req_t;

typedef struct __attribute__((packed)){
    uint8_t session_id[16];
    char username[32];
    uint32_t expires_at;
} login_ok_t;

typedef struct __attribute__((packed)){
    uint8_t session_id[16];
} logout_req_t, subcribe_req_t;

typedef struct __attribute__((packed)){
    uint8_t session_id[16];
    char device_id[32];
    char name[64];
    char description[128];
} dev_add_req_t, dev_edit_req_t;


#endif