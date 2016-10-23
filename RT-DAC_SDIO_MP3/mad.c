#include "mad.h"
#include "oversample.h"

//#define dbgprintf(a,...) chprintf((BaseSequentialStream*)&SDU1, a, __VA_ARGS__ )
#define dbgprintf(a,...)

thread_t *MadThd;

void *mp3_malloc(int s) {
  void* p = chHeapAlloc(NULL, s);
  dbgprintf("malloc %d -> %x\r\n", s, p);
  return p;
}
void mp3_free(void *p) {
  dbgprintf("free %p\r\n", p);
  chHeapFree(p);
}
void *mp3_calloc(int num,int size)
{
 unsigned char *ptr;
 int i;
 ptr=mp3_malloc(size*num);
 dbgprintf("calloc %d x %d -> %x\r\n", num, size, ptr);
 if (!ptr) return NULL;
 //for(i=0;i<num*size;i++) ptr[i]=0;
 memset(ptr, 0, size*num);
 return ptr;
}

struct buffer {
  unsigned char *start;
  int eof;
  FIL *file;
  int samplerate;
};

#define wave_buf_size 1152*2*OVERSAMPLE_FACTOR //1152 double samples

uint16_t wave_buf[2*wave_buf_size]; //two banks for the dma
uint16_t playing;

#define MP3BUF 4096

unsigned char mp3buf[MP3BUF];
FIL fIn;
struct buffer buffer;
struct mad_decoder decoder;


/////////DAC SHIT BLOCK

static void end_cb1(DACDriver *dacp, const dacsample_t *dacbuffer, size_t n) {
  (void)dacp;
  (void)n;
  chSysLockFromISR();
  if(dacbuffer == (dacsample_t*)wave_buf) {
    chEvtSignalI(MadThd, (eventmask_t)1);
  } else {
    chEvtSignalI(MadThd, (eventmask_t)2);
  }
  chSysUnlockFromISR();
}

/*
 * DAC error callback.
 */
static void error_cb1(DACDriver *dacp, dacerror_t err) {

  (void)dacp;
  (void)err;
  chSysHalt("DAC failure");
}

static const DACConfig dac1cfg1 = {
	  .init         = 2047U << 4,
	  .datamode     = DAC_DHRM_12BIT_RIGHT_DUAL
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

static THD_WORKING_AREA(waDACPlayer, 128);
static THD_FUNCTION(DACPlayer, arg) {
  dacStart(&DACD1, &dac1cfg1);
  gptStart(&GPTD6, &gpt6cfg1);

  dacStartConversion(&DACD1, &dacgrpcfg1, wave_buf, wave_buf_size);
  gptStartContinuous(&GPTD6, 42000000U / (((int)arg)*2));
}

/////////


enum mad_flow input(void *data,
		    struct mad_stream *stream)
{
  struct buffer *buffer = data;
  UINT unRead = 0, MaxRead;
  UINT RemainingBytes = stream->bufend - stream->next_frame;


  if (buffer->eof)
    return MAD_FLOW_STOP;
 

  if ((RemainingBytes)&&(RemainingBytes<MP3BUF))
    memmove(buffer->start, stream->next_frame, RemainingBytes);

  MaxRead = (MP3BUF - RemainingBytes)&0xFE00;

  FRESULT fr = f_read(buffer->file, buffer->start + RemainingBytes, MaxRead, &unRead);
  if (unRead!=MaxRead) buffer->eof = 1;

  dbgprintf("Read : %d (rem=%d - MaxRead=%d)\n\r",unRead,RemainingBytes,MaxRead);

  mad_stream_buffer(stream, buffer->start, RemainingBytes+unRead);


  return MAD_FLOW_CONTINUE;
}

enum mad_flow header(void *data,
		    struct mad_header const *header)
{
  struct buffer *buffer = data;

 if (buffer->samplerate!=header->samplerate)
 {
  dbgprintf("MP3 Header Info :\n\r",NULL);
  dbgprintf(" Bitrate : %d\n\r",header->bitrate);
  dbgprintf(" Samplerate : %d\n\r",header->samplerate);

  //Codec_AudioInterface_Init(header->samplerate);
  //insert own shit here maybe
  chprintf((BaseSequentialStream*)&SDU1, "MP3 Header Info :\n\r",NULL);
  chprintf((BaseSequentialStream*)&SDU1, " Bitrate : %d\n\r",header->bitrate);
  chprintf((BaseSequentialStream*)&SDU1, " Samplerate : %d\n\r",header->samplerate);

  buffer->samplerate=header->samplerate;
 }

 return MAD_FLOW_CONTINUE;
}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */
 
//insert sg magic here

signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));
#if 0
  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
#else
  { signed int result;
    asm("ssat %0, %2, %1, asr %3" \
        : "=r" (result) \
        : "%r" (sample), \
          "M" (16), "M" (MAD_F_FRACBITS + 1 - 16) );
    return result + 0x8000; }
#endif
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */

#define OFFSET 0

oversample_ctx over_ctx_l;
oversample_ctx over_ctx_r;

enum mad_flow output(void *data,
		     struct mad_header const *header,
		     struct mad_pcm *pcm)
{
  unsigned int nchannels, nsamples;
  mad_fixed_t const *left_ch, *right_ch;
  signed sample;
  uint16_t *write_buf;
  struct buffer *buffer = data;
  int i;

  /* pcm->samplerate contains the sampling frequency */

  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];

  dbgprintf("output: %d channels, %d samples, left_ch=%x, right_ch=%x\r\n", nchannels, nsamples, left_ch, right_ch);

  if (playing==0){
    chprintf((BaseSequentialStream*)&SDU1, "starting DAC @%d hz\r\n", buffer->samplerate);
    chThdCreateStatic(waDACPlayer, sizeof(waDACPlayer), NORMALPRIO, DACPlayer, buffer->samplerate);
    playing=1;
    dbgprintf("DAC output running.\r\n", NULL);
  }

  eventmask_t event = chEvtWaitAny((eventmask_t)(1|2|4));

  if (event & (eventmask_t)(1|2)) {
    write_buf=wave_buf+((event & (eventmask_t)2) ? wave_buf_size : 0);
    dbgprintf("writing interleaved samples to %x\r\n", write_buf);
    /*
    while (nsamples--) {
      sample = scale(*left_ch++);
      *write_buf++=sample;
      if (nchannels == 2) sample = scale(*right_ch++);
      *write_buf++=sample;
    }
    */
    oversample_run(&over_ctx_l,nsamples,left_ch,1,write_buf,2);
    if(nchannels == 2) oversample_run(&over_ctx_r,nsamples,right_ch,1,write_buf+1,2);
    return MAD_FLOW_CONTINUE;
  } else if(event & (eventmask_t)4) {
    dbgprintf("stopping playback\r\n", NULL);
    return MAD_FLOW_STOP;
  }
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
 * header file.
 */

enum mad_flow error(void *data,
		    struct mad_stream *stream,
		    struct mad_frame *frame)
{
  struct buffer *buffer = data;

  dbgprintf("decoding error 0x%04x (%s) at byte offset %u\r\n",
	  stream->error, mad_stream_errorstr(stream),
	  stream->this_frame - buffer->start);

  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
}

/*
 * This is the function called by main() above to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */

static THD_WORKING_AREA(waThdMad, 8192);
static THD_FUNCTION(ThdMad, arg) //arg -> filename
{
  (void)arg;
  fs_ready=TRUE;
  int result;
  MadThd = chThdGetSelfX();

  oversample_reset(&over_ctx_l, (float)(1.0/(1<<17)));
  oversample_reset(&over_ctx_r, (float)(1.0/(1<<17)));

  FRESULT errFS = f_open(&fIn, (char*)arg, FA_READ);
  if(errFS != FR_OK) {
    dbgprintf( "Mp3Decode: Failed to open file \"%s\" for reading, err=%d\r\n", (char*)arg, errFS);
    return -1;
  }
  fs_ready=TRUE;

  /* initialize our private message structure */
  buffer.start  = mp3buf;
  buffer.file   = &fIn;
  buffer.eof    = 0;
  buffer.samplerate = 10000; //to make sure we can hear the error ghosts whispering

  /* configure input, output, and error functions */
  mad_decoder_init(&decoder, &buffer,
		   input, header, 0 /* filter */, output,
		   error, 0 /* message */);

  playing=0;

  /* start decoding */
  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

  dbgprintf("Decoding return %d\r\n",result);

  /* release the decoder */
  mad_decoder_finish(&decoder);
  f_close(&fIn);

  gptStopTimer(&GPTD6);
  gptStop(&GPTD6);
  dacStopConversion(&DACD1);
  dacStop(&DACD1);

  return result;
}
