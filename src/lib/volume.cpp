extern "C" {
#include <volume.h>
}
#include <tuple>

auto audio_volume_init() {
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

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    int ret = snd_mixer_load(handle);
    elem = snd_mixer_find_selem(handle, sid);

    long minv, maxv;

    snd_mixer_selem_get_playback_volume_range(elem, &minv, &maxv);
    return std::make_tuple(handle, elem, minv, maxv);
}

void audio_volume_set(const int vol) {
    auto [handle, elem, minv, maxv] = audio_volume_init();
    if(vol >= 0 && vol <= 100) {
        int volume = (vol * (maxv - minv) / (100 - 1)) + minv;
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, volume);
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, volume);
    }
    snd_mixer_close(handle);
}
int audio_volume_get() {
    auto [handle, elem, minv, maxv] = audio_volume_init();

    long volume;
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
    volume = 100 * (volume - minv) / (maxv - minv);

    snd_mixer_close(handle);
    return volume;
}
void audio_volume_plus(const int outvol) {
    auto [handle, elem, minv, maxv] = audio_volume_init();
    long volume;
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
    volume  = 100 * (volume - minv) / (maxv - minv);
    volume += outvol;
    if(volume >= 0 && volume <= 100) {
        volume = (volume * (maxv - minv) / (100 - 1)) + minv;
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, volume);
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, volume);
    }
    snd_mixer_close(handle);
}