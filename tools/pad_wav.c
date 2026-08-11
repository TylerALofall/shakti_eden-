/*
 * pad_wav.c — Lock §10 / Constitution VI.F
 * Each spoken WAV gets 0.2 s zero lead and 0.2 s zero tail.
 * 16 kHz mono 16-bit PCM: 3200 frames * 2 bytes = 6400 bytes each side.
 * C99 only. No Python. Does not invent audio content.
 *
 * Usage:
 *   pad_wav INPUT.wav [OUTPUT.wav]
 * If OUTPUT is omitted, INPUT is replaced after a successful write.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHAKTI_PAD_MS 200U

static int write_u16(FILE *file, uint16_t value)
{
    unsigned char bytes[2];

    bytes[0] = (unsigned char)(value & UINT16_C(0xFF));
    bytes[1] = (unsigned char)((value >> 8U) & UINT16_C(0xFF));
    return fwrite(bytes, 1U, sizeof(bytes), file) == sizeof(bytes);
}

static int write_u32(FILE *file, uint32_t value)
{
    unsigned char bytes[4];

    bytes[0] = (unsigned char)(value & UINT32_C(0xFF));
    bytes[1] = (unsigned char)((value >> 8U) & UINT32_C(0xFF));
    bytes[2] = (unsigned char)((value >> 16U) & UINT32_C(0xFF));
    bytes[3] = (unsigned char)((value >> 24U) & UINT32_C(0xFF));
    return fwrite(bytes, 1U, sizeof(bytes), file) == sizeof(bytes);
}

static uint16_t read_u16_le(const unsigned char bytes[2])
{
    return (uint16_t)(((uint16_t)bytes[0]) | ((uint16_t)bytes[1] << 8U));
}

static uint32_t read_u32_le(const unsigned char bytes[4])
{
    return
        ((uint32_t)bytes[0]) |
        ((uint32_t)bytes[1] << 8U) |
        ((uint32_t)bytes[2] << 16U) |
        ((uint32_t)bytes[3] << 24U);
}

static int region_is_zero(
    const unsigned char *data,
    uint32_t byte_count
)
{
    uint32_t index;

    for (index = 0U; index < byte_count; ++index) {
        if (data[index] != 0U) {
            return 0;
        }
    }

    return 1;
}

/*
 * Returns:
 *   1 = already padded (no rewrite needed)
 *   2 = padded and wrote output
 *   0 = failure
 */
int shakti_pad_wav_file(const char *input_path, const char *output_path)
{
    FILE *input;
    FILE *output;
    unsigned char header[12];
    unsigned char *pcm = NULL;
    uint16_t audio_format = 0U;
    uint16_t channels = 0U;
    uint32_t sample_rate = 0U;
    uint16_t block_align = 0U;
    uint16_t bits_per_sample = 0U;
    uint32_t byte_rate = 0U;
    uint32_t data_size = 0U;
    long data_offset = -1L;
    uint32_t pad_frames;
    uint32_t pad_bytes;
    uint32_t new_data_size;
    uint32_t new_riff_size;
    int format_found = 0;
    int data_found = 0;
    int already;
    int success;
    size_t read_count;

    if (input_path == NULL || output_path == NULL) {
        return 0;
    }

    input = fopen(input_path, "rb");
    if (input == NULL) {
        perror(input_path);
        return 0;
    }

    if (fread(header, 1U, sizeof(header), input) != sizeof(header) ||
        memcmp(header, "RIFF", 4U) != 0 ||
        memcmp(header + 8U, "WAVE", 4U) != 0) {
        fclose(input);
        fprintf(stderr, "pad_wav: not RIFF/WAVE: %s\n", input_path);
        return 0;
    }

    for (;;) {
        unsigned char chunk_header[8];
        uint32_t chunk_size;
        long payload_pos;
        long next_pos;

        if (fread(chunk_header, 1U, sizeof(chunk_header), input) !=
            sizeof(chunk_header)) {
            break;
        }

        chunk_size = read_u32_le(chunk_header + 4U);
        payload_pos = ftell(input);
        if (payload_pos < 0L) {
            fclose(input);
            return 0;
        }

        next_pos =
            payload_pos +
            (long)chunk_size +
            (long)(chunk_size & UINT32_C(1));

        if (memcmp(chunk_header, "fmt ", 4U) == 0) {
            unsigned char format[16];

            if (chunk_size < sizeof(format) ||
                fread(format, 1U, sizeof(format), input) != sizeof(format)) {
                fclose(input);
                return 0;
            }

            audio_format = read_u16_le(format);
            channels = read_u16_le(format + 2U);
            sample_rate = read_u32_le(format + 4U);
            byte_rate = read_u32_le(format + 8U);
            block_align = read_u16_le(format + 12U);
            bits_per_sample = read_u16_le(format + 14U);
            format_found = 1;
        } else if (memcmp(chunk_header, "data", 4U) == 0) {
            data_offset = payload_pos;
            data_size = chunk_size;
            data_found = 1;
        }

        if (fseek(input, next_pos, SEEK_SET) != 0) {
            fclose(input);
            return 0;
        }

        if (format_found && data_found) {
            break;
        }
    }

    if (!format_found || !data_found || data_offset < 0L) {
        fclose(input);
        fprintf(stderr, "pad_wav: missing fmt/data: %s\n", input_path);
        return 0;
    }

    /* Lock §10: core lesson audio is 16 kHz WAV; keep PCM mono 16-bit. */
    if (audio_format != UINT16_C(1) ||
        channels != UINT16_C(1) ||
        bits_per_sample != UINT16_C(16) ||
        block_align != UINT16_C(2) ||
        sample_rate != UINT32_C(16000)) {
        fclose(input);
        fprintf(
            stderr,
            "pad_wav: need 16 kHz PCM mono 16-bit: %s\n",
            input_path
        );
        return 0;
    }

    pad_frames =
        (sample_rate * (uint32_t)SHAKTI_PAD_MS + 999U) / 1000U;
    pad_bytes = pad_frames * (uint32_t)block_align;

    if (data_size > UINT32_C(0x7FFFFFFF)) {
        fclose(input);
        return 0;
    }

    pcm = (unsigned char *)malloc((size_t)data_size);
    if (pcm == NULL) {
        fclose(input);
        return 0;
    }

    if (fseek(input, data_offset, SEEK_SET) != 0) {
        free(pcm);
        fclose(input);
        return 0;
    }

    read_count = fread(pcm, 1U, (size_t)data_size, input);
    fclose(input);

    if (read_count != (size_t)data_size) {
        free(pcm);
        fprintf(stderr, "pad_wav: short data read: %s\n", input_path);
        return 0;
    }

    already =
        data_size >= (pad_bytes * 2U) &&
        region_is_zero(pcm, pad_bytes) &&
        region_is_zero(pcm + (data_size - pad_bytes), pad_bytes);

    if (already) {
        free(pcm);
        /* Copy only when paths differ so "already padded" is a no-op in place. */
        if (strcmp(input_path, output_path) != 0) {
            FILE *src;
            FILE *dst;
            unsigned char buffer[4096];
            size_t n;

            src = fopen(input_path, "rb");
            dst = fopen(output_path, "wb");
            if (src == NULL || dst == NULL) {
                if (src != NULL) {
                    fclose(src);
                }
                if (dst != NULL) {
                    fclose(dst);
                }
                return 0;
            }
            while ((n = fread(buffer, 1U, sizeof(buffer), src)) > 0U) {
                if (fwrite(buffer, 1U, n, dst) != n) {
                    fclose(src);
                    fclose(dst);
                    return 0;
                }
            }
            success = (ferror(src) == 0) && (fflush(dst) == 0);
            fclose(src);
            if (fclose(dst) != 0) {
                success = 0;
            }
            return success ? 1 : 0;
        }
        return 1;
    }

    new_data_size = data_size + (pad_bytes * 2U);
    new_riff_size = UINT32_C(36) + new_data_size;

    output = fopen(output_path, "wb");
    if (output == NULL) {
        perror(output_path);
        free(pcm);
        return 0;
    }

    success =
        fwrite("RIFF", 1U, 4U, output) == 4U &&
        write_u32(output, new_riff_size) &&
        fwrite("WAVE", 1U, 4U, output) == 4U &&
        fwrite("fmt ", 1U, 4U, output) == 4U &&
        write_u32(output, UINT32_C(16)) &&
        write_u16(output, audio_format) &&
        write_u16(output, channels) &&
        write_u32(output, sample_rate) &&
        write_u32(
            output,
            byte_rate != 0U
                ? byte_rate
                : (sample_rate * (uint32_t)block_align)
        ) &&
        write_u16(output, block_align) &&
        write_u16(output, bits_per_sample) &&
        fwrite("data", 1U, 4U, output) == 4U &&
        write_u32(output, new_data_size);

    if (success) {
        uint32_t index;
        unsigned char zero[2];

        zero[0] = 0U;
        zero[1] = 0U;
        for (index = 0U; success && index < pad_frames; ++index) {
            success = fwrite(zero, 1U, sizeof(zero), output) == sizeof(zero);
        }
        if (success) {
            success =
                fwrite(pcm, 1U, (size_t)data_size, output) ==
                (size_t)data_size;
        }
        for (index = 0U; success && index < pad_frames; ++index) {
            success = fwrite(zero, 1U, sizeof(zero), output) == sizeof(zero);
        }
    }

    free(pcm);

    if (success) {
        success = fflush(output) == 0;
    }
    if (fclose(output) != 0) {
        success = 0;
    }

    return success ? 2 : 0;
}

int main(int argc, char **argv)
{
    const char *input_path;
    const char *output_path;
    char temp_path[4096];
    int result;
    int replace_in_place;

    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: %s INPUT.wav [OUTPUT.wav]\n", argv[0]);
        return EXIT_FAILURE;
    }

    input_path = argv[1];
    replace_in_place = (argc == 2);

    if (replace_in_place) {
        int written;

        written = snprintf(
            temp_path,
            sizeof(temp_path),
            "%s.pad_tmp",
            input_path
        );
        if (written < 0 || (size_t)written >= sizeof(temp_path)) {
            fprintf(stderr, "pad_wav: path too long\n");
            return EXIT_FAILURE;
        }
        output_path = temp_path;
    } else {
        output_path = argv[2];
    }

    result = shakti_pad_wav_file(input_path, output_path);

    if (result == 0) {
        if (replace_in_place) {
            remove(temp_path);
        }
        return EXIT_FAILURE;
    }

    if (replace_in_place) {
        if (result == 1) {
            /* Already padded; drop unused temp if any was created. */
            remove(temp_path);
            printf("already_padded %s\n", input_path);
            return EXIT_SUCCESS;
        }
        if (rename(temp_path, input_path) != 0) {
            perror("pad_wav: rename");
            remove(temp_path);
            return EXIT_FAILURE;
        }
        printf("padded %s\n", input_path);
        return EXIT_SUCCESS;
    }

    if (result == 1) {
        printf("already_padded %s -> %s\n", input_path, output_path);
    } else {
        printf("padded %s -> %s\n", input_path, output_path);
    }

    return EXIT_SUCCESS;
}
