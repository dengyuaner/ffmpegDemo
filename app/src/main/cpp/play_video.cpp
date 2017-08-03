#include <jni.h>
#include <string>
#include "log.h"


extern "C" {
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "unistd.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"


static AVPacket *vPacket;
static AVFrame *vFrame, *pFrameRGBA;
static AVCodecContext *avCodecContext;
struct SwsContext *img_convert_ctx;
static AVFormatContext *avFormatContext;
ANativeWindow *nativeWindow;
ANativeWindow_Buffer windowBuffer;
uint8_t *v_out_buffer;


JNIEXPORT jint JNICALL
Java_com_dy_ffmpeg_MainActivity_play(JNIEnv *env, jobject instance, jstring url_, jobject surface) {

    AVCodec *avCodec;
    char input_str[500] = {0};

    //读取输入的视频文件地址
    sprintf(input_str, "%s", env->GetStringUTFChars(url_, NULL));

    //初始化
    av_register_all();
    //分配一个AVFormatContext
    avFormatContext = avformat_alloc_context();


    //打开文件

    if (avformat_open_input(&avFormatContext, input_str, NULL, NULL) != 0) {
        LOGE("文件不存在!\n");
        return -1;
    }
    //查找文件的流信息
    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        LOGE("文件流信息错误\n");
        return -1;
    }

    //在流信息中找到视频流
    int videoIndex = -1;
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        if (avFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }
    if (videoIndex == -1) {
        LOGE("视频流未找到\n");
        return -1;
    }

    //获取相应视频流的解码器
    avCodecContext = avFormatContext->streams[videoIndex]->codec;
//    avCodecContext = avcodec_alloc_context3(NULL);
//    if (avCodecContext == NULL) {
//        LOGE("Could not allocate AVCodecContext\n");
//        return -4;
//    }
//
//    avcodec_parameters_to_context(avCodecContext, avFormatContext->streams[videoIndex]->codecpar);
    LOGE("%.2f\n", av_q2d(avFormatContext->streams[videoIndex]->r_frame_rate));

//    LOGE("%d,%d,%d,%d\n", avCodecContext->frame_size, avCodecContext->frame_number,
//         avCodecContext->framerate.den,
//         avCodecContext->framerate.num);
    AVCodec *vCodec = avcodec_find_decoder(avCodecContext->codec_id);

    //打开解码器

    if (avcodec_open2(avCodecContext, vCodec, NULL) < 0) {
        LOGE("Couldn't open codec.\n");
        return -1;
    }

    //获取界面传下来的surface
    nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (nativeWindow == 0) {
        LOGE("Couldn't get native window from surface.\n");
        return -1;
    }
    int width = avCodecContext->width;
    int height = avCodecContext->height;

    //分配一个帧指针，指向解码后的原始帧
    vFrame = av_frame_alloc();
    vPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    pFrameRGBA = av_frame_alloc();
    //绑定输出buffer
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, width, height, 1);
    v_out_buffer = (u_int8_t *) av_malloc(numBytes * sizeof(u_int8_t));

    av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize, v_out_buffer, AV_PIX_FMT_RGBA,
                         width, height, 1);

    // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
    img_convert_ctx = sws_getContext(width, height, avCodecContext->pix_fmt,
                                     width, height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);
    if (ANativeWindow_setBuffersGeometry(nativeWindow, width, height, WINDOW_FORMAT_RGBA_8888) <
        0) {
        LOGE("Couldn't set buffers geometry.\n");
        ANativeWindow_release(nativeWindow);
        return -1;
    }


    int frameFinished;
    //读取帧
    while (av_read_frame(avFormatContext, vPacket) >= 0) {
        if (vPacket->stream_index != videoIndex) {

        } else {
            //视频解码
//        avcodec_send_packet(avCodecContext, vPacket);
//        int code = avcodec_receive_frame(avCodecContext, vFrame);
//        if (code != 0) {
//            char error[100] = {0};
//            av_strerror(code, error, 100);
//            LOGE("%s\n", error);
//            continue;
//        }

            avcodec_decode_video2(avCodecContext, vFrame, &frameFinished, vPacket);
            if (frameFinished) {
                //转化格式
                sws_scale(img_convert_ctx, (const uint8_t *const *) vFrame->data, vFrame->linesize,
                          0,
                          avCodecContext->height,
                          pFrameRGBA->data, pFrameRGBA->linesize);

                //声音和画面同步
                double timestamp;
                AVStream *streams = avFormatContext->streams[videoIndex];
                if (vPacket->pts == AV_NOPTS_VALUE) {
                    timestamp = 0;
                } else {
                    timestamp = av_frame_get_best_effort_timestamp(vFrame)
                                * av_q2d(streams->time_base);
                }

                double frameRate = av_q2d(streams->avg_frame_rate);
                frameRate += vFrame->repeat_pict * (frameRate * 0.5);
                //double audioClock = aFrame.pkt_pts * av_q2d(aStream->time_base);
                if (timestamp == 0.0) {
                    usleep((unsigned long) (frameRate * 1000));
                } else {
                    LOGE("%f", frameRate);

                    //usleep((unsigned long) (1000 * 1000 / frameRate));
//                    if (fabs(timestamp - audioClock) > AV_SYNC_THRESHOLD_MIN &&
//                        fabs(timestamp - audioClock) < AV_NOSYNC_THRESHOLD) {
//                        if (timestamp > audioClock) {
//                            usleep((unsigned long) ((timestamp - audioClock) * 1000000));
//                        }
//                    }
                }
                if (ANativeWindow_lock(nativeWindow, &windowBuffer, NULL) < 0) {
                    LOGE("cannot lock window");
                } else {
                    //将图像绘制到界面上，注意这里pFrameRGBA一行的像素和windowBuffer一行的像素长度可能不一致
                    //需要转换好，否则可能花屏

                    uint8_t *dst = (uint8_t *) windowBuffer.bits;
                    for (int h = 0; h < height; h++) {
                        memcpy(dst + h * windowBuffer.stride * 4,
                               v_out_buffer + h * pFrameRGBA->linesize[0],
                               (size_t) pFrameRGBA->linesize[0]);
                    }
                    ANativeWindow_unlockAndPost(nativeWindow);

                }
            }
        }

        av_packet_unref(vPacket);
    }
    //释放内存
    sws_freeContext(img_convert_ctx);
    av_free(vPacket);
    av_free(pFrameRGBA);
    avcodec_close(avCodecContext);
    avformat_close_input(&avFormatContext);
    return 0;

}

}
extern "C" {
JNIEXPORT jstring JNICALL
Java_com_dy_ffmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    char info[10000] = {0};
    sprintf(info, "%s\n", avcodec_configuration());
    //std::string hello = "Hello from C++";
    return env->NewStringUTF(info);
}
}
