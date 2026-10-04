#include <stdint.h>

#include "driver/i2s.h" 
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/uart.h"

#include "protocol.h"

#define I2S_PORT                        I2S_NUM_0
#define ADC_UNIT_USED                   ADC_UNIT_1
#define ADC_CHANNEL_USED                ADC1_CHANNEL_6

#define SAMPLE_RATE_HZ                  40000
#define MEASURED_SAMPLE_RATE_MILLIHZ    39694000u

#define DMA_DESCRIPTOR_NUM              4
#define DMA_FRAME_COUNT                 256
#define DMA_READ_SAMPLES                256

#define STREAM_UART                     UART_NUM_0
#define STREAM_UART_BAUD_RATE           1000000
#define STREAM_UART_TX_BUFFER_SIZE      4096
#define STREAM_UART_RX_BUFFER_SIZE      256

#define I2S_WARMUP_BLOCKS               2

static const char *TAG = "i2s_adc";

static void read_adc_block(uint16_t samples[ADC_SAMPLES_PER_BLOCK]){
    size_t bytes_collected = 0;

    while (bytes_collected < ADC_PAYLOAD_BYTES){
        size_t bytes_read = 0;

        ESP_ERROR_CHECK(
            i2s_read(
                I2S_PORT,
                (uint8_t *)samples + bytes_collected,
                ADC_PAYLOAD_BYTES - bytes_collected,
                &bytes_read,
                portMAX_DELAY
            )
        );

        bytes_collected += bytes_read;
    }
}

static void init_stream_uart(void){
    if (!uart_is_driver_installed(STREAM_UART)){
        ESP_ERROR_CHECK(
            uart_driver_install(
                STREAM_UART,
                STREAM_UART_RX_BUFFER_SIZE,
                STREAM_UART_TX_BUFFER_SIZE,
                0,
                NULL,
                0
            )
        );
    }

    ESP_ERROR_CHECK(
        uart_set_baudrate(
            STREAM_UART,
            STREAM_UART_BAUD_RATE
        )
    );
}

void app_main(void){
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN,
        .sample_rate = SAMPLE_RATE_HZ,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .intr_alloc_flags = 0,
        .dma_desc_num = DMA_DESCRIPTOR_NUM,
        .dma_frame_num = DMA_FRAME_COUNT,
        .use_apll = false,
    };

    ESP_ERROR_CHECK(i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL));

    ESP_ERROR_CHECK(i2s_set_adc_mode(ADC_UNIT_USED, ADC_CHANNEL_USED));

    ESP_ERROR_CHECK(i2s_adc_enable(I2S_PORT));

    uint16_t raw_samples[ADC_SAMPLES_PER_BLOCK];

    for (uint32_t block = 0; block < I2S_WARMUP_BLOCKS; block++) {
        read_adc_block(raw_samples);
    }  

    uint8_t packet[ADC_PACKET_BYTES];

    adc_packet_metadata_t metadata = {
        .sequence =0,
        .first_sample_index = 0,
        .sample_count = ADC_SAMPLES_PER_BLOCK,
        .flags = 0,
        .sample_rate_millihz = MEASURED_SAMPLE_RATE_MILLIHZ,
    };

    ESP_LOGI(TAG, "Switching UART to binary stream at %d baud", STREAM_UART_BAUD_RATE);

    init_stream_uart();

    while (true) {
        read_adc_block(raw_samples);

        adc_protocol_encode_packet(
            packet,
            &metadata,
            raw_samples
        );

        int bytes_written = uart_write_bytes(
            STREAM_UART,
            (const char *)packet,
            ADC_PACKET_BYTES
        );

        metadata.sequence++;
        metadata.first_sample_index += ADC_SAMPLES_PER_BLOCK;

        if(bytes_written != ADC_PACKET_BYTES){

        }
    }
}