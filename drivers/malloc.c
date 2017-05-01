/*
*****************************************************************************************************************
*                                           MuChun Electron
*                                       ZhenJiang,Jiangsu,China
*                             (c) Copyright 2016,MuChun Electron,Zhenjiang,China
*                                         All Rights Reserved
*
*
*
*文 件 名 :malloc.c
*作    者 :SMC
*文件说明 :动态内存管理，可跨平台移植，代码出自原子STM32例程，向原作者致谢！
*****************************************************************************************************************
*/
/* $ PAGE*/
#include "malloc.h"



/* 内存管理控制器 */
_m_mallco_dev mallco_dev = {
	mem_init,
	mem_perused,
	0,
	0,
	0,
};

/*
******************************************************************************************************************
*                                                  __memcpy()
*
*作    者 : SMC
*函数说明 : 内存复制
*输入参数 : des:目的地址; src:源地址; n:需要复制的内存长度(字节为单位)
*输出参数 : none
*返 回 值 : none
*注意事项 : none
******************************************************************************************************************
*/
static void __memcpy(void *des, void *src, int32_u n)  
{
	int8_u *xdes = des;
	int8_u *xsrc = src;
	
	while (n--)
		*xdes++ = *xsrc++;
}

/*
******************************************************************************************************************
*                                                  __memset()
*
*作    者 : SMC
*函数说明 : 设置内存
*输入参数 : *s:内存首地址; c :要设置的值; count:需要设置的内存大小(字节为单位)
*输出参数 : none
*返 回 值 : none
*注意事项 : none
******************************************************************************************************************
*/
static void __memset(void *s, int8_u c, int32_u count)  
{
	int8_u *xs = s;

	while (count--)
		*xs++ = c;
}

/*
******************************************************************************************************************
*                                                  mem_init()
*
*作    者 : SMC
*函数说明 : 内存管理初始化
*输入参数 : none
*输出参数 : none
*返 回 值 : none
*注意事项 : none
******************************************************************************************************************
*/
static void mem_init(void)  
{
    __memset(mallco_dev.membase, 0, sizeof(mallco_dev.membase)); /* 内存池素有数据清零 */
	__memset(mallco_dev.memmap, 0, sizeof(mallco_dev.memmap));   /* 内存状态表数据清零 */
    mallco_dev.memrdy = 1;                                       /* 内存管理初始化成功 */
}

/*
******************************************************************************************************************
*                                                  mem_perused()
*
*作    者 : SMC
*函数说明 : 获取内存使用率
*输入参数 : none
*输出参数 : none
*返 回 值 : 使用率(0~100)
*注意事项 : none
******************************************************************************************************************
*/
int8_u mem_perused(void)  
{
	int16_u used = 0;  
	int32_u i;
	
	for (i = 0; i < MEM_ALLOC_TABLE_SIZE; i++)
		if (mallco_dev.memmap[i])
			used++;

	return used * 100 / MEM_ALLOC_TABLE_SIZE;  
}

/*
******************************************************************************************************************
*                                                  mem_malloc()
*
*作    者 : SMC
*函数说明 : 内存分配
*输入参数 : size:要分配的内存大小(字节)
*输出参数 : none
*返 回 值 : 0XFFFFFFFF,代表错误; 其他,内存偏移地址 
*注意事项 : none
******************************************************************************************************************
*/
static int32_u mem_malloc(int32_u size)
{
	signed long offset = 0;
	int16_u nmemb;                   /* 需要的内存块数 */
	int16_u cmemb = 0;               /* 连续空内存块数 */
	int32_u i;
	
	if (!mallco_dev.memrdy)
		mallco_dev.init();
	if (size == 0)
		return 0XFFFFFFFF;           /* 不需要分配                 */
	nmemb = size / MEM_BLOCK_SIZE;   /* 获取需要分配的连续内存块数 */
	if (size % MEM_BLOCK_SIZE)
		nmemb++;
	for (offset = MEM_ALLOC_TABLE_SIZE - 1; offset >= 0; offset--) {
		if (!mallco_dev.memmap[offset])
			cmemb++;                              /* 连续空内存块数增加        */
		else 
			cmemb=0;                              /* 连续内存块清零            */
		if (cmemb == nmemb) {                     /* 找到了连续nmemb个空内存块 */
			for (i = 0; i < nmemb; i++)           /* 标注内存块非空            */
				mallco_dev.memmap[offset + i] = nmemb;
			return (offset * MEM_BLOCK_SIZE);     /* 返回偏移地址              */
		}
	}
	
	return 0XFFFFFFFF;
}

/*
******************************************************************************************************************
*                                                  mem_free()
*
*作    者 : SMC
*函数说明 : 释放内存
*输入参数 : offset:内存地址偏移
*输出参数 : none
*返 回 值 : 0,释放成功; 1,释放失败;
*注意事项 : none
******************************************************************************************************************
*/
static int8_u mem_free(int32_u offset)  
{
	int i;
	int index;
	int nmemb;
	
	/* 未初始化,先执行初始化 */
	if (!mallco_dev.memrdy) {
		mallco_dev.init();    
		return 1;           
	}
	
	if (offset < MAX_MEM_SIZE) {
		index = offset / MEM_BLOCK_SIZE;      /* 偏移所在内存块号码 */
		nmemb = mallco_dev.memmap[index];     /* 内存块数量         */
		for (i = 0; i < nmemb; i++)           /* 内存块清零         */
			mallco_dev.memmap[index + i] = 0;
		return 0;
	} else {
		return 2;
	}
}

/*
******************************************************************************************************************
*                                                  free_m()
*
*作    者 : SMC
*函数说明 : 释放内存
*输入参数 : ptr:内存首地址
*输出参数 : none
*返 回 值 : none
*注意事项 : none
******************************************************************************************************************
*/
void free_m(void *ptr)  
{
	int32_u offset;  
	
	if (ptr == NULL)
		return;
	offset = (int32_u)ptr - (int32_u)&mallco_dev.membase;
	mem_free(offset);
}

/*
******************************************************************************************************************
*                                                  malloc_m()
*
*作    者 : SMC
*函数说明 : 分配内存
*输入参数 : size:内存大小(字节)
*输出参数 : none
*返 回 值 : 分配到的内存首地址
*注意事项 : none
******************************************************************************************************************
*/
void *malloc_m(int32_u size)  
{
	int32_u offset;
	
	offset = mem_malloc(size);  
	if (offset == 0XFFFFFFFF)
		return NULL; 
	else
		return (void*)((int32_u)&mallco_dev.membase + offset);
}

/*
******************************************************************************************************************
*                                                  realloc_m()
*
*作    者 : SMC
*函数说明 : 重新分配内存
*输入参数 : *ptr:旧内存首地址; size:要分配的内存大小(字节)
*输出参数 : none
*返 回 值 : 新分配到的内存首地址
*注意事项 : none
******************************************************************************************************************
*/
void *realloc_m(void *ptr, int32_u size)  
{
	int32_u offset;
	
	offset = mem_malloc(size);
	if (offset == 0XFFFFFFFF) {
		return NULL;
	} else {
		__memcpy((void*)((int32_u)&mallco_dev.membase+offset), ptr,size);
		free_m(ptr);
		return (void*)((int32_u)&mallco_dev.membase+offset);
	}
}



























