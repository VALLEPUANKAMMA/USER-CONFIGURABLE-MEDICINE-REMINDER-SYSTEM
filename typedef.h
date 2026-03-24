/*
 * ============================================================
 * FILE: typedef.h
 * Author: Ankamma V
 * DESCRIPTION:
 * User-defined data type aliases for better readability
 * and portability in embedded systems.
 *
 * PURPOSE:
 * - Simplifies complex data type names
 * - Ensures consistency across the project
 * - Improves code readability and maintenance
 * ============================================================
 */
#ifndef _TYPEDEF_
#define _TYPEDEF_
/*
 * ------------------------------------------------------------
 * Unsigned 32-bit integer
 * Range: 0 to 4,294,967,295
 * Commonly used for counters, registers, addresses
 * ------------------------------------------------------------
 */
typedef unsigned int u32;

/*
 * ------------------------------------------------------------
 * Signed 32-bit integer
 * Range: -2,147,483,648 to 2,147,483,647
 * Used when negative values are required
 * ------------------------------------------------------------
 */
typedef signed int s32;

/*
 * ------------------------------------------------------------
 * Unsigned 8-bit integer (1 byte)
 * Range: 0 to 255
 * Used for characters, small data, flags
 * ------------------------------------------------------------
 */
typedef unsigned char u8;

/*
 * ------------------------------------------------------------
 * Signed 8-bit integer (1 byte)
 * Range: -128 to 127
 * ------------------------------------------------------------
 */
typedef signed char s8;

/*
 * ------------------------------------------------------------
 * Unsigned 16-bit integer (2 bytes)
 * Range: 0 to 65,535
 * ------------------------------------------------------------
 */
typedef unsigned short int u16;

/*
 * ------------------------------------------------------------
 * Signed 16-bit integer (2 bytes)
 * Range: -32,768 to 32,767
 * ------------------------------------------------------------
 */
typedef signed short int s16;

/*
 * ------------------------------------------------------------
 * 32-bit floating point number (single precision)
 * Used for decimal values
 * ------------------------------------------------------------
 */
typedef float f32;

/*
 * ------------------------------------------------------------
 * 64-bit floating point number (double precision)
 * Used for high precision decimal values
 * ------------------------------------------------------------
 */
typedef double d64;

#endif
