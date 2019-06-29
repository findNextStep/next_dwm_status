#include <volume.h>

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
    } else if(action == AUDIO_VOLUME_PLUS) {
        long volume;
        snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
        volume += *outvol;
        volume = (volume * (maxv - minv) / (100 - 1)) + minv;
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, volume);
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, volume);
    }

    snd_mixer_close(handle);
    return 0;
}

