#ifndef MODEM_L2CACHE_TEST_CASE
#define MODEM_L2CACHE_TEST_CASE

#if 0
#define ATE_L2CACHE_TEST
#endif
#define ATE_L2_TEST_BUFFER_SIZE      (0x100000)/*1M*/
#define ATE_L2_TEST_SIZE             (0x8000000)/*128M*/

#define ATE_l2_TEST_LOW_ADDR         (0x30000000)
#define ATE_L2_TEST_HIGH_ADDR        (ATE_l2_TEST_LOW_ADDR + 127 * ATE_L2_TEST_BUFFER_SIZE)
#define ATE_L2_TEST_FLAG             (0x35000000)

#endif
