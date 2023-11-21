#ifndef _BENCHMARK_h_
#define _BENCHMARK_h_

#include "cerberusdef.h"

#define DSP_FIRMWARE_VERSION	1.0
#define DSP_FIRMWARE_DATE		1.1.1

#define MAX_BASE_GROUPS						(30)
#define MAX_DERIVED_GROUPS					(0)
#define MAX_COORD_GROUPS					(0)	//(2 LineDif + 3 FltLocAdv + 2*2 CTSup)
#define MAX_SECOND_ESTIMATION_GROUPS 		(0) // for the distance current groups (1 Dist * 2 Groups * (2 base + 1 sum)for sum groups)
#define MAX_GROUPS        (30)//(MAX_BASE_GROUPS + MAX_DERIVED_GROUPS + MAX_COORD_GROUPS + MAX_SECOND_ESTIMATION_GROUPS)


#define MAX_CALCULATED_SAMPLES 				(30) 	//max derived for 24 local quantities 
	// 12I+12U	->	4 Iabc + 4Vabc	->	4 Ires + 2 Sum Iabc + 2 Sum Ires + 4 Vab,bc,ca + 4 Vres -> 28
	// 24I		->	8 Iabc			->	8 Ires + 4 Sum Iabc + 4 Sum Ires						-> 24
	
#define MAX_CALCULATED_FREQ_DERIVED (0u)
	// maximum number of internally calculated voltage ph-ph with frequency estimation: 3 Ph-E voltages groups (maximum
	// is 4 but is not realistic)

#define MAX_AC_QUANTITIES_PLUS_CALCULATED 	(60u)//(MAX_AC_LOCAL_QUANTITIES + MAX_CALCULATED_SAMPLES) calculated.
#define MAX_NUM_CH							(0)
#define MAX_REMOTE_AC_QUANTITIES			(0)

#define MAX_AC_LOCAL_QUANTITIES (30u)
#define MAX_AC_QUANTITIES    (30u)

#define	UNUSED_GROUP		0x00
#define UNUSED_INPUT		0xFF

#define MAX_CALCULATED_COORDSAMPLES 		(0u) 	//max coordderived calulated quantities
#define MAX_COORDSAMPLES_PLUS_CALCULATED	(MAX_AC_QUANTITIES + MAX_CALCULATED_COORDSAMPLES)

#define P1_TRIP_TIME	1
#define P2_TRIP_TIME	2
#define P3_TRIP_TIME	4
#define P4_TRIP_TIME	10
#define BASE_RUN_TASK	5

#define MAX_P1_FUNCTIONS	20	//12 is already in use			
#define MAX_P2_FUNCTIONS	12  //11 is already in use
#define MAX_P3_FUNCTIONS	50  //44 is already in use
#define MAX_P4_FUNCTIONS	16  //13 is already in use
#define MAX_P1AUX_FUNCTIONS 5 //3 is already in use

#define MAX_FUNCTIONS (MAX_P1_FUNCTIONS+MAX_P2_FUNCTIONS+MAX_P3_FUNCTIONS+MAX_P4_FUNCTIONS+MAX_P1AUX_FUNCTIONS)
#define MAX_DATABASE_SIZE 2500 // This value must be reviewed each time we add a new function to the product.

// LineDif and inter substations communication
#define MAX_USER_DIGITAL			(32)
#define MAX_SAMPLES_MESSAGE			(6)
#define MAX_COM_DELAY				(100000)

#define MAX_SAMPLES_REM				(60)	// Number of buffered raw samples, available for disturbance recorder, from remote quantities

//Defines to sincronize the dsp and the isc board
#define ISC_DSP_READY				(0x0001)  
#define ISC_INIT_RECEIVED			(0x0003) //INIT_RECEIVED is bit 2, but will always be set with DSP_READY

//******************************************************************************
//
// ACQUISITION SYSTEM DEFINES (Used in CeEstimation and CePAL(ISR))
//
//******************************************************************************

#define MAX_SWITCH_SUM_NUM           (3) 
#define SAMPLE_WINDOW_SIZE					(60)	// Size of historic filtered sample windows, available for algorithms
#define LAST_CYCLE_WINDOW_SIZE				(45)	// Size of last cycle filtered sample windows, available for algorithms
#define LAST_CYCLE_RAW_WINDOW_SIZE			(50)	// Size of last cycle unfiltered sample windows, available for algorithms
#define LAST_CYCLE_RAW_BUFFER_SIZE			(LAST_CYCLE_RAW_WINDOW_SIZE*2)

#define QUARTER_CYCLE_WINDOW		 		(5)
#define HALF_CYCLE_WINDOW					(10)
#define ONE_CYCLE_WINDOW					(20)
#define FIVE_QUARTER_CYCLE_WINDOW			(25)
#define THREE_HALF_CYCLE_WINDOW				(30)
#define TWO_CYCLE_WINDOW					(40)
#define THREE_CYCLE_WINDOW					(60)
#define FOUR_CYCLE_WINDOW					(80)
#define ONE_CYCLE_RAW_WINDOW				(40)

#define MAX_GENERAL_QUAL_ELEM				(5) //1.25 cycles

#define AC_LP_HP_FILTER_ORDER 				(2)

#define BLOCK_FREQUENCY_CALCULUS 				(TWO_CYCLE_WINDOW) //Blocks ISR frequency calculus after fault 
#define BLOCK_APP_FREQ_READING					(ONE_CYCLE_WINDOW) //FreqOK() for app only after BLOCK_FREQUENCY_CALCULUS + BLOCK_APP_FREQ_READING
#define TOTAL_BLOCK_FREQUENCY_AFTER_FAULT		(BLOCK_FREQUENCY_CALCULUS+BLOCK_APP_FREQ_READING)
#define BLOCK_FREQUENCY_CALCULUS_AFTER_SHIFT 	(40)
#define TOTAL_BLOCK_FREQUENCY_AFTER_SHIFT		(BLOCK_FREQUENCY_CALCULUS_AFTER_SHIFT+BLOCK_APP_FREQ_READING)
#define BLOCK_FREQUENCY_CALCULUS_AFTER_INV	 	(30)	//quality invalid (process bus)
#define TOTAL_BLOCK_FREQUENCY_AFTER_INV			(BLOCK_FREQUENCY_CALCULUS_AFTER_INV+BLOCK_APP_FREQ_READING)

/* MA: Calc of Period Variation Threshold.
For 0.1Hz tolerance at 1kHz and 50Hz System --> (1000/(50 - 0.1)) - 20  = 0.04008 --> rounded up to 0.045
For 0.1Hz tolerance at 4kHz and 50Hz System --> (4000/(50 - 0.1)) - 80  = 0.16032 --> rounded up to 0.165
*/
#define MAX_PERIOD_VARIATION \
    (0.165f)  //(0.045f)		 //variation (in SamplingVars.SamplingPeriod (sec)) between consecutive zero cossings ->
              //period'
//@50Hz -> 0.1hz jump at 50 (before average)
//@60Hz -> 0.12hz jump at 60 (before average)

/*
Delta Period Variation does not change because it's a division of two Period Variation. Hence, the multiplication with 4
is neglected.
*/
#define MAX_DELTA_PERIOD_VARIATION (0.4f)  // pu variation between consecutive period variations

#define FREEZE_FREQ_UPDATE_COUNT           (2)  // counter in zero crossings - after jump in frequency calculus
#define FREEZE_FREQ_UPDATE_COUNT_AFTER_INV (4)  // counter in zero crossings - after invalid samples
// Freeze frequency update after high delta period and high pu variation between consecutive period variations

#define INVALID_PERIOD (-1.0f)
// definition of invalid period: used directly on phasor period - always distinguishable from a real calculated value.
#define COUNT_INV_VALID_PERIOD (100)
#define INVALID_IDX            (0xFF)

#define CURRENT_FAULT_DETECTION_FACTOR 		(0.5f)	//pu per cycle
#define CURRENT_STABILITY_DETECTION_FACTOR	(0.1f)	//pu per cycle
#define VOLTAGE_FAULT_DETECTION_FACTOR		(0.2f)	//pu per cycle
#define VOLTAGE_STABILITY_DETECTION_FACTOR	(0.05f)	//pu per cycle
#define FAULT_DETECTION_SLOP				(0.40f)
#define FAULT_STABILITY_SLOP				(0.35f)
#define FAULT_DETECTOR_MIN_FREQ_50			(49.8f)	//Hz
#define FAULT_DETECTOR_MAX_FREQ_50			(50.2f)	//Hz
#define FAULT_DETECTOR_MIN_FREQ_60			(59.8f)	//Hz
#define FAULT_DETECTOR_MAX_FREQ_60			(60.2f)	//Hz
#define FAULT_DETECTOR_DB_FREQ				(0.05f)	//Hz

#define VOLTAGE_MAG_FREQ_THRESHOLD			(0.04f) 

#define MIN_MAG_FILTER_COMP_NORMAL			(0.02f) // Minimal Magnitude to allow frequency compesation of the current HP filter
#define MIN_MAG_FILTER_COMP_SENSITIVE		(0.002f)// Minimal Magnitude to allow frequency compesation of the current HP filter
#define MIN_MAG_FILTER_COMP_NORMAL_RESET	(0.03f) // Minimal Magnitude reset (dead band)
#define MIN_MAG_FILTER_COMP_SENSITIVE_RESET	(0.003f)// Minimal Magnitude reset (dead band)

// Used on RMS Calculation
#define MIN_MAG_RMS_START					(0.03f) // Minimal Magnitude to allow frequency window calculation on RMS
#define MIN_MAG_RMS_RESET					(0.02f) // Minimal Magnitude reset (dead band)

#define ONE_SECOND							(1000000)
#define ONE_MILISEC							(1000)
#define HALF_MILISEC						(500)
#define QUARTER_MILISEC						(250)
#define LAST_MILISECOND						(998500)

#define MEAS_DIAG_SEC_DB_CURRENT	(0.05f)	//A sec
#define MEAS_DIAG_SEC_DB_VOLTAGE	(0.05f)	//V sec

// AC analogue input types
#define	AC_CURRENT_INPUT			(0xCC)
#define AC_VOLTAGE_INPUT			(0xA7)
#define INVALID_AC_INPUT_INDEX		(0xFF)
// Additional AC input information
#define AC_INFO_NONE        		(0x00)
#define AC_INFO_SENSITIVE  	 		(0x55)
#define AC_INFO_RESISTIVE    		(0x17)

#define PB_MAX_SVCB 8

//#define MAX_SAMPLES_VARIATION_PER_HALF_CYCLE	(0.08f) JP: 50hz=0.2Hz/Cycle (variation near nominal frequency) (10Hz/s) 60hz(aprox=)0.240Hz/Cycle (variation near nominal frequency) (>10Hz/s)
//Should be this but practicaly the error variation can be to large so the limit is incremented
#define MAX_SAMPLES_VARIATION_PER_HALF_CYCLE	(0.3f)

#define RAW_SAMPLES_PER_ISR						(4u)

#define FILTER_AVG8 (1.004059685f)
#define FILTER_AVG4 (1.003866143f)

#define CURRENT_FILTER_RE_COMP	 0.15643446504023f	//cos(-81)
#define CURRENT_FILTER_IM_COMP	-0.98768834059514f	//sin(-81)
#define CURRENT_FILTER_ANG_COMP	-1.41371669411541f	//(-81)
#define SEC_HARM_CURR_FILTER_ANG_COMP -1.2566370614359f //(-72)	 
#define FIFTH_HARM_CURR_FILTER_ANG_COMP -0.78539816339744f //(-45)
#define CURRENT_FILTER_2h_RE_COMP 0.15835394783f// (1/1.95143221)*cos(-72)
#define CURRENT_FILTER_2h_IM_COMP -0.48736333828f// (1/1.95143221)*sin(-72)
#define CURRENT_FILTER_5h_RE_COMP 0.172774663602f//(1/4.092653207)*cos(-45)
#define CURRENT_FILTER_5h_IM_COMP -0.172774663602f //(1/4.092653207)*sin(-45)

#define VOLTAGE_FILTER_ANG_COMP	0
#define VOLTAGE_FILTER_RE_COMP	1	   //cos(0)
#define VOLTAGE_FILTER_IM_COMP	0	   //sin(0)
#define VOLTAGE_FILTER_2h_RE_COMP	1	   //cos(0)
#define VOLTAGE_FILTER_2h_IM_COMP	0	   //sin(0)
#define VOLTAGE_FILTER_5h_RE_COMP	1	   //cos(0)
#define VOLTAGE_FILTER_5h_IM_COMP	0	   //sin(0)	 

#define RAW_SAMPLES_PER_ISR						(4u)


#define FAULT_DETECTOR_MAX_TIME 			(5000)	//fault detector max time in ms (after the status remains stable)
#define FAULT_DETECTOR_STABILITY_COUNT  	(TWO_CYCLE_WINDOW)//number of samples considered stable before reset

#define TIMESTAMPSIZE						(1u)

#define MIN_PERIOD_COMPENSABLE		(16.5f)	//50Hz->60.6Hz 60Hz->72.7Hz
#define MAX_PERIOD_COMPENSABLE		(25.5f) //50Hz->39.2Hz 60Hz->47.1Hz

#define CONST_ONE_THIRD 					(0.33333333f)

//10 samples
#define mA10 			1.000000f
#define mB10 			1.618034f
#define mC10 			1.175571f
#define mD10 			1.902113f

//20 samples without compensation
#define K20				0.1f
#define mA20 			0.0951056516f
#define mB20 			0.0809016994f
#define mC20 			0.0587785252f
#define mD20 			0.0309016994f

#define mE20 			0.0309016994f
#define mF20 			0.0587785252f
#define mG20 			0.0809016994f
#define mH20 			0.0951056516f
//20 samples with compensation just a rotation of -81 degrees caused by the current filter
//This DFT can only be used in currents and should only be used inside functions that have the necessity
//to invoke DFT to calculate for example diferential currents phasors. 
#define C20				0.01564344650f
#define cA20 			0.04539904997f
#define cB20			0.07071067812f
#define cC20 			0.08910065242f
#define cD20 			0.09876883406f


#define GROUP_BUFFER_SIZE			4

typedef enum{
	DSC_TYPE_DISABLED = 0,
	DSC_TYPE_RESIDUAL_FROM_BASE,// 1+2+3
	DSC_TYPE_RESIDUAL_FROM_DERIVED,// 1+2+3
	DSC_TYPE_VOLTAGE_PN2PP,		// 1-2
	DSC_TYPE_VOLTAGE_PP2PP,		// -1-2
	DSC_TYPE_GROUP_SUM,			// 1+2
	DSC_TYPE_GROUP_SWITCH_SUM   //sel1*(1) + sel2*(2)
}DerivedSamplesCalculation;

typedef struct tagRcvDataPrBus {
	UINT32 CRC;
	UINT8 MsgType;
	UINT8 dummy;
	UINT16 DeltaT;
	UINT32 TimeStamp;
	UINT32 SmpCnt;
	INT32 Smp[RAW_SAMPLES_PER_ISR][MAX_AC_LOCAL_QUANTITIES];
	INT32 SmpAv[MAX_AC_LOCAL_QUANTITIES];
	UINT16 Qual[RAW_SAMPLES_PER_ISR][MAX_AC_LOCAL_QUANTITIES];
	UINT16 QualAv[MAX_AC_LOCAL_QUANTITIES];
} RCV_DATA_PR_BUS;

//The Historic Buffer has the samples summed but not filtered. It must be oversized
// by AC_LP_HP_FILTER_ORDER-1. The SamplePosition index is (AC_LP_HP_FILTER_ORDER-1) behaind the real position
// for simplicity is managing the TimeStamp buffer and the buffer overlap
typedef struct tagSAMPLEWINDOW {
	/*Historic:
	 	1 kHz, not filtered (only 8 average), SAMPLE_WINDOW_SIZE used, 
	 	AC_LP_HP_FILTER_ORDER-1 extended for data continuity (Last cycle filtering) 
	*/
	FLOAT32 Historic[MAX_AC_LOCAL_QUANTITIES][SAMPLE_WINDOW_SIZE + AC_LP_HP_FILTER_ORDER-1];

	UINT16 HistoricQual[MAX_AC_LOCAL_QUANTITIES][SAMPLE_WINDOW_SIZE + AC_LP_HP_FILTER_ORDER-1];


	/*HistoricDerived:
	 	1 kHz, not filtered (only 8 average), SAMPLE_WINDOW_SIZE used, 
	 	no extension for data continuity (Last cycle filtering only for base quantity) 
	*/
	FLOAT32 HistoricDerived[MAX_CALCULATED_SAMPLES][SAMPLE_WINDOW_SIZE];

	UINT16 HistoricDerivedQual[MAX_CALCULATED_SAMPLES][SAMPLE_WINDOW_SIZE];

	/*LastCycleRaw:
	 	2 kHz, not filtered, LAST_CYCLE_RAW_WINDOW_SIZE used, 
	 	LAST_CYCLE_RAW_WINDOW_SIZE extended for data continuity (RMS calculus) 
	*/
	FLOAT32 LastCycleRaw[MAX_AC_QUANTITIES_PLUS_CALCULATED][LAST_CYCLE_RAW_BUFFER_SIZE];
/*#ifdef FEATURE_QUALITY_AC
	UINT16 LastCycleRawQual[MAX_AC_QUANTITIES_PLUS_CALCULATED][LAST_CYCLE_RAW_BUFFER_SIZE];
#endif*/
	/*LastCycle:
	 	1 kHz, filtered (HP for current, LP for voltage), ONE_CYCLE_WINDOW used, 
	 	ONE_CYCLE_WINDOW extended for data continuity (Fast DFT - only for base quantity) 
	*/
	FLOAT32 LastCycle[MAX_AC_LOCAL_QUANTITIES][LAST_CYCLE_WINDOW_SIZE];

	UINT16 LastCycleQual[MAX_AC_LOCAL_QUANTITIES][LAST_CYCLE_WINDOW_SIZE];

	/*LastCycleDerived:
	 	1 kHz, filtered (HP for current, LP for voltage), ONE_CYCLE_WINDOW used, 
	 	no extension for data continuity (Fast DFT - only for base quantity) 
	*/
	FLOAT32 LastCycleDerived[MAX_CALCULATED_SAMPLES][ONE_CYCLE_WINDOW];

	UINT16	LastCycleDerivedQual[MAX_CALCULATED_SAMPLES][ONE_CYCLE_WINDOW];

	/*General quality for phasors:
		It is majorated to protect DFT, fast DFT, RMS and harmonics: 1.25 cicles 
	 	Uses quality from Last Cycle to ensured wors case (filter average plus HP/LP), 
		Merged every calculate AC over the new samples (5 samples or less during fast DFT)
	 	General quality for estimation is the merging off all the elements in the buffer. 
	*/
	UINT16 GeneralQual[MAX_AC_LOCAL_QUANTITIES][MAX_GENERAL_QUAL_ELEM];

	UINT32  TimeStamp[SAMPLE_WINDOW_SIZE];									// Timestamps for historic buffer
} SAMPLEWINDOW;

typedef struct tagSAMPLINGVARS {
	INT32 SamplePosition;					// Current sample position in waveform windows
	INT32 LastSamplePosition;				// last sample already in Historic window (one behind SamplePosition)
//	INT32 OneCycleBefore;					// Sample one cicle before current sample (points to real sample position)
	INT32 SampleIndex;						// Sample index relative to cosine and sine filters
	INT32 RawSampleIndex;					// Current raw buffer sample position
	INT32 RawSampleIndexNext;
	INT32 LastSampleIndex;					// Sample index one instant before (for zero-crossing detection)
	INT32 AwakeCounter;						// Counter for signalling estimation thread periodically
	UINT8 FaultDetectorActive;				// Fault detector active (enabled)
	INT32 FaultDetectorSampleIndex;			// Sample position where fault was detected (in LastCycle)
	INT32 FaultDetectorHistoricSample;		// Sample position where fault was detected (in Historic)
	INT32 FaultDetectorStatus;				// Fault detected / Number of samples since fault detector active (only for the first cycle)
	INT32 StabilityCounter;					// To rearm fault detector
	UINT8 StartFilterDelay;
	INT32 FreqInitCounter;
	FLOAT32 SamplingPeriod;
	INT32 SampleCopy;						// Next sample to be copied from Historic to RemMostRecent
	UINT8 UsedSampleBuffers;				//Counts the used calculated sample buffers.
	UINT8 UsedFreqVars;						//Counts the used calculated frequency from derived calculus.
	FLOAT32 RatedPeriod;					//Rated period in ms
	UINT8 ISRError;							//Bad error stopping Cerberus
	FLOAT32 FaultDetectorFreqLimit[4];		//for threshold adjust
	UINT8 AcquisitionType;					//0 for Local; 1 for ProcessBus
	UINT16 USecCounter;						//Microseconds counter;
	UINT16 ACshift_High;					//high limiar for AC phase shift based on the microseconds counter;
	UINT16 ACshift_Low;						//low limiar for AC phase shift based on the microseconds counter;
    // 4k Frequency Estimation
    // ***************************************************************************
    INT32 LastSample4k[MAX_AC_LOCAL_QUANTITIES];
    FLOAT32 PeriodMemMovAvg[MAX_AC_LOCAL_QUANTITIES][2];  // Storing Moving Average of periods.
    //******************************************************************************

	UINT8 ResetGeneralQual;				//Flag to reset next element in general quality buffer.
	UINT32 GeneralQualIndex;				//Index for general quality buffer.

} SAMPLINGVARS;

typedef struct tagACQUANTITYCONFIG {
	UINT8 	Type;							// Input type (current or voltage)
	UINT8	FilterType;						// For ISR filter decision (0 - Uses Low Pass (Voltages) 1 - Uses High Pass (Currents))
	UINT8	TypeInfo;						// Additional information (sensitive, low voltage, ...)
	UINT8	Origin;							// Channel Origin (0 - McASP; 1 - ProcessBus)
	union {
		INT32	Offset;							// Zero offset (for filtered buffers) a(x+b)
		FLOAT32	PrBusOffset;					// Offset for process bus acquisition (ax+b)
	}Offset;
	INT32	RawOffset;						// Zero offset (for Raw buffer)
	FLOAT32	HighScale;						// High hardware scale
	FLOAT32	LowScale;						// Low hardware scale
	UINT8	ConfiguredScale;				// Configured hardware scale (high/low)
	FLOAT32	SecondaryRatedValue;			// Secondary rated value
	FLOAT32 Ratio;							// External CT or VT ratio
	FLOAT32 MagnCorr;						// Magnitude correction factor (for rated value)
	FLOAT32 PhaseCorr;						// Phase correction factor in degrees (for rated value)
	FLOAT32 RePhaseComp;					// Real part of phase compensation phasor
	FLOAT32 ImPhaseComp;					// Imaginary part of phase compensation phasor
	FLOAT32 PrimaryConversion;				// Primary conversion factor (internal scale, ratio and correction factor)
	FLOAT32 RawPrimaryConversion;				// Primary conversion factor (internal scale, ratio)
	FLOAT32 PrimaryConversionComp;			// Primary conversion factor compensated by frequency
	UINT8 	FaultDetectorActive;			// Inputs eligible for fault detecting (application decision)
	FLOAT32 DeltaFaultDetectorStart;		// Variation that leads to fault detection (primary value)
	FLOAT32 DeltaFaultDetectorReset;		// Variation that indicates stability (primary value)
	UINT8 	FaultDetected;					// Fault detected in this input
	UINT8   HalfCycleCounter;				// Number of samples since last zero-crossing
	UINT8   OneCycleCounter;				// Number of samples since last zero-crossing with the same derivative signal
	FLOAT32 HalfCycleBeforeZeroTime;		// Interpoled zero-crossing time (half cycle before)
	FLOAT32 OneCycleBeforeZeroTime;			// Interpoled zero-crossing time (one cycle before)
	FLOAT32 MinMagFilterComp;				// Minimal Magnitude to allow frequency compesation of the current HP filter
	FLOAT32 MinMagFilterCompReset;			// Minimal Magnitude dead band	

    // Also used as threshold for frequency estimation
    FLOAT32 Period;							//Period Calculated
	FLOAT32 PeriodMem[2];					//Memory from last two perids calculated
	UINT8	MemIndex;						//Index for memory period
	INT32 	InputFreqInit;					//Counter to block frequency for input

    UINT8 FreezeFreqUpdate;      // Counter to freeze frequency estimation update in phasors
    UINT8 DerivedFreVarsIndex1;  // Index for DerivedFreVars
    UINT8 DerivedFreVarsIndex2;  // Index for DerivedFreVars. If ACQuantity config is a PHASE_EARTH it can be configured
                                 // in two DerivedFreVars. Example: ACQuantity is PhaseA -> PhaseAB and PhaseCA
    FLOAT32 ValidPeriod;         // Last valid period calculate in the ISR
    UINT8 InvValidPeriod;        // Counter to inv ValidPeriod
    UINT8 FreqBckUpIdx;          // ACQuantity Idx for frequency backup
    FLOAT32 FreqCompAlgo;        // Frequency used in Frequency compensation algorithm

    PHASOR* pGroupPhase;
	PHASOR* pCoordGroupPhase;
	PHASOR* pGroupPhase2h;
//	PHASOR* pGroupPhase5h;
	PHASOR* pCoordGroupPhase2h;
	PHASOR* pCoordGroupPhase5h;
	PHASOR* pSecondEstimation;
	UINT8 FDFrequencySecurity;				//Algorithm used in fault detector
} ACQUANTITYCONFIG;

typedef struct{
	UINT8	Sel1; // Selector1 value
	UINT8	Sel2; // Selector2 value
}SWITCH_SUM_VALUE; // Used to save Switch Sum devived group configuration and de value of the selectors.


typedef struct tagDerivedSamplesCfg{
	UINT8	Type;//This type will inform the ISR of the operations that must be done.If it's value is zero the calculation is OFF
	UINT8	FirstParcel;
	UINT8	SecondParcel;
	UINT8	ThirdParcel;
	SWITCH_SUM_VALUE*	SwitchSumValue;
}DerivedSamplesCfg;

//This will hold the pointer to the configuration of the Derived samples.
typedef struct tagGroupDerivedSamplesCfg {
	DerivedSamplesCfg* PhaseA;
	DerivedSamplesCfg* PhaseB;
	DerivedSamplesCfg* PhaseC;
	DerivedSamplesCfg* PhaseRes;
	DerivedSamplesCfg* PhaseAB;
 	DerivedSamplesCfg* PhaseBC;
	DerivedSamplesCfg* PhaseCA;
} GroupDerivedSamplesCfg;

typedef struct tagCoordDerivedSamplesCfg{
	UINT8	Type;//This type will inform the CoordinateAC of the operations that must be done.If it's value is zero the calculation is OFF
	UINT8	ACLocalFilterType;	//if 0 is remote; If >0 is Local and -> (1 - Uses Low Pass (Voltages) 2 - Uses High Pass (Currents))
	UINT8	FirstParcel;
	UINT8	SecondParcel;
	SWITCH_SUM_VALUE*	SwitchSumValue;
}CoordDerivedSamplesCfg;

enum {
	LOCAL_ACQUISITION,
	PROCESS_BUS_ACQUISITION
};

typedef enum{
	ORIG_LOCAL = 0, //McASP
	ORIG_PROCESS_BUS, //Process Bus
	NUM_ORIG_AC_INPUTS
}ChannelOrigin;

typedef enum{
	LOW_SCALE,
	HIGH_SCALE,
	ALL_SCALES
}HWScale;

typedef struct tagGROUP {
	PHASOR		A;					  	// Phase A phasor
	PHASOR		B;					  	// Phase B phasor
	PHASOR		C;					  	// Phase C phasor
	PHASOR		RES;				  	// Residual phasor
//	PHASOR		H;					  	// Zero sequence phasor: not used (equal to Residual / 3)
	PHASOR		D;					  	// Positive sequence phasor
	PHASOR		I;					  	// Negative sequence phasor
	PHASOR		AB;					  	// Phase-phase AB phasor
	PHASOR		BC;					  	// Phase-phase BC phasor
	PHASOR		CA;					  	// Phase-phase CA phasor

} GROUP;

typedef struct tagGROUPCONFIG {
	UINT8	GroupID;
	FLOAT32	Ratio;
	FLOAT32	RatedValue;
	FLOAT32	PrimaryConversion;
	UINT8	ACInputIndex_A_AB;
	UINT8	ACInputIndex_B_BC;
	UINT8	ACInputIndex_C_CA;
	UINT8	ACInputIndex_RES;
	//PHASOR	LastPhasorF;
	//PHASOR*	PhasorF;
	UINT8	GroupType;
	UINT8	InputType;
	UINT8	InputInfo;			//	Additional information regarding the AC inputs (sensitive, resistive, ...)
	//void 	*SampleBuffer;
	void	*InternalGroup1;
	void	*InternalGroup2;
	void	*Selector;
	UINT8 	SwitchSumIdx;
	UINT8	NumberofInputs;
	UINT8	Orientation;
	UINT8	VectorGroup;
	FLOAT32 MagnRatio;
	FLOAT32 MagnCorr;
	FLOAT32 PhaseCorr;
	UINT8 	Calculate;	//Inform the Estimation how many derived values are calculated 1 to 4
	UINT8	IsCoordGroup;
	UINT32 	CoordGroupIndex;
	UINT8	IsSecEstimationGroup;
	UINT8	SecondEstimationGroupIndex;
	INT8	ChannelIndex;
//	void 	*_pGroupFunction(void *);
} GROUPCONFIG;


typedef struct tagINTERNALGROUP {
	GROUPCONFIG Config;					// Configuration parameters of the group
	GROUP Estimation;					// Group quantities calculated by estimation
	//GROUP Task[GROUP_BUFFER_SIZE];		// Group quantities available to each task
} INTERNALGROUP;

typedef struct tagESTIMATIONVARS {
	INT32 EstimationStatus;
	INT32 FaultDetectorStatus[GROUP_BUFFER_SIZE];									// Fault detected / Number of samples since fault detector active (only for the first cycle)
	INT32 FaultDetectorHistoricSample;							// Sample position where fault was detected (in Historic)
	FLOAT32 Error[MAX_NUM_CH+1][GROUP_BUFFER_SIZE];
    UINT16 InpQual[MAX_AC_LOCAL_QUANTITIES];							//Quality AC inputs - from last phasor calculus
	UINT16 TaskInpQual[GROUP_BUFFER_SIZE][MAX_AC_LOCAL_QUANTITIES];	//Quality AC inputs - saved for tasks
	struct {
		FLOAT32 Estimation;	//Flag to activate the alsolute polarity estimation (0- not active; FLOAT32- magnitude expected for polarity)
		INT8 Result;		//Absolute polarity result (0-not calculated; 1-positive; -1-negative)
		UINT32 Index; 	//Index of the input to estimate
		FLOAT32 Offset;
		INT32 Ready;
	}Polarity;
}ESTIMATIONVARS;

UINT16 Invalid(UINT16 Qual){return ((Qual & (UINT16)VALIDITY_MASK)==(UINT16)INVALID_QUAL);}

void ForceValid(UINT16* pQual){*pQual = (UINT16)VALID_QUAL;}

UINT16 Merge4Qual(UINT16 Qual1, UINT16 Qual2, UINT16 Qual3, UINT16 Qual4){
	UINT16 aux = Qual1 | Qual2 | Qual3 | Qual4;
	if( ((Qual1 & VALIDITY_MASK) == 1) | ((Qual2 & VALIDITY_MASK) == 1) | \
		((Qual3 & VALIDITY_MASK) == 1) | ((Qual4 & VALIDITY_MASK) == 1) ){
		aux &= (~SyBIT1);
	}
	return aux;
}

UINT16 Merge2QualVal2_MergedInv(UINT16 Qual1, UINT16 Qual2, UINT16 Val2, UINT16* InvMerged){
	UINT16 Inv1 = Invalid(Qual1);
	UINT16 GenVal = !Inv1 && Val2;
	*InvMerged = !GenVal;
	return (((Inv1|GenVal) * Qual1) | (((!Val2)|GenVal) * Qual2) );
}

UINT16 Merge2QualVal2(UINT16 Qual1, UINT16 Qual2, UINT16 Val2){
	UINT16 Inv1 = Invalid(Qual1);
	UINT16 GenVal = !Inv1 && Val2;
	return (((Inv1|GenVal) * Qual1) | (((!Val2)|GenVal) * Qual2) );
}

UINT16 Merge2Qual(UINT16 Qual1, UINT16 Qual2){
	UINT16 aux = Qual1 | Qual2;
	if( ((Qual1 & VALIDITY_MASK) == 1) | ((Qual2 & VALIDITY_MASK) == 1) ){
		aux &= (~SyBIT1);
	}
	return aux;
}

UINT16 Merge3Qual(UINT16 Qual1, UINT16 Qual2, UINT16 Qual3){
	UINT16 aux = Qual1 | Qual2 | Qual3;
	if( ((Qual1 & VALIDITY_MASK) == 1) | ((Qual2 & VALIDITY_MASK) == 1) | ((Qual3 & VALIDITY_MASK) == 1) ){
		aux &= (~SyBIT1);
	}
	return aux;
}

void FilterMgnComp(UINT8 Type, FLOAT32 NewPeriod , FLOAT32 OldPeriod, FLOAT32* PrimaryConversionComp);

FLOAT32 Filter(FLOAT32 * A, FLOAT32 * B){
	return (A[0] * B[0]) + (A[1] * B[1]);
}

UINT16 Merge5Qual(const UINT16* pQual){
	UINT16 aux = pQual[0] | pQual[1] | pQual[2] | pQual[3] | pQual[4];
	if( ((pQual[0] & VALIDITY_MASK) == 1) | ((pQual[1] & VALIDITY_MASK) == 1) | \
	    ((pQual[2] & VALIDITY_MASK) == 1) | ((pQual[3] & VALIDITY_MASK) == 1) | \
		((pQual[4] & VALIDITY_MASK) == 1) ){
		aux &= (~SyBIT1);
	}
	return aux;
}

PHASOR Dft20(FLOAT32 *pSamples);

PHASOR FastDft(FLOAT32 *pSamples,FLOAT32 *pCos_Sen, FLOAT32 *pCoefOrth, UINT8 Samples);

FLOAT32 Magnitude(PHASOR* phasor){
    return sqrt(phasor->Re * phasor->Re + phasor->Im * phasor->Im);
}


void RootMeanSquare(int i);

bool ProcessBusISR(RCV_DATA_PR_BUS* Rcv);

void DerivedSampleCalculus(INT32 SamplePos, INT32 NextRawSampleIndex, INT32 RawSampleIndexNext, INT32 NextRMSRawIndex);

void Init();

void CalculateAC();

int start_calculation(int initial , int repeat);












#endif

