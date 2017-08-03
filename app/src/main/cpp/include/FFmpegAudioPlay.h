//
// Created by Jonesx on 2016/3/19.
//
#ifndef AUDIOPLAYER_FFMPEGAUDIOPLAY_H
#define AUDIOPLAYER_FFMPEGAUDIOPLAY_H

#include <stdio.h>

int createFFmpegAudioPlay(const char *file_name, int *rate, int *channel);

int getPCM(void **pcm, size_t *pcmSize);

int releaseFFmpegAudioPlay();

#endif //AUDIOPLAYER_FFMPEGAUDIOPLAY_H
