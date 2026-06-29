/**
 * **************************************************************************
 * @file    kpm.h
 * @brief   Abstract macros mapping matrix rows/columns to physical GPIO lines.
 * **************************************************************************
 */

#ifndef __kpm_h__                    
#define __kpm_h__                    

#include "types.h"                  

/* Hardware Output Port Matrix Assignment mapping targets (Pins P1.16 - P1.19) */
#define row0 16                      
#define row1 17                      
#define row2 18                      
#define row3 19                      

/* Hardware Input Port Matrix Assignment mapping targets (Pins P1.20 - P1.23) */
#define col0 20                      
#define col1 21                      
#define col2 22                      
#define col3 23                      

/* Keypad Driver APIs */
void init_kpm(void);                 
u32 colscan(void);                   
s32 keyscan(void);                   
u32 rowcheck(void);                  
u32 colcheck(void);                  
s32 readnum(void);                   
void id_kpm(s8 *ptr);                
void password_kpm(s8 *ptr);          

#endif