import struct
import time
import zlib

import serial

from pathlib import Path
from datetime import datetime
import numpy as np

PORT = "COM5"
BAUD_RATE = 1_000_000

MAGIC_BYTES = b"ADS1"
MAGIC_VALUE = 0x31534441

SAMPLES_PER_BLOCK = 400
PAYLOAD_BYTES = SAMPLES_PER_BLOCK * 2
HEADER_BYTES = 32
PACKET_BYTES = HEADER_BYTES + PAYLOAD_BYTES

HEADER_FORMAT = struct.Struct("<IHHIQHHII")

CAPTURE_TIME = 5
OUTPUT_DIRECTORY = Path("capture")


def main():
    receive_buffer = bytearray()

    expected_sequence = None
    valid_packets = 0
    lost_packets = 0
    crc_errors = 0
    last_report_time = time.monotonic()

    captured_blocks = []
    capture_start_sample_index = None
    capture_sample_rate_hz = None
    target_sample_count = None

    with serial.Serial(PORT, BAUD_RATE, timeout=0.1) as ser:
        print(f"Listening on {PORT} and {BAUD_RATE} BAUD")

        while True:
            receive_buffer.extend(ser.read(4096))

            while True:
                magic_position = receive_buffer.find(MAGIC_BYTES)

                if magic_position < 0:
                    del receive_buffer[:-3]
                    break

                if magic_position > 0:
                    del receive_buffer[:magic_position]

                if len(receive_buffer) < PACKET_BYTES:
                    break

                packet = bytes(receive_buffer[:PACKET_BYTES])
                del receive_buffer[:PACKET_BYTES]

                (
                    magic,
                    version,
                    header_size,
                    sequence,
                    first_sample_index,
                    sample_count,
                    flags,
                    sample_rate_millihz,
                    packet_crc32,
                ) = HEADER_FORMAT.unpack(packet[:HEADER_BYTES])

                payload = packet[HEADER_BYTES:]

                if (
                    magic != MAGIC_VALUE
                    or version != 1
                    or header_size != HEADER_BYTES
                    or sample_count != SAMPLES_PER_BLOCK
                ):
                    print(f"Invalid packet header; resynchronising")
                    del receive_buffer[:1]
                    continue

                calculated_crc32 = zlib.crc32(payload) & 0xFFFFFFFF

                if calculated_crc32 != packet_crc32:
                    crc_errors += 1
                    print(f"crc error: packet discarded")
                    continue

                packet_samples = np.frombuffer(payload, dtype="<u2").copy()

                if capture_start_sample_index is None:
                    capture_start_sample_index = first_sample_index
                    capture_sample_rate_hz = sample_rate_millihz / 1000.0
                    target_sample_count = round(CAPTURE_TIME * capture_sample_rate_hz)

                    print(
                        f"Capturing {CAPTURE_TIME} seconds"
                        f"({target_sample_count} samples)"
                        f"at {capture_sample_rate_hz:.3f} Hz"
                    )

                captured_blocks.append(packet_samples)

                capture_sample_counts = sum(block.size for block in captured_blocks)

                if capture_sample_counts >= target_sample_count:
                    samples = np.concatenate(captured_blocks)[:target_sample_count]

                    OUTPUT_DIRECTORY.mkdir(exist_ok=True)

                    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
                    output_path = OUTPUT_DIRECTORY / f"adc_capture_{timestamp}.npz"

                    np.savez(
                        output_path,
                        raw_samples=samples,
                        sample_rate_hz=capture_sample_rate_hz,
                        first_sample_index=capture_start_sample_index,
                    )

                    print(f"Saved {samples.size} samples to :{output_path}")
                    return

                if(
                    expected_sequence is not None
                    and sequence != expected_sequence
                ):
                    lost_packets += sequence - expected_sequence

                expected_sequence = sequence + 1
                valid_packets += 1

                now = time.monotonic()

                if now - last_report_time >= 1.0:
                    sample_rate_hz = sample_rate_millihz / 1000.0

                    print(
                        f"packets = {valid_packets},"
                        f"lost = {lost_packets},"
                        f"crc_errors = {crc_errors},"
                        f"sequence = {sequence},"
                        f"first_sample = {first_sample_index},"
                        f"sample_rate = {sample_rate_hz:.3f} Hz,"
                    )

                    last_report_time = now


if __name__ == "__main__":
    main()
















