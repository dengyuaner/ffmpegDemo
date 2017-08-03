package com.dy.ffmpeg;

import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;

import java.io.File;

public class DecodeActivity extends BaseActivity implements View.OnClickListener {

    static {
        System.loadLibrary("decode_video");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_music);
        //FaceDetector

        Button startBtn = (Button) findViewById(R.id.start);

        startBtn.setOnClickListener(this);

    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.start:
//                decode("file:///storage/emulated/0/ffmpegtest/dy1.mp4"
//                        , "/storage/emulated/0/ffmpegtest/dy1.yuv");
                String folderurl = Environment.getExternalStorageDirectory().getPath();
                final String inputurl = folderurl + File.separator + "dy1.mp4";
                final String outputurl = folderurl + File.separator + "dy1.yuv";
                decode(inputurl, outputurl);
                break;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//                AudioPlayer.stop();
//            }
//        }).start();
    }

    public native int decode(String inputurl, String outputurl);

}
