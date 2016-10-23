#ifndef INC_GUARD_FAXNXH9GH0HXYDQG69HOW67RRVQRWA
#define INC_GUARD_FAXNXH9GH0HXYDQG69HOW67RRVQRWA

#include <stdint.h>
#include <stddef.h>

#define OVERSAMPLE_FACTOR 2
#define OVERSAMPLE_FIFO_SIZE 8
#define OVERSAMPLE_FIFO_MASK 7
#define OVERSAMPLE_NS_ORD 4

typedef struct {
    float gain;
    int32_t fifo_data[OVERSAMPLE_FIFO_SIZE];
    int fifo_pos;
    float last_err;
    float t[OVERSAMPLE_NS_ORD];
} oversample_ctx;

extern void oversample_reset(oversample_ctx* ctx, float gain);

extern void oversample_set_gain(oversample_ctx* ctx, float gain);

extern float oversample_get_gain(oversample_ctx* ctx);

extern void oversample_run(oversample_ctx* ctx,
                           unsigned int count,
                           const int32_t in[], ptrdiff_t in_step,
                           int16_t out[], ptrdiff_t out_step);

#endif /* include guard */

