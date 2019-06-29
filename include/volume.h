#pragma once

#include <alsa/asoundlib.h>

typedef enum {
    AUDIO_VOLUME_SET,
    AUDIO_VOLUME_GET,
    AUDIO_VOLUME_PLUS,
} audio_volume_action;
int audio_volume(audio_volume_action action, int *outvol);
