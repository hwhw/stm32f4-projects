#include <stdlib.h>
#include "oversample.h"

#define OVERSAMPLE_IR_LEN 8
#define OVERSAMPLE_IR_MAXIDX (OVERSAMPLE_IR_LEN-1)

/* quater sample interpolation filter for 2X oversampling
 * (given samples at x, interpolate at o)
 *
 *   ---x---x---x---xo--x---x---x---x---
 */

static const float ir[OVERSAMPLE_IR_LEN] = {
  -1.12746117107814e-02,
   3.83025405152053e-02,
  -1.16624324792097e-01,
   5.21666904043242e-01,
   6.35085871896743e-02,
   8.82448701227850e-03,
  -9.21510527815687e-03,
  -9.39121099423289e-04
};

/*               b0 + b2 z^-2
 * H(z) =  -------------------------
 *         1 + a1 z^-1 + ... a4 z^-4
 *
 *
 * NTF(z) = 1 - z^-1 H(z)  (noise transfer function)
 */

#if 1

#define NS_A1 1.441343797520019f
#define NS_A2 1.480913492184099f
#define NS_A3 0.750307027424829f
#define NS_A4 0.180625000000000f

#define NS_B0 2.882687595040038f
#define NS_B2 1.500614054849658f

#else

#define NS_A1 0.0f
#define NS_A2 0.0f
#define NS_A3 0.0f
#define NS_A4 0.0f

#define NS_B0 0.0f
#define NS_B2 0.0f

#endif

void oversample_reset(oversample_ctx* ctx, float gain)
{
    ctx->gain = gain;
    for (int i = 0; i < OVERSAMPLE_FIFO_SIZE; ++i) {
        ctx->fifo_data[i] = 0;
    }
    ctx->fifo_pos = 0;
    ctx->last_err = 1.0f;
    for (int i = 0; i < OVERSAMPLE_NS_ORD; ++i) {
        ctx->t[i] = 0.0f;
    }
}

void oversample_set_gain(oversample_ctx* ctx, float gain)
{
    ctx->gain = gain;
}

float oversample_get_gain(oversample_ctx* ctx)
{
    return ctx->gain;
}

static int16_t quantize(float x)
{
    if (x >=  2047) return  2047;
    if (x <= -2048) return -2048;
    return (int16_t)(x + (x >= 0 ? 0.5f : -0.5f));
}

void oversample_run(oversample_ctx* ctx,
                    unsigned int count,
                    const int32_t in[], ptrdiff_t in_step,
                    int16_t out[], ptrdiff_t out_step)
{
    int32_t *dat = ctx->fifo_data;
    int pos = ctx->fifo_pos;
    float gain = ctx->gain;
    ptrdiff_t out_step2 = out_step * 2;
    float tmp[OVERSAMPLE_FACTOR];
    float last_err = ctx->last_err;
    float *t = ctx->t;
    while (count-- > 0) {
        // upsampling...
        pos = (pos + 1) & OVERSAMPLE_FIFO_MASK;
        dat[(pos + OVERSAMPLE_IR_MAXIDX) & OVERSAMPLE_FIFO_MASK] = *in;
        in += in_step;
        float sum0 = 0.0f;
        float sum1 = 0.0f;
        for (int i = 0; i < OVERSAMPLE_IR_LEN; ++i) {
            float c = ir[i];
            sum0 += dat[(pos                        + i) & OVERSAMPLE_FIFO_MASK] * c;
            sum1 += dat[(pos + OVERSAMPLE_IR_MAXIDX - i) & OVERSAMPLE_FIFO_MASK] * c;
        }
        tmp[0] = sum0 * gain;
        tmp[1] = sum1 * gain;

        // noise shaping...
        for (int i = 0; i < OVERSAMPLE_FACTOR; ++i) {
            // apply filter H on last_err...
            float tm1 = last_err - NS_A1 * t[0]
                                 - NS_A2 * t[1]
                                 - NS_A3 * t[2]
                                 - NS_A4 * t[3];
            float feedback = NS_B0 * tm1
                           + NS_B2 * t[1];
            for (int i = OVERSAMPLE_NS_ORD - 1; i > 0; --i) {
                t[i] = t[i - 1];
            }
            t[0] = tm1;

            // quantize...
            float wanted = tmp[i] - feedback;
            float dither = ((rand() & 15) + 0.5f) * (1.f/16) - 0.5f;
            int16_t q = quantize(wanted + dither);
            last_err = q - wanted;
            if (last_err < -2.f) last_err = -2.f; else
            if (last_err >  2.f) last_err =  2.f;
            *out = q+2048;
            out += out_step;
        }
    }
    ctx->fifo_pos = pos;
    ctx->last_err = last_err;
}

