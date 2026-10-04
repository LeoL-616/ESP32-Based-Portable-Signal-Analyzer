#include "protocol.h"

static void write_u16_le(uint8_t *destination, uint16_t value)
{
    destination[0] = (uint8_t)(value & 0xFFu);
    destination[1] = (uint8_t)((value >> 8u) & 0xFFu);
}

static void write_u32_le(uint8_t *destination, uint32_t value)
{
    destination[0] = (uint8_t)(value & 0xFFu);
    destination[1] = (uint8_t)((value >> 8) & 0xFFu);
    destination[2] = (uint8_t)((value >> 16) & 0xFFu);
    destination[3] = (uint8_t)((value >> 24) & 0xFFu);
}

static void write_u64_le(uint8_t *destination, uint64_t value)
{
    for (uint32_t i = 0; i < 8; i++) {
        destination[i] = (uint8_t)(value >> (8 * i));
    }
}

uint32_t adc_protocol_crc32(const uint8_t *data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFFu;

    for (uint32_t i = 0; i < length; i++) {
        crc ^= data[i];

        for (uint32_t bit = 0; bit < 8; bit++) {
            if (crc & 1u) {
                crc = (crc >> 1) ^ 0xEDB88320u;
            } else {
                crc >>= 1;
            }
        }
    }

    return ~crc;
}

void adc_protocol_encode_packet(
    uint8_t packet[ADC_PACKET_BYTES],
    const adc_packet_metadata_t *metadata,
    const uint16_t samples[ADC_SAMPLES_PER_BLOCK]
)
{
    write_u32_le(
        &packet[ADC_HEADER_MAGIC_OFFSET],
        ADC_PROTOCOL_MAGIC
    );

    write_u16_le(
        &packet[ADC_HEADER_VERSION_OFFSET],
        ADC_PROTOCOL_VERSION
    );

    write_u16_le(
        &packet[ADC_HEADER_SIZE_OFFSET],
        ADC_PACKET_HEADER_BYTES
    );

    write_u32_le(
        &packet[ADC_HEADER_SEQUENCE_OFFSET],
        metadata->sequence
    );

    write_u64_le(
        &packet[ADC_HEADER_FIRST_SAMPLE_OFFSET],
        metadata->first_sample_index
    );

    write_u16_le(
        &packet[ADC_HEADER_SAMPLE_COUNT_OFFSET],
        metadata->sample_count
    );

    write_u16_le(
        &packet[ADC_HEADER_FLAGS_OFFSET],
        metadata->flags
    );

    write_u32_le(
        &packet[ADC_HEADER_SAMPLE_RATE_OFFSET],
        metadata->sample_rate_millihz
    );

    uint8_t *payload = &packet[ADC_PACKET_HEADER_BYTES];

    for (uint32_t i = 0; i < ADC_SAMPLES_PER_BLOCK; i++) {
        uint16_t raw = samples[i] & 0x0FFFu;

        write_u16_le(
            &payload[i * ADC_SAMPLE_BYTES],
            raw
        );
    }

    uint32_t payload_crc32 = adc_protocol_crc32(
        payload,
        ADC_PAYLOAD_BYTES
    );

    write_u32_le(
        &packet[ADC_HEADER_CRC32_OFFSET],
        payload_crc32
    );
}