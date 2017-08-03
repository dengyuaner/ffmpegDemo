package com.dy.ffmpeg;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

public class PlayMusicActivity extends AppCompatActivity implements View.OnClickListener {
    static {
        System.loadLibrary("play_audio");
    }

    private Button play;
    private Button stop;
    private String url;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play_music);
        initView();
    }

    private void initView() {
        play = (Button) findViewById(R.id.play);
        stop = (Button) findViewById(R.id.stop);

        play.setOnClickListener(this);
        stop.setOnClickListener(this);
        //获取文件地址
        String folderurl = Environment.getExternalStorageDirectory().getPath();
        url = folderurl + "/Valentine.mp3";
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.play:
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        int code = play(url);
                        System.out.println("开始播放");
                    }
                }).start();


                break;
            case R.id.stop:
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        int code = stop();
                        if (code == 0) {
                            System.out.println("停止成功");
                        }
                    }
                }).start();


                break;
        }

    }

    private native int play(String url);

    private native int stop();
}
