
#ifndef _MEASUREMENT_H_
#define _MEASUREMENT_H_

#define TXRX_SWITCH   4
#define S_METER       32            // GPIO port S-Meter
#define FWD_METER     33            // GPIO port Fwd-Meter
#define REV_METER     39             // GPIO port Rev-Meter
#define NUMBR_SSID    5                // Store max 5 ssid's and passwords

#define TWENTYTOONE               0 // 1 to select the values for 20 to 1 coupler, else 0
#define SAMPLE_TIMER           1000 // Interrupt timer, in microseconds
#define BUF_SHORT               100 // Buffer size for 100ms Peak
#define PEP_BUFFER               50 // PEP Buffer size in multiples of BUF_SHORT, can hold up to 5 second PEP
#define LONG_BUFFER              30 // LONG Buffer size in multiples of shortest PEP period (1s) - typically 30 seconds
#define AVG_BUF1S              1000 // AVG Buffer sample size (1000 is 1 second, if SAMPLE_TIMER = 1000)
#define AVG_BUFSHORT            100 // Short (100ms) AVG Buffer size
#define AVG_BUFSWR               10 // Very short buffer for SWR bargraph smoothing.  Time is POLL_TIMER*AVG_BUFSWR
#define MODSCOPE_DIVISOR          1 // Modultion Scope scan rate divisor
#define AD8307_INSTALLED		  1
#define CAL1_NOR_VALUE          400 // 40 dBm, default dBm level1 for both AD8307
#define CAL2_NOR_VALUE          100 // 10 dBm, default dBm level2 for both AD8307
#define CALFWD1_DEFAULT       2.233 // Default raw Voltage level1 at  40 dBm
#define CALREV1_DEFAULT       2.233 // Default raw Voltage level1 at  40 dBm
#define CALFWD2_DEFAULT       1.528 // Default raw Voltage level2 at  10 dBm
#define CALREV2_DEFAULT       1.528 // Default raw Voltage level2 at  10 dBm
#define PEP_PERIOD (2500000/SAMPLE_TIMER)/BUF_SHORT // 2.5 seconds = Default

#if TWENTYTOONE                     // Defs when using a 20 to 1 coupler and AD7991
#define MIN_PWR_FOR_SWR_CALC   0.5  // Minimum Power in mW for SWR calculation and display
#else                               // Defs when using a 30 to 1 coupler and AD7991
#define MIN_PWR_FOR_SWR_CALC   1.0  // Minimum Power in mW for SWR calculation and display
#endif
#define MIN_PWR_FOR_SWR_SHOW  0.01  // Minimum Power in mW for SWR indication (use recent value)

//-----------------------------------------------------------------------------
// DEFS for FWD and REF coupler, 2xAD8307 or Diode detectors
//-----------------------------------------------------------------------------
// DEFS for AD8307 Calibration (dBm *10)
// levels for the forward and reverse measurements - in order to allow a Calibration
//
// Note that the AD8307 generates 2uA per dB and applies an internal resistor of 12.5kohms
// 2uA * 12.5kohm gives the specified slope of 25mV/dB.  
// Unloaded Teensy 3.2 AD inputs appear to have an impedance of approx 200 kohms,
// hence the voltage slope is slightly affected:
// 12.5k||200k = 11.75k.  2uA * 11.75k gives a slope value of 23.5mV/dB.
// However if the meter has a voltage divider made out of RS=15kohm and RP=10kohm, 
// then the slope would need to be recalculated like below:
// 12.5k||25k = 8.333k.  2uA * 8.333k then gives a slope value of 16.67mV/dB 
// or if RS = 47kohm and RP = 33kohm (RS+RP = 80 kohm), then:
// 12.5k||80k = 10.81k.  2uA * 10.81k gives a slope value of 21.62mV/dB
//
#define LOGAMP1_SLOPE          23.5 // mV/dB.  Used with Single Level Calibration
#define LOGAMP2_SLOPE          23.5 // mV/dB.  Used with Single Level Calibration
                                    // Typical value for AD8307 is 25mV/dB if AD input is ~1Mohm or higher
                                    // Typical value for ADL5519 is 22mV/dB
//-----------------------------------------------------------------------------
#define CAL_INP_QUALITY        12.0 // (dB) Minimum difference between the Fwd and Rev
                                    // signals to be allowed to calibrate
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADC Reference Voltage and Voltage Drop Definitions
#define ADCREF_BUILTIN            0 // 1 for builtin 1.2V reference, 0 for external 3.3V reference
//-----------------------------------------------------------------------------
// Automatic, no need to do anything with these (unless external voltage regulator is not exactly 3.3V)
#if ADCREF_BUILTIN
#define ADC_REF                1.20 // Internal Reference voltage for ADC
#else
#define ADC_REF                3.30 // External Reference voltage for ADC
#endif
//-----------------------------------------------------------------------------
// Resistors for Voltage Drop before AD input - used when internal 1.2V reference
// and/or when Diode Detector type coupler is used instead of a 2x AD8307 coupler.
#define RS                      0.0 // Series Resistor between measured voltage and ADC_IN (k ohm)
#define RP                     10.0 // Parallel Resistor, ADC_IN to ground for ADC_IN voltage drop (k ohm)
                                    // If no RS and RP, then set RS as 0 and RP as any non-zero value                                   
//-----------------------------------------------------------------------------
 // Equivalent Reference Voltage, derived from ADC_REF & voltage drop resistors
#define ADC_RES  ADC_REF*(RS+RP)/RP // Don't touch, automaticcally derived from the above


#ifndef SQR
#define SQR(x) ((x)*(x))
#endif
#ifndef ABS
#define ABS(x) ((x>0)?(x):(-x))
#endif

//-----------------------------------------------------------------------------
// Structures and Unions


typedef struct {
          int16_t  fwd[256];                  // Circular buffer of Forward measurement values
          int16_t  rev[256];                  // Circular buffer of Reverse measurement values
          uint8_t  incount;                   // Pointer to most recent input value of circular buffer
          uint8_t  outcount;                  // Pointer to most recent output value of circular buffer
               }  adbuffer_t;
               
typedef struct {
          int16_t  db10m;                     // Calibrate, value in dBm x 10
          double   Fwd;                       // corresponding AD8307 forward output value, V * 10000
          double   Rev;                       // corresponding AD8307 reverse output value, V * 10000
               }  cal_t;

typedef struct {
                cal_t    cal_AD[2];                 // 2 Calibration points for both AD8307, if AD8307 option
                uint16_t PEP_period;                // PEP envelope sampling time in SAMPLE_TIME increments
                uint16_t AVG_period;                // AVG sampling time in SAMPLE_TIME increments
                unsigned low_power_floor     : 3;   // Lowest power level shown on display
                     #define  FLOOR_NOISEFLOOR 0    // No low power level threshold, lowest power shown is the effective noise floor
                     #define  FLOOR_ONE_uW     1    // Any power levels below 1uW are displayed as "0 uW"
                     #define  FLOOR_TEN_uW     2    // Any power levels below 10uW are displayed as "0.00mW"
                     #define  FLOOR_100_uW     3    // Any power levels below 100uW are displayed as "0.0mW"
                     #define  FLOOR_ONE_mW     4    // Any power levels below 1mW are displayed as "0 mW"
                     #define  FLOOR_TEN_mW     5    // Any power levels below 10mW are displayed as "0.00W"
               int32_t       correction_si5351_no1;
               int32_t       correction_si5351_no2;
               int32_t       xtal_si5351_no1;
               int32_t       xtal_si5351_no2;
               char         ssid[5][45];
               char         password[5][64];
               long         current_frq1[10];
			   long         current_frq2[10];
               uint8_t      band[2];
               uint8_t      active_vfo;
               uint8_t      wifi_onoff;
               } var_t;

extern double power_mw;
extern double power_mw_pep;
extern double power_db;       // Calculated power in dBm
extern double power_db_pk;    // Calculated 100ms peak power in dBm
extern double power_db_pep;   // Calculated PEP power in dBm
extern double power_db_long;  // Calculated MAX power in dBm, 30 sec or longer window
extern double rev_power_db;
extern char   lcd_buf[];
extern var_t  R;
extern double adc_ref;        // ADC reference 
extern int16_t     fwd;            // AD input - 12 bit value, v-forward
extern int16_t     rev;            // AD input - 12 bit value, v-reverse

extern  SemaphoreHandle_t swrBinarySemaphore;
extern  QueueHandle_t     rotary_queue;

extern void adc_init(void);
extern void adc_poll_and_feed_circular(void* pvParameters);
extern void pswr_sync_from_interrupt(void);
extern void pswr_determine_dBm(void);
extern void determine_power_pep_pk(void);
extern uint16_t print_p_mw(double pwr);
extern uint16_t print_swr(void);
extern void calc_SWR_and_power(void);
extern void start_measurement();
#endif
