#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "ff.h"

/*===========================================================================*/
/* FatFs related.                                                            */
/*===========================================================================*/

/**
 * @brief FS object.
 */
static FATFS SDC_FS;
static FIL fsrc;

/* FS mounted and ready.*/
static bool fs_ready = FALSE;
static bool have_err = FALSE;

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palTogglePad(GPIOB, GPIOB_LED);
    chThdSleepMilliseconds(have_err ? 125 : (fs_ready ? 1000 : 3000));
  }
}



#define DAC_BUFFER_SIZE 1024

static dacsample_t dac_buffer[DAC_BUFFER_SIZE];

//static thread_t *tp_shuffler;
static thread_reference_t trp = NULL;
static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Thread2, arg) {
  (void)arg;
	FRESULT err;
  chRegSetThreadName("shuffler");
  while (true) {
    chSysLock();
    uint8_t *dst = (uint8_t*) chThdSuspendS(&trp);
    chSysUnlock();

    unsigned int br;
    unsigned int s = DAC_BUFFER_SIZE;
    err = f_read(&fsrc, dst, s, &br);
    if(err == FR_OK) {
      if(br < s) {
        memset(dst + br, 80U, s-br);
        err = f_lseek(&fsrc, 0);
      }
    }
  }
}

static void end_cb1(DACDriver *dacp, const dacsample_t *buffer, size_t n) {
  (void)dacp;
  (void)n;
  uint8_t *dst;
  if(buffer == dac_buffer) {
    dst = (uint8_t*)dac_buffer;
  } else {
    dst = (uint8_t*)&dac_buffer[DAC_BUFFER_SIZE/2];
  }
  chSysLockFromISR();
  chThdResumeI(&trp, (msg_t)dst);
  chSysUnlockFromISR();
}

/*
 * DAC error callback.
 */
static void error_cb1(DACDriver *dacp, dacerror_t err) {

  (void)dacp;
  (void)err;
  have_err = true;
  chSysHalt("DAC failure");
}

static const DACConfig dac1cfg1 = {
	  .init         = 2047U << 4,
	  .datamode     = DAC_DHRM_12BIT_LEFT_DUAL
};

static const DACConversionGroup dacgrpcfg1 = {
	.num_channels = 2U,
  .end_cb       = end_cb1,
	.error_cb     = error_cb1,
	.trigger      = DAC_TRG(0)
};

/*
 *  * GPT6 configuration.
 *   */
static const GPTConfig gpt6cfg1 = {
	  .frequency    = 42000000U,
	  .callback     = NULL,
	  .cr2          = TIM_CR2_MMS_1,    /* MMS = 010 = TRGO on Update Event.    */
	  .dier         = 0U
};


/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Set up DAC output pins
   */
  palSetGroupMode(GPIOA, PAL_PORT_BIT(4) | PAL_PORT_BIT(5), 0, PAL_MODE_INPUT_ANALOG);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  dacStart(&DACD1, &dac1cfg1);
  gptStart(&GPTD6, &gpt6cfg1);

  sdcObjectInit(&SDCD1);
  sdcStart(&SDCD1, NULL);

	FRESULT err;
	sdcConnect(&SDCD1);
	err = f_mount(&SDC_FS, "/", 1);
	if (err != FR_OK) {
		goto stop;
	}
	err=f_open(&fsrc, "SINE.PCM", FA_READ);
	if (err != FR_OK) {
		goto stop;
	}
	unsigned int br;
	err=f_read(&fsrc,dac_buffer,1024*sizeof(dacsample_t), &br);
	if (err != FR_OK) {
		goto stop;
	}
	fs_ready = TRUE;
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  dacStartConversion(&DACD1, &dacgrpcfg1, dac_buffer, DAC_BUFFER_SIZE/2);
  gptStartContinuous(&GPTD6, 952U);

stop:
	while(true) {
	    chThdSleepMilliseconds(1000);
	}
}
