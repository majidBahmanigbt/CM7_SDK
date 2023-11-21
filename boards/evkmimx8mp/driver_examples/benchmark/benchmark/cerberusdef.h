/* Copyright (c) EFACEC Sistemas de Electronica, S.A.
 * All rigths reserved.
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF EFACEC Sistemas de Electronica.
 */

/**
 * Standard definitions for Cerberus framework.
 *
 * @file 	Cerberusdef.h
 * @author  Joao Rosa Dias
 * @version 1.0.0 - 2008/12/05 - First release.
 */

#include "stdbool.h"
#include "fsl_gpio.h"

#define TEST_GPIO     GPIO5
#define TEST_GPIO_PIN 8U

#ifndef _CERBERUSDEF_
#define _CERBERUSDEF_

#ifndef NULL
	#define NULL	0
#endif


#undef TRUE
#define TRUE true
/** Define FALSE used in boolean (BOOL) type */
#undef FALSE
#define FALSE false



#ifdef __i386__
#undef TRUE
#define TRUE (1)

#undef FALSE
#define FALSE (0)
#endif

// Types definitions

/** framework types: BOOL*/
#define BOOL	bool
/** framework types: 8 bits signed*/
#define INT8	signed char
/** framework types: 16 bits signed*/
#define INT16	signed short
/** framework types: 32 bits signed*/
#ifndef uCLinux
#define INT32	int
#else 
#define INT32	long int
#endif 
#define LONG	INT32
/** framework types: 64 bits signed*/
#define INT64	long long
/** framework types: 8 bits unsigned*/
#define UINT8	unsigned char

/** framework types: 16 bits unsigned*/
#define UINT16	unsigned short

/** framework types: 32 bits unsigned*/
#ifndef uCLinux
#define UINT32	unsigned int
#else
#define UINT32	unsigned long int
#endif

/** framework types: 64 bits unsigned*/
#define UINT64	unsigned long long
/** framework types: float 32 bits*/
#define FLOAT32 float
/** framework types: float 64 bits*/
#define FLOAT64 double
/** Cerberus framework types: float 16 bits*/
#define BFLOAT16 unsigned short
/** framework types: signed char*/
#define CHAR	char

/** Cerberus framework types: GPIO Value*/
#define LOW		(0) 
#define HIGH	(1)

// Bit definitions
/** Bit definitions: bit 0 */
#define SyBIT0      0x1
/** Bit definitions: bit 1 */
#define SyBIT1      0x2
/** Bit definitions: bit 2 */
#define SyBIT2      0x4
/** Bit definitions: bit 3 */
#define SyBIT3      0x8
/** Bit definitions: bit 4 */
#define SyBIT4      0x10
/** Bit definitions: bit 5 */
#define SyBIT5      0x20
/** Bit definitions: bit 6 */
#define SyBIT6      0x40
/** Bit definitions: bit 7 */
#define SyBIT7      0x80
/** Bit definitions: bit 8 */
#define SyBIT8      0x100
/** Bit definitions: bit 9 */
#define SyBIT9      0x200
/** Bit definitions: bit 10 */
#define SyBIT10     0x400
/** Bit definitions: bit 11 */
#define SyBIT11     0x800
/** Bit definitions: bit 12 */
#define SyBIT12     0x1000
/** Bit definitions: bit 13 */
#define SyBIT13     0x2000
/** Bit definitions: bit 14 */
#define SyBIT14     0x4000
/** Bit definitions: bit 15 */
#define SyBIT15     0x8000
/** Bit definitions: bit 16 */
#define SyBIT16     0x10000
/** Bit definitions: bit 17 */
#define SyBIT17     0x20000
/** Bit definitions: bit 18 */
#define SyBIT18     0x40000
/** Bit definitions: bit 19 */
#define SyBIT19     0x80000
/** Bit definitions: bit 20 */
#define SyBIT20     0x100000
/** Bit definitions: bit 21 */
#define SyBIT21     0x200000
/** Bit definitions: bit 22 */
#define SyBIT22     0x400000
/** Bit definitions: bit 23 */
#define SyBIT23     0x800000
/** Bit definitions: bit 24 */
#define SyBIT24     0x1000000
/** Bit definitions: bit 25 */
#define SyBIT25     0x2000000
/** Bit definitions: bit 26 */
#define SyBIT26     0x4000000
/** Bit definitions: bit 27 */
#define SyBIT27     0x8000000
/** Bit definitions: bit 28 */
#define SyBIT28     0x10000000
/** Bit definitions: bit 29 */
#define SyBIT29     0x20000000
/** Bit definitions: bit 30 */
#define SyBIT30     0x40000000
/** Bit definitions: bit 31 */
#define SyBIT31     0x80000000


//Quality defines according to IEC 61850
#define VALIDITY_MASK 		(0x0003)
#define QUEST_QUAL 			(0x0003)
#define INVALID_QUAL		(0x0001)
#define ISOLATED_BIT_QUEST	(0x0002)
#define VALID_QUAL			(0x0000)
#define DETAIL_QUAL_BAD_REF	(0x0010) //bit4
#define DETAIL_QUAL_OLDDATA	(0x0080) //bit7
#define QUALITY_TEST_MASK 	(0x0800) //Bit 11
#define QUALITY_OPER_BLK    (0x1000) //Bit 12
#define INVALID_QUAL_AND_FAILURE (0x0041)
#define INVALID_QUAL_AND_BAD_REF     (0x0011)

#define OVERFLOW_QUAL		(0x0004)
#define OUTOFRANGE_QUAL		(0x0008)
#define OSCILLATORY_QUAL	(0x0020)
#define FAILURE_QUAL		(0x0040)
#define INCONSISTENCY_QUAL	(0x0100)
#define INACCURANTE_QUAL	(0x0200)
#define PROCESS_SUBS_QUAL	(0x0400)
#define MASK_IGNORED_INPUT  (0xE000)

#define INCONSISTENT_QUAL	(SyBIT8)

#define QUAL_STD_BITS_MASK	0x3FFF	// The two most significant bits are not standard - they're for internal use only. 

#define PMU_COORD_INFO (0x4000)


// Bit mask tests
// CAUTION: DONOT change them...
/** This macro indicates if any bit(s) of \a l is set in \a c.
* @return TRUE if any bit(s) is set; otherwise FALSE.
*/
#define isset(c,l)          (((c)&(l))!=0)              // any bit(s) set

/** This macro indicates if all bits of \a l is set in \a c.
* @return TRUE if all bits are set; otherwise FALSE.
*/
#define ismask(c,l)         (((c)&(l))==(l))          // all bit(s) set

/** This macro indicates if the \a m bits of \a l are set in \a c.
* @return TRUE if \a m bits are set; otherwise FALSE.
*/
#define isinmask(c,l,m)     (((c)&(l))==(m))            // some bit(s) set

/** This macro indicates if all bits of \a l are cleared in \a c.
* @return TRUE if all bits are cleared; otherwise FALSE.
*/
#define isclear(c,l)        (((c)&(l))==0)              // bit(s) cleared

/** This macro indicates if the value \a c is between \a l and \a r.
* @return TRUE if the value is in range; otherwise FALSE.
*/
#define inrange(c,l,r)      (((c)>=(l))&&((c)<=(r)))    // value within range

/** This macro indicates if the value \a c is odd.
* @return TRUE if the value is odd; otherwise FALSE.
*/
#define isodd(c)            isset((c),SyBIT0)               // bit 0 set

/** This macro indicates if the value \a c is even.
* @return TRUE if the value is even; otherwise FALSE.
*/
#define iseven(c)           isclear((c),SyBIT0)             // bit 0 cleared

/** This macro indicates if the value \a c is the next number of \a l.
* @return TRUE if the value \a c is the next number of \a l; otherwise FALSE.
*/
#define isnext(c,l)         ((c)==(l+1))                // sucessor

/** This macro indicates if the value \a c is the previous value of \a l.
* @return TRUE if the value \a c is the previous number of \a l.
*/
#define isprevious(c,l)     ((c)==(l-1))                // antecessor


// Bit mask assigns
// CAUTION: DONOT change them...
/** This macro sets the bit(s) \a l in \a c. */
#define set(c,l)            ((c)|=(l))                  // bit(s) set

/** This macro clears the bit(s) \a l in \a c.*/
#ifndef __i386__
#define clear(c,l)          ((c)&=~(l))                 // bit(s) cleared
#endif

/** This macro toggles bit(s) \a l in \a c. */
#define toggle(c,l)         ((c)^=(l))                  // bit(s) changed


// Max min tests
/** The max macro retrieves the maximum number of \a a and \a b. */
#ifndef __i386__
#define max(a, b)  (((a) > (b)) ? (a) : (b))
#endif

/** The min macro retrieves the minimum number of \a a and \a b. */
#ifndef __i386__
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#define FFT_ARRAY_SIZE 40		// TODO: this is the same as FFT_SIZE


typedef struct _RECEIVECE_CTRL {
    UINT16 RdIdx;
    UINT16 SubscriptionFailure;
    UINT16 Reserved1;
} RECEIVECE_CTRL;

typedef struct _SEND_CTRL {
	UINT16 Ovrlpd;
    UINT16 WrIdx;
    UINT16 Full;
    UINT16 NumLastElementsWritten;
	UINT16 NewData;
} SEND_CTRL;

typedef struct _CtrlPMU {
    RECEIVECE_CTRL rcvCtrl;
    SEND_CTRL sndCtrl;
} CtrlPMU;

typedef enum{
	SYNC_SOURCE_NONE = 0,
	SYNC_SOURCE_LOCAL,
	SYNC_SOURCE_GLOBAL
}SYNC_SOURCE;

typedef struct tagPHASOR {
	FLOAT32 Re;
	FLOAT32 Im;
	FLOAT32 Magnitude;
	FLOAT32 Angle;
	FLOAT32 Rms;
	FLOAT32 Period;
} PHASOR;

typedef struct tagTimestampPMU {
	UINT32 Days;
	UINT32 SecDay;
	UINT32 MicroSec;
	UINT16 SyAttrs;
} TimestampPMU;

typedef struct _PhasorPMU {
    UINT16 Id;
    FLOAT32 Magnitude;
    FLOAT32 Angle;
    UINT16 Quality;
    TimestampPMU Timestamp;
} PhasorPMU;


typedef struct _HarmonicPMU {
    UINT16 Id;
    BFLOAT16 Magnitude[FFT_ARRAY_SIZE];
    INT16 Angle[FFT_ARRAY_SIZE];
    UINT16 Quality;
    TimestampPMU Timestamp;
} HarmonicPMU;


typedef struct tagRMSCalculation {
	UINT8		Enable;
	UINT8		Size;
	FLOAT32		Rms;
	FLOAT32		MinMagStart;
	FLOAT32		MinMagReset;
	FLOAT32*	SrcAddr;
	PHASOR*		DstPhasor;
}RMSCalculation;

typedef struct tagFREQ {
	FLOAT32		Value;
	UINT16		Quality;
} FREQ;

typedef struct tagSYNC {
	UINT32		SmpSync;
	UINT16		SmpSyncQual;
	UINT32		SyncSrcID;
	UINT16		SyncSrcIDQual;
} SYNC;

typedef struct tagPROP {
	FREQ			Frequency;
	SYNC			Synchronism;
	TimestampPMU	TimeStamp;
	UINT32			ChangeCounter;
} PROP;

#define NORMAL_RUN 			0x00
#define FAULT_DETECTED 		0x01
#define FAULT_RESET			0x02
#define COORDINATE_RUN		0x03
#define FAST_RUN			0x04
#define ARC_FAULT_RUN		0x05
#define ARC_OC_RUN			0x06
#define ARC_OC_LIGHT_RUN    0x07
#define ISR_ERROR_REM		0x08
#define ISR_ERROR_MCASP		0x09
#define PMU_NORMAL_RUN		0x10
#define EXT_PHASOR_CALC_RUN     0x11
#define EXT_PHASOR_CALC_VA      0x12
#define EXT_PHASOR_CALC_VB      0x13
#define EXT_PHASOR_CALC_VC      0x14
#define EXT_PHASOR_CALC_VAB     0x15
#define EXT_PHASOR_CALC_VBC     0x16
#define EXT_PHASOR_CALC_VCA     0x17
#define EXT_PHASOR_CALC_CA      0x18
#define EXT_PHASOR_CALC_CB      0x19
#define EXT_PHASOR_CALC_CC      0x1A
#define PMUADV_RUN              0x1B
#define PMUADV_RUN_INST1        0x1C
#define PMUADV_RUN_INST2        0x1D
#define PMUADV_RUN_INST3        0x1E
#define PMUADV_RUN_INST4        0x1F
#define PMUADV_RUN_INST5        0x20

#define ISR_ERROR_SENT	    0xFF


typedef struct tagSyriusTime{
	UINT32	Days;
	UINT32	Ms;
	UINT16  Us;
	UINT16	Attrs;
}SyTime;

#define CE_ATTRS_TIME_RUNNING	SyBIT0		// This attribute is not set during initialization and is set when the time starts being updated. 
#define CE_ATTRS_TIME_DIFF_SIGN	SyBIT15		// This attribute is used when the CeTime struct represents a time difference. This bit indicates whether the difference is positive (bit = 0) or negative (bit = 1).

typedef struct tagCerberusTime{
	UINT32	Days;
	UINT32	Ms;
	UINT16  Us;
	UINT16	SyAttrs;
	UINT16	CeAttrs;
	UINT16  SyncStatus; //used to know if FPGA is locked. Only used between FPGA and DSP
}CeTime;

typedef struct _PmuSlaveCeTime{
    CeTime Time;
    UINT32 CRC;
}PmuSlaveCeTime;

typedef struct tagSharedSyCeTime{
	CeTime CerberusTime;
	UINT32 TimeCounterReg;
}SharedSyCeTime;

//Buffer priority
#define PRIORITY0           0x01
#define PRIORITY1           0x02
#define PRIORITY2           0x03



#define BAP_MAX_NUM_ACTIONS		2

#endif
