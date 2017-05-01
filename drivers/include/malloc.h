/*
*****************************************************************************************************************
*                                           MuChun Electron
*                                       ZhenJiang,Jiangsu,China
*                             (c) Copyright 2016,MuChun Electron,Zhenjiang,China
*                                         All Rights Reserved
*
*
*
*文 件 名 :malloc.h
*作    者 :SMC
*文件说明 :动态内存管理，可跨平台移植
*****************************************************************************************************************
*/
/* $ PAGE*/
#ifndef __MALLOC_H
#define __MALLOC_H

#include "common.h"

#define MEM_BLOCK_SIZE       32                                      /* 内存块大小为32字节 */
#define MAX_MEM_SIZE         (1 * 1024)                              /* 最大管理内存 1k    */
#define MEM_ALLOC_TABLE_SIZE (MAX_MEM_SIZE / MEM_BLOCK_SIZE)         /* 内存表大小         */

/* 内存管理控制器 */
typedef struct _m_mallco_dev {
	void (*init)(void);                                              /* 初始化             */
	int8_u (*perused)(void);                                         /* 内存使用率         */
	int8_u  membase[MAX_MEM_SIZE];                                   /* 内存池             */
	int16_u memmap[MEM_ALLOC_TABLE_SIZE];                            /* 内存管理状态表     */
	int8_u  memrdy;                                                  /* 内存管理是否就绪   */
} _m_mallco_dev;

/* 内存管理控制器 */
extern struct _m_mallco_dev mallco_dev;

/*
******************************************************************************************************************
*                                            FUNCTION PROTOTYPES
******************************************************************************************************************
*/
/* 本文件内部调用函数  */
static void __memset(void *s, int8_u c, int32_u count);            /* 设置内存           */
static void __memcpy(void *des, void *src, int32_u n);             /* 复制内存           */ 
static void mem_init(void);                                        /* 内存管理初始化函数 */
static int32_u mem_malloc(int32_u size);                           /* 内存分配           */
static int8_u mem_free(int32_u offset);                            /* 内存释放           */

/* 供外部文件调用  */
int8_u mem_perused(void);                                          /* 获得内存使用率     */
void free_m(void *ptr);                                            /* 内存释放           */
void *malloc_m(int32_u size);                                      /* 内存分配           */
void *realloc_m(void *ptr,int32_u size);                           /* 重新分配内存       */







#endif






