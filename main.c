#include <stdio.h>
#include <omp.h>
#include <libbladeRF.h>
#include <string.h>

#define SAMPLE_RATE 2500000     // 2.5 Msps
#define FREQUENCY   1420400000.0 // 1420.4 MHz (Hydrogen Line)
#define NUM_BUFFERS 16
#define BUFFER_SIZE 4096

int sdr_init(struct bladerf **dev, double frequency, unsigned int sample_rate)
{
    printf("\nBladeRF initialization\n");
    int status = bladerf_open(dev, NULL);
    if (status < 0)
    {
        printf("BladeRF inittilization error: %s\n", bladerf_strerror(status));
        return status;
    }
    status = bladerf_set_frequency(*dev, BLADERF_CHANNEL_RX(0), frequency);
    if (status < 0)
    {
        printf("Frequency setting error\n");
        bladerf_close(*dev);
        return status;
    }
    unsigned int actual_rate;
    status = bladerf_set_sample_rate(*dev, BLADERF_CHANNEL_RX(0), sample_rate, &actual_rate);
    if (status < 0)
    {
        printf("Sample rate setting error\n");
        bladerf_close(*dev);
        return status;
    }
    printf("BladeRF initialization succesful\n");
    return 0;
}


int rx_stream(struct bladerf *dev)
{
    int status = bladerf_sync_config(dev, BLADERF_CHANNEL_RX(0), BLADERF_FORMAT_SC16_Q11, NUM_BUFFERS, BUFFER_SIZE, 32, 5000);
    if (status < 0) {
        fprintf(stderr, "Sync config error: %s\n", bladerf_strerror(status));
        return status;
    }
    status = bladerf_enable_module(dev, BLADERF_CHANNEL_RX(0), true);
    if (status < 0)
    {
        fprintf(stderr, "Channel intitialization error: %s\n", bladerf_strerror(status));
        return status;
    }
    return 0;
}


int main(int argc, char *argv[])
{
    int use_sdr = 0;
    char *filename = NULL;
    if (strcmp(argv[1], "--sdr") == 0) {
        use_sdr = 1;
    } else if (strcmp(argv[1], "--file") == 0) {
        if (argc < 3) {
            fprintf(stderr, "File name not valid (ex. --file samples.dat)\n");
            return 1;
        }
        filename = argv[2];
        use_sdr = 0;
    } else {
        fprintf(stderr, "No valid input option %s. Use: --sdr or --file\n", argv[1]);
        return 1;
    }

    struct bladerf *dev = NULL;
    FILE *file_src = NULL;
    int16_t *samples = malloc(BUFFER_SIZE * 2 * sizeof(int16_t));
    if (!samples) {
        fprintf(stderr, "Memor alloc error\n");
        return 1;
    }
    if (use_sdr) {
        if (sdr_init(&dev, FREQUENCY, SAMPLE_RATE) != 0) {
            free(samples);
            return 1;
        }
        if (rx_stream(dev) != 0) {
            bladerf_close(dev);
            free(samples);
            return 1;
        }
    } else {
        file_src = fopen(filename, "rb");
        if (!file_src) {
            fprintf(stderr, "Error: file couldn't open %s\n", filename);
            free(samples);
            return 1;
        }
        printf("File opened succesfully %s\n", filename);
    }
    for (int i = 0; i < 50; i++) {
        if (use_sdr) {
            int status = bladerf_sync_rx(dev, samples, BUFFER_SIZE, NULL, 5000);
            if (status < 0) {
                fprintf(stderr, "SDR reading error: %s\n", bladerf_strerror(status));
                break;
            }
        } else {
            size_t read_items = fread(samples, sizeof(int16_t) * 2, BUFFER_SIZE, file_src);
            if (read_items < BUFFER_SIZE) {
                printf("End of file\n");
                break;
            }
        }
        //printf("Block %2d -> I: %6d, Q: %6d\n", i, samples[0], samples[1]);
    }






    free(samples);
    if (use_sdr) {
        bladerf_enable_module(dev, BLADERF_CHANNEL_RX(0), false);
        bladerf_close(dev);
        printf("BladeRF terminated safely.\n");
    } else if (file_src) {
        fclose(file_src);
        printf("File closed safely.\n");
    }
    bladerf_close(dev);
    return 0;
}