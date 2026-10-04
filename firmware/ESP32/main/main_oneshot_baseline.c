#include <stdio.h>
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define ADC_UNIT_USED ADC_UNIT_1
#define ADC_CHANNEL_USED ADC_CHANNEL_6
#define ADC_BITWIDTH_USED ADC_BITWIDTH_12
#define ADC_ATTEN_USED ADC_ATTEN_DB_12
#define ADC_DEFAULT_VREF_MV 1100
#define SAMPLES_PER_REPORT 64

static const char *TAG = "adc_test";

static void log_calibration_source(void)
{
    adc_cali_line_fitting_efuse_val_t source;

    ESP_ERROR_CHECK(
        adc_cali_scheme_line_fitting_check_efuse(&source)
    );

    switch (source) {
    case ADC_CALI_LINE_FITTING_EFUSE_VAL_EFUSE_TP:
        ESP_LOGI(TAG, "Calibration source: eFuse Two-Point");
        break;

    case ADC_CALI_LINE_FITTING_EFUSE_VAL_EFUSE_VREF:
        ESP_LOGI(TAG, "Calibration source: eFuse Vref");
        break;

    case ADC_CALI_LINE_FITTING_EFUSE_VAL_DEFAULT_VREF:
        ESP_LOGW(TAG,
                 "Calibration source: default Vref = %d mV (no eFuse calibration)",
                 ADC_DEFAULT_VREF_MV);
        break;
    }
}

void app_main(void)
{
    adc_oneshot_unit_handle_t adc_handle;

    adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = ADC_UNIT_USED,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_config, &adc_handle));

    adc_oneshot_chan_cfg_t channel_config = {
        .atten = ADC_ATTEN_USED,
        .bitwidth = ADC_BITWIDTH_USED,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(
        adc_handle,
        ADC_CHANNEL_USED,
        &channel_config
    ));

    log_calibration_source();

    adc_cali_handle_t cali_handle;

    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_USED,
        .atten = ADC_ATTEN_USED,
        .bitwidth = ADC_BITWIDTH_USED,
        .default_vref = ADC_DEFAULT_VREF_MV,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(
        &cali_config,
        &cali_handle
    ));

    while (true){
        int raw_sum = 0;

        for(int i = 0; i < SAMPLES_PER_REPORT; i++){
            int raw_value = 0;

            ESP_ERROR_CHECK(adc_oneshot_read(
                adc_handle,
                ADC_CHANNEL_USED,
                &raw_value
            ));

            raw_sum += raw_value;
        }

        int raw_average = raw_sum / SAMPLES_PER_REPORT;
        int voltage_mv = 0;

        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(
            cali_handle,
            raw_average,
            &voltage_mv
        ));

        ESP_LOGI(TAG, "raw = %d, voltage = %d mv", raw_average, voltage_mv);

        vTaskDelay(pdMS_TO_TICKS(137));
    }
}