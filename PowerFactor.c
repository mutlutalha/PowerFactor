#include "main.h"
#include "math.h"
#include "arm_math.h"

#define SAMPLES 1000
#define VOLTAGE_TRANSFORMER_RATIO 1 // buradaki katsayı gerilim trafosunun dönüşüm oranıdır.
#define CURRENT_TRANSFORMER_RATIO 1 // buradaki katsayı akım trafosunun dönüşüm oranıdır.
#define ADC_RESOLUTION 4096.0
#define VREF 3.3

float32_t voltage_samples[SAMPLES];
float32_t current_samples[SAMPLES];

void read_samples() {
  for (int i = 0; i < SAMPLES; i++) {
    HAL_ADC_Start(&hadc1); // ADC1 için gerilim örnekleme
    HAL_ADC_PollForConversion(&hadc1, 10);
    voltage_samples[i] = (HAL_ADC_GetValue(&hadc1) / ADC_RESOLUTION) * VREF * VOLTAGE_TRANSFORMER_RATIO;

    HAL_ADC_Start(&hadc2); // ADC2 için akım örnekleme
    HAL_ADC_PollForConversion(&hadc2, 10);
    current_samples[i] = (HAL_ADC_GetValue(&hadc2) / ADC_RESOLUTION) * VREF * CURRENT_TRANSFORMER_RATIO;

    HAL_Delay(1);
  }
}

float32_t calculate_true_rms(float32_t *samples) {
  float32_t sum_of_squares = 0;

  for (int i = 0; i < SAMPLES; i++) {
    sum_of_squares += samples[i] * samples[i];
  }

  return sqrt(sum_of_squares / SAMPLES);
}

float32_t calculate_power_factor(float32_t voltage_rms, float32_t current_rms) {
  float32_t active_power = 0;

  for (int i = 0; i < SAMPLES; i++) {
    active_power += voltage_samples[i] * current_samples[i];
  }

  active_power /= SAMPLES;

  return active_power / (voltage_rms * current_rms);
}

void filter_samples(float32_t *samples) {
  arm_biquad_cascade_df2T_instance_f32 filter_instance;
  float32_t filter_coefficients[] = { /* Düşük geçiren filtre katsayıları */ };
  float32_t filter_state[4];
  arm_biquad_cascade_df2T_init_f32(&filter_instance, 1, filter_coefficients, filter_state);
  arm_biquad_cascade_df2T_f32(&filter_instance, samples, samples, SAMPLES);
}

int main() {
  // ... Diğer başlatma ve yapılandırma kodları

  while (1) {
    read_samples();

    filter_samples(voltage_samples);
    filter_samples(current_samples);

    float32_t voltage_rms = calculate_true_rms(voltage_samples);
    float32_t current_rms = calculate_true_rms(current_samples);
    float32_t power_factor = calculate_power_factor(voltage_rms, current_rms);

    printf("Güç Faktörü: %f\n", power_factor);

    HAL_Delay(1000);
  }
}

