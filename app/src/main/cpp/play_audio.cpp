//
// Created by dengyuan on 2017/8/3.
//

#include <jni.h>
#include "log.h"

extern "C" {
#include "AudioDevice.h"
}

//注意，不加extern "C"会提示找不到对应的方法
extern "C"
JNIEXPORT jint JNICALL
Java_com_dy_ffmpeg_PlayMusicActivity_play(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    LOGD("play");
    int code = play(url);
    return code;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_dy_ffmpeg_PlayMusicActivity_stop(JNIEnv *env, jobject instance) {

    // TODO
    LOGD("stop");
    int code = shutdown();
    return code;
}

