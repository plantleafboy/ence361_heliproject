#define SAMPLE_RATE_HZ 80

void init_altitude (void);

void init_ADC (void);

void ADC_int_handler(void);

int altitude_percentage(int mean_val);

int calc_meanVal(void);

int16_t target_altitude (void);

void setoffset (int value);
