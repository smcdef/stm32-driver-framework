/*
*****************************************************************************************************************
*                                           MuChun Electron
*                                       ZhenJiang,Jiangsu,China
*                             (c) Copyright 2016,MuChun Electron,Zhenjiang,China
*                                         All Rights Reserved
*
*
*
*文 件 名 :common.h
*作    者 :SMC
*文件说明 :驱动通用部分
*****************************************************************************************************************
*/
/* $ PAGE*/
#ifndef __COMMON_H
#define __COMMON_H


#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))    /* 计算数组元素个数 */


typedef unsigned char  int8_u;		                  /* Unsigned  8 bit quantity       */
typedef signed   char  int8_s;		                  /* Signed    8 bit quantity       */
typedef unsigned short int16_u;		                  /* Unsigned 16 bit quantity       */
typedef signed   short int16_s;		                  /* Signed   16 bit quantity       */
typedef unsigned int   int32_u;		                  /* Unsigned 32 bit quantity       */
typedef signed   int   int32_s;		                  /* Signed   32 bit quantity       */
typedef float          fp32;		                  /* Single precision floating point*/
typedef double         fp64;		                  /* Double precision floating point*/






#endif






