#include "config.h"
#include "protocol.h"
#include "utils.h"

int build_header_pkt(uint8_t *buf, int buf_size, msg_t type, uint16_t flags , uint32_t req_id, uint32_t length){
    if(!buf) return -1;
    if(buf_size < FRAME_HEADER_SIZE) return -1;
    if(length > MAX_PAYLOAD) return -1;
    memset(buf, 0, FRAME_HEADER_SIZE);
    int offset = 0;
    uint32_t net_magic = htonl(MAGIC);
    memcpy(buf, &net_magic, 4);
    offset = offset + 4;
    uint16_t net_type = htons((uint16_t)type);
    memcpy(buf + offset, &net_type, 2);
    offset = offset + 2;
    uint16_t net_flags = htons(flags);
    memcpy(buf + offset, &net_flags, 2);
    offset = offset + 2;
    uint32_t net_req_id = htonl(req_id);
    memcpy(buf + offset, &net_req_id, 4);
    offset = offset + 4;
    uint32_t net_length = htonl(length);
    memcpy(buf + offset, &net_length, 4);
    return FRAME_HEADER_SIZE;
}

int parse_header_pkt(uint8_t *buf, int buf_size, msg_t *type, uint16_t *flags, uint32_t *req_id, uint32_t *length){
    if(!buf || !type || !flags || !req_id || !length) return -1;
    if(buf_size < FRAME_HEADER_SIZE) return -1;
    uint32_t raw_magic;
    memcpy(&raw_magic, buf, 4);
    if((ntohl(raw_magic))!= MAGIC) return -1;
    int offset = 4;
    uint16_t raw_type, raw_flags;
    uint32_t raw_req_id, raw_length;
    memcpy(&raw_type, buf + offset, 2);
    *type = (msg_t)ntohs(raw_type);
    offset = offset + 2;
    memcpy(&raw_flags, buf + offset, 2);
    *flags = ntohs(raw_flags);
    offset = offset + 2;
    memcpy(&raw_req_id, buf + offset, 4);
    *req_id = ntohl(raw_req_id);
    offset = offset + 4;
    memcpy(&raw_length, buf + offset, 4);
    *length = ntohl(raw_length);
    offset = offset + 4;
    if(*length > MAX_PAYLOAD) return -1;
    return FRAME_HEADER_SIZE;
}

int build_regsiter_req(uint8_t *buf, int buf_size, const char *username, const char *password){
    if(!buf || !username || !password) return -1;
    if(buf_size < (MAX_USERNAME + MAX_PASSWORD)) return -1;
    int username_len = strnlen(username, MAX_USERNAME);
    int password_len = strnlen(password, MAX_PASSWORD);
    if(username_len == 0 || username_len >= MAX_USERNAME) return -1;
    if(password_len == 0 || password_len >= MAX_PASSWORD) return -1;
    memset(buf, 0, (MAX_USERNAME + MAX_PASSWORD));
    memcpy(buf, username, username_len);
    memcpy(buf + MAX_USERNAME, password, password_len);
    return (MAX_USERNAME + MAX_PASSWORD);
}

int parse_register_req(const uint8_t *buf, int buf_size, char *user, int user_size, char *password, int password_size){
    if(!buf || !user || !password) return -1;
    if(buf_size < (MAX_USERNAME + MAX_PASSWORD)) return -1;
    if(user_size < MAX_USERNAME + 1) return -1;
    if(password_size < MAX_PASSWORD + 1) return -1;
    memcpy(user, buf, MAX_USERNAME);
    memcpy(password, buf + MAX_USERNAME, MAX_PASSWORD);
    user[MAX_USERNAME] = '\0';
    password[MAX_PASSWORD] = '\0';
    if(user[0] == '\0') return -1;
    if(password[0] == '\0') return -1;
    return (MAX_USERNAME + MAX_PASSWORD);
}