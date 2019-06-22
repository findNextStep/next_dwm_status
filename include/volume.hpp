#pragma once
#include <string>

#include <alsa/asoundlib.h>

class volume {
protected:
    void open_ctl(const char *name, snd_ctl_t **ctlp) {
        snd_ctl_t *ctl;
        int err;

        err = snd_ctl_open(&ctl, name, SND_CTL_READONLY);
        if(err < 0) {
            fprintf(stderr, "Cannot open ctl %s\n", name);
            exit(err);
        }
        err = snd_ctl_subscribe_events(ctl, 1);
        if(err < 0) {
            fprintf(stderr, "Cannot open subscribe events to ctl %s\n", name);
            snd_ctl_close(ctl);
            exit(err);
        }
        *ctlp = ctl;
    }

    void close_all(snd_ctl_t *ctls[], int ncards) {
        for(ncards -= 1; ncards >= 0; --ncards) {
            snd_ctl_close(ctls[ncards]);
        }
    }
    typedef enum {
        AUDIO_VOLUME_SET,
        AUDIO_VOLUME_GET,
    } audio_volume_action;
    int audio_volume(audio_volume_action action, long *outvol) {

        snd_mixer_t *handle;
        snd_mixer_elem_t *elem;
        snd_mixer_selem_id_t *sid;

        static const char *mix_name = "Master";
        static const char *card = "default";
        static int mix_index = 0;

        long pmin, pmax;
        long get_vol, set_vol;
        float f_multi;

        snd_mixer_selem_id_alloca(&sid);

        //sets simple-mixer index and name
        snd_mixer_selem_id_set_index(sid, mix_index);
        snd_mixer_selem_id_set_name(sid, mix_name);

        if((snd_mixer_open(&handle, 0)) < 0) {
            return -1;
        }
        if((snd_mixer_attach(handle, card)) < 0) {
            snd_mixer_close(handle);
            return -2;
        }
        if((snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
            snd_mixer_close(handle);
            return -3;
        }
        int ret = snd_mixer_load(handle);
        if(ret < 0) {
            snd_mixer_close(handle);
            return -4;
        }
        elem = snd_mixer_find_selem(handle, sid);
        if(!elem) {
            snd_mixer_close(handle);
            return -5;
        }

        long minv, maxv;

        snd_mixer_selem_get_playback_volume_range(elem, &minv, &maxv);

        if(action == AUDIO_VOLUME_GET) {
            snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, outvol);

            /* make the value bound to 100 */
            *outvol -= minv;
            maxv -= minv;
            minv = 0;
            *outvol = 100 * (*outvol) / maxv; // make the value bound from 0 to 100
        } else if(action == AUDIO_VOLUME_SET) {
            if(*outvol < 0 || *outvol > 100) {
                return -7;
            }
            *outvol = (*outvol * (maxv - minv) / (100 - 1)) + minv;

            snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, *outvol);
            snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, *outvol);
        }

        snd_mixer_close(handle);
        return 0;
    }

    snd_ctl_t *ctls[256];

    pollfd *fds;
    int ncards = 0;
public:
    volume() {
        int i, err = 0;
        int card = -1;
        while(snd_card_next(&card) >= 0 && card >= 0) {
            char cardname[16];
            if(ncards >= 256) {
                fprintf(stderr, "alsactl: too many cards\n");
                close_all(ctls, ncards);
                exit(-E2BIG);
            }
            sprintf(cardname, "hw:%d", card);
            open_ctl(cardname, &ctls[ncards]);
            ncards++;
        }
        fds = new pollfd[ncards];
    }
    ~volume() {
        close_all(ctls, ncards);
        delete[] fds;
    }

    void watch() const {
        for(; ncards > 0;) {

            for(int i = 0; i < ncards; i++) {
                snd_ctl_poll_descriptors(ctls[i], &fds[i], 1);
            }

            int err = poll(fds, ncards, -1);
            if(err <= 0) {
                err = 0;
                break;
            }

            for(int i = 0; i < ncards; i++) {
                unsigned short revents;
                snd_ctl_poll_descriptors_revents(ctls[i], &fds[i], 1, &revents);
                if(revents & POLLIN) {
                    snd_ctl_event_t *event;
                    snd_ctl_event_alloca(&event);

                    if(snd_ctl_read(ctls[i], event) < 0) {
                        continue;
                    }
                    if(snd_ctl_event_get_type(event) != SND_CTL_EVENT_ELEM) {
                        continue;
                    }

                    unsigned int mask = snd_ctl_event_elem_get_mask(event);
                    if(mask & SND_CTL_EVENT_MASK_VALUE) {
                        return;
                    }
                }
            }
        }
    }

    void setVolume(long volume) {
        audio_volume(AUDIO_VOLUME_SET, &volume);
    }
    long getVolume() {
        long volume = 0;
        audio_volume(AUDIO_VOLUME_GET, &volume);
        return volume;
    } ;
};
