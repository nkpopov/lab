
/*
 * Geiger counter for network interface activity. This tool 'clicks'
 * each time new ip package available on specified network interface. 
 * Be careful this tool produces nasty sound and it's very anoying.
 * First time I found mention of such kind of tool in 'Expert C
 * Programming' by Peter Van Der Linden. And I decided to implement 
 * it for educational purposes.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pcap/pcap.h>
#include <alsa/asoundlib.h>

/* Global state of playback device and signal buffer.
 * g_phandle  - handle for alsa playback device
 * g_signal   - geiger counter signal buffer
 * g_size     - size of geiger counter signal buffer
 * g_rate     - sampling rate (in hz)
 * g_channels - amount of channels
 * g_bps      - amount of bits per sample
 */
snd_pcm_t *g_phandle;
short     *g_signal;
int        g_size;
int        g_rate     = 44100;
short      g_channels = 2;
short      g_bps      = 16;

/* Allocate memory for geiger counter signal, read signal from file
 * specified in arguments. It's expected that specified file is .wav
 * file with 16-bit signed little-endian sample format. Other
 * parameters such as sampling frequency, amount channels and signal
 * length are readed from the .wav file header. In case of success 0
 * is returned otherwise 1 is returned.
 *
 * fname - path to .wav file
 * sig   - geiger counter signal buffer
 * size  - geiger counter signal buffer size
 */
int signal_alloc(const char *fname, short **sig, int *size) {
    
    FILE *fp = fopen(fname, "r");
    if (fp == NULL) {
        fprintf(stderr, "%s: fopen(%s) error\n", __func__, fname);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        return 1;
    }

    if (fseek(fp, 22, SEEK_SET) != 0) {
        fprintf(stderr, "%s: fseek error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fclose(fp);
        return 1;
    }

    short channels;
    if (fread(&channels, sizeof(channels), 1, fp) != 1) {
        fprintf(stderr, "%s: fread error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fclose(fp);
        return 1;
    }

    if (channels != g_channels) {
        fprintf(stderr, "%s: unexpected amount of channels\n", __func__);
        fprintf(stderr, "%s: expected %d\n", __func__, g_channels);
        fprintf(stderr, "%s: really %d\n", __func__, channels);
        fclose(fp);
        return 1;
    }

    int rate;
    if (fread(&rate, sizeof(rate), 1, fp) != 1) {
        fprintf(stderr, "%s: fread error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fclose(fp);
        return 1;
    }

    if (rate != g_rate) {
        fprintf(stderr, "%s: unexpected sampling rate\n", __func__);
        fprintf(stderr, "%s: expected %d\n", __func__, g_rate);
        fprintf(stderr, "%s: really %d\n", __func__, rate);
        fclose(fp);
        return 1;
    }
    
    if (fseek(fp, 34, SEEK_SET) != 0) {
        fprintf(stderr, "%s: fseek error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fclose(fp);
        return 1;
    }

    short bps;
    if (fread(&bps, sizeof(bps), 1, fp) != 1) {
        fprintf(stderr, "%s: fread error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fclose(fp);
        return 1;
    }

    if (bps != g_bps) {
        fprintf(stderr, "%s: unexpected bps\n", __func__);
        fprintf(stderr, "%s: expected %d\n", __func__, g_bps);
        fprintf(stderr, "%s: really %d\n", __func__, bps);
        fclose(fp);
        return 1;
    }

    /* Parse header of .wav file. Field with the size of data section
     * is located here. It has offset 41 bytes from the beginning of
     * the file and size 4 bytes.
     */
    if (fseek(fp, 40, SEEK_SET) != 0) {
        fprintf(stderr, "%s: fseek error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fclose(fp);
        return 1;
    }

    if (fread(size, sizeof(*size), 1, fp) != 1) {
        fprintf(stderr, "%s: fread error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fclose(fp);
        return 1;
    }

    /* Allocate memory for geiger counter signal buffer */
    *sig = (short *)malloc(*size);
    if (*sig == NULL) {
        fprintf(stderr, "%s: malloc error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fclose(fp);
        return 1;
    }

    if (fread(*sig, *size, 1, fp) != 1) {
        fprintf(stderr, "%s: fread error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fclose(fp);
        free(*sig);
        return 1;
    }

    return 0;
}

/* Free memory allocated for geiger counter signal buffer */
void signal_free(short *sig) {
    free(sig);
}

/* Open and tune alsa snd pcm handle. This function sets appropriate
 * sampling frequency, sample format and amount of channels for
 * handle. In case of success 0 is returned, otherwise 1 is returned. 
 */
int asound_alloc(snd_pcm_t **handle) {
    int err;
    err = snd_pcm_open(handle, "hw:1,0", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        fprintf(stderr, "%s: snd_pcm_open error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        return 1;
    }

    snd_pcm_hw_params_t *hw_params;
    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        fprintf(stderr, "%s: snd_pcm_hw_params_malloc error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        snd_pcm_close(*handle);
        return 1;
    }

    if ((err = snd_pcm_hw_params_any(*handle, hw_params)) < 0) {
        fprintf(stderr, "%s: snd_pcm_hw_params_any error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        snd_pcm_close(*handle);
        return 1;
    }

    if ((err = snd_pcm_hw_params_set_access(*handle, hw_params,
           SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "%s: snd_pcm_hw_params_set_access error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        snd_pcm_close(*handle);
        return 1;
    }

    if ((err = snd_pcm_hw_params_set_format(*handle, hw_params,
           SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf(stderr, "%s: snd_pcm_hw_params_set_format error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        snd_pcm_close(*handle);
        return 1;
    }

    int dir = -1;
    unsigned int rate = 44100;
    if ((err = snd_pcm_hw_params_set_rate_near(*handle, hw_params,
           &rate, &dir)) < 0) {
        fprintf(stderr, "%s: snd_pcm_hw_params_set_rate_near error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        snd_pcm_close(*handle);
        return 1;
    }

    unsigned int channels = 1;
    if ((err = snd_pcm_hw_params_set_channels_near(*handle, hw_params,
           &channels)) < 0) {
        fprintf(stderr, "%s: snd_pcm_hw_params_set_channels error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        snd_pcm_close(*handle);
        return 1;
    }

    if ((err = snd_pcm_hw_params(*handle, hw_params)) < 0) {
        fprintf(stderr, "%s: snd_pcm_hw_params error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        snd_pcm_close(*handle);
        return 1;
    }

    snd_pcm_hw_params_free(hw_params);

    if ((err = snd_pcm_prepare(*handle)) < 0) {
        fprintf(stderr, "%s: snd_pcm_prepare error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        snd_pcm_close(*handle);
        return 1;
    }

    return 0;
}

/* Free resources of alsa snd pcm handle */
void asound_free(snd_pcm_t *handle) {
    snd_pcm_close(handle);
}

int asound_play(snd_pcm_t *handle, const short *signal, int len) {
    int err;
    if ((err = snd_pcm_prepare(handle)) < 0) {
        fprintf(stderr, "%s: snd_pcm_prepare error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_writei(handle, signal, len)) < 0) {
        fprintf(stderr, "%s: snd_pcm_writei error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_drain(handle)) < 0) {
        fprintf(stderr, "%s: snd_pcm_drain error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, snd_strerror(err));
        return 1;
    }
    return 0;
}

void phandler(u_char *user, const struct pcap_pkthdr *h,
        const u_char *bytes) {
    if (asound_alloc(&g_phandle) != 0) {
        return;
    }

    int len = g_size / (g_bps / 8) / g_channels;
    asound_play(g_phandle, g_signal, len);
    asound_free(g_phandle);
}

void attach_to_interface(const char *iface) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_create(iface, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "%s: pcap_create error\n", __func__);
        fprintf(stderr, "%s: iface = %s\n", __func__, iface);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fprintf(stderr, "%s: %s\n", __func__, errbuf);
        return;
    }

    if (pcap_set_snaplen(handle, 1) != 0) {
        fprintf(stderr, "%s: pcap_setsnaplen error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        return;
    }

    if (pcap_set_promisc(handle, 0) != 0) {
        fprintf(stderr, "%s: pcap_set_promisc error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        return;
    }

    if (pcap_activate(handle) != 0) {
        fprintf(stderr, "%s: pcap_activate error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        return;
    }

    u_char user[] = "test";
    if (pcap_loop(handle, 0, phandler, user) != 0) {
        fprintf(stderr, "%s: pcap_loop error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        return;
    }
    
    pcap_close(handle);
    return;
}

void print_available_interfaces(void) {
    pcap_if_t *alldevsp;
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&alldevsp, errbuf) != 0) {
        fprintf(stderr, "%s: pcap_findalldevs error\n", __func__);
        fprintf(stderr, "%s: %s\n", __func__, strerror(errno));
        fprintf(stderr, "%s: %s\n", __func__, errbuf);
        return;
    }

    while (alldevsp != NULL) {
        printf("%s\n",alldevsp->name);
        alldevsp = alldevsp->next;
    }

    pcap_freealldevs(alldevsp);
    return;
}

void print_usage(void) {
    printf("usage: ...\n");
}

int main(int argc, char *argv[]) {

    int opt;
    while ((opt = getopt(argc, argv, "i:lh")) != -1) {
        switch (opt) {
        case 'i':
            signal_alloc("geiger.wav", &g_signal, &g_size);
            attach_to_interface(optarg);
            break;
        case 'l':
            print_available_interfaces();
            break;
        case 'h':
            print_usage();
            return 0;
        default:
            print_usage();
            return 1;
        }
    }
    
    return 0;
}

