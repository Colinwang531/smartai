//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-05-27
//		Description:
//
//		History:						Author										Date													Description
//											王科威										2017-05-27									创建
//											王科威										2017-06-14									归类整理错误码值定义
//

#ifndef ERRS_H
#define ERRS_H

//Specific
#ifndef ERROR_CONNECTION_ABORTED
#define ERROR_CONNECTION_ABORTED 1236L
#endif//ERROR_CONNECTION_ABORTED
//Public
#define ERR_OK 1
#define ERR_INVALID_PARAM -1
#define ERR_BAD_ALLOC -2
#define ERR_BAD_FREE -3
#define ERR_EXISTED -4
#define ERR_NOT_FOUND -5
#define ERR_NOT_SUPPORT -6
#define ERR_UNKNOWN_TYPE -7
#define ERR_ABORT -8
#define ERR_NOT_IMPLEMENT -9
#define ERR_OUT_RANGE -10
#define ERR_BAD_URI -11
#define ERR_BAD_OPERATE - 12
#define ERR_BAD_HEADER -13
#define ERR_EMPTY -14
//SDK
#define ERR_SDK_BAD_INIT -100
#define ERR_SDK_BAD_RELEASE -101
//Network
#define ERR_INVALID_SOCKET -200
#define ERR_INVALID_ADDRESS -201
#define ERR_INVALID_PORT -202
#define ERR_SEND_FAILED -203
#define ERR_RECV_FAILED -204
#define ERR_BIND_FAILED -205
#define ERR_LISTEN_FAILED -206
#define ERR_CONNECT_FAILED -207
#define ERR_SETOPT_FAILED -208
#define ERR_TIMER_EXPIRED -209
//Stream
#define ERR_STREAM_MUXING -300
#define ERR_STREAM_UNREACHABLE -301
#define ERR_STREAM_DATALOST -302
#define ERR_STREAM_OVERFLOW -303
//File
#define ERR_FILE_OPEN_FAILED -500
#define ERR_FILE_READ_FAILED -501
//Data
#define ERR_PARSE_FAILED -600
#define ERR_ENCAPS_FAILED -601
//Authen
#define ERR_BAD_PASSWD -700
#define ERR_BAD_AUTH -701
//Routing
#define ERR_ROUTING_UNREACHABLE -800
#define ERR_ROUTING_ROUTING -801

#endif//ERRS_H
