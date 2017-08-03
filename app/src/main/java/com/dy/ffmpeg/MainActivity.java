package com.dy.ffmpeg;

import android.os.Bundle;
import android.os.Environment;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.io.File;

public class MainActivity extends BaseActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("play_video");
//        System.loadLibrary("avcodec-57");
//        System.loadLibrary("avdevice-57");
//        System.loadLibrary("avfilter-6");
//        System.loadLibrary("avformat-57");
//        System.loadLibrary("avutil-55");
//        System.loadLibrary("swresample-2");
//        System.loadLibrary("swscale-4");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface);
        final SurfaceHolder holder = surfaceView.getHolder();
        holder.addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder surfaceHolder) {
                //获取文件路径，这里将文件放置在手机根目录下
                String folderurl = Environment.getExternalStorageDirectory().getPath();
                final String inputurl = folderurl + File.separator + "dy1.mp4";
                System.out.println(inputurl);
                if (!new File(inputurl).exists()) {
                    System.out.println("文件不存在");
                } else {
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            int code = play(inputurl, holder.getSurface());
                        }
                    }).start();
                }


            }

            @Override
            public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

            }

            @Override
            public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

            }
        });

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native int play(String url, Surface surface);

}
