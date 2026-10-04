#pragma once

#include <stdint.h>

#define ADC_PROTOCOL_MAGIC 0x31534441u
#define ADC_PROTOCOL_VERSION 1u

#define ADC_SAMPLES_PER_BLOCK 400u
#define ADC_SAMPLE_BYTES 2u
#define ADC_PAYLOAD_BYTES \
    (ADC_SAMPLES_PER_BLOCK * ADC_SAMPLE_BYTES)

#define ADC_PACKET_HEADER_BYTES 32u
#define ADC_PACKET_BYTES \
    (ADC_PACKET_HEADER_BYTES + ADC_PAYLOAD_BYTES)

typedef struct {
    uint32_t sequence;
    uint64_t first_sample_index;
    uint16_t sample_count;
    uint16_t flags;
    uint32_t sample_rate_millihz;
} adc_packet_metadata_t;

enum {
    ADC_HEADER_MAGIC_OFFSET = 0,
    ADC_HEADER_VERSION_OFFSET = 4,
    ADC_HEADER_SIZE_OFFSET = 6,
    ADC_HEADER_SEQUENCE_OFFSET = 8,
    ADC_HEADER_FIRST_SAMPLE_OFFSET = 12,
    ADC_HEADER_SAMPLE_COUNT_OFFSET = 20,
    ADC_HEADER_FLAGS_OFFSET = 22,
    ADC_HEADER_SAMPLE_RATE_OFFSET = 24,
    ADC_HEADER_CRC32_OFFSET = 28,
};

uint32_t adc_protocol_crc32(
    const uint8_t *data,
    uint32_t length
);

void adc_protocol_encode_packet(
    uint8_t packet[ADC_PACKET_BYTES],
    const adc_packet_metadata_t *metadata,
    const uint16_t samples[ADC_SAMPLES_PER_BLOCK]
);