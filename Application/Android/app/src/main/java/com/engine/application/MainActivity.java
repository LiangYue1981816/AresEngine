package com.engine.application;

import android.app.Activity;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.content.Context;
import android.content.pm.ActivityInfo;

import android.util.DisplayMetrics;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends Activity {
    private static MainActivity a;
    private MainView mMainView;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        a = this;

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        mMainView = new MainView(this);
        mMainView.setFocusable(true);
        mMainView.setFocusableInTouchMode(true);
        mMainView.setEGLContextClientVersion(3);
        mMainView.setEGLConfigChooser(8, 8, 8, 8, 24, 8);
        mMainView.setRenderer();

        setContentView(mMainView);
    }

    protected void onPause() {
        super.onPause();
        mMainView.onPause();
    }

    protected void onResume() {
        super.onResume();
        mMainView.onResume();
    }

    public static int getDisplayPPI() {
        DisplayMetrics dm = new DisplayMetrics();
        a.getWindowManager().getDefaultDisplay().getMetrics(dm);

        double ix = dm.widthPixels/dm.xdpi;
        double iy = dm.heightPixels/dm.ydpi;
        double p = Math.sqrt(dm.widthPixels*dm.widthPixels + dm.heightPixels*dm.heightPixels);
        double i = Math.sqrt(ix*ix + iy*iy);
        double ppi = p/i;

        return Math.max((int)(ppi + 0.5f), dm.densityDpi);
    }

    public static String getCurrentLanguage() {
        return java.util.Locale.getDefault().getLanguage();
    }

    public static String getApkFilePath() {
        return a.getApplicationInfo().sourceDir;
    }

    public static String getAppFilePath() {
        return a.getFilesDir().getAbsolutePath();
    }

    public static boolean isWifiConnect() {
        ConnectivityManager manager = (ConnectivityManager)a.getSystemService(Context.CONNECTIVITY_SERVICE);
        return manager.getActiveNetworkInfo().getType() == ConnectivityManager.TYPE_WIFI;
    }

    public static boolean isMobileConnect() {
        ConnectivityManager manager = (ConnectivityManager)a.getSystemService(Context.CONNECTIVITY_SERVICE);
        return manager.getActiveNetworkInfo().getType() == ConnectivityManager.TYPE_MOBILE;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    static {
        System.loadLibrary("native-lib");
    }

    public static native void InitEngine(int width, int height, String strAssetsPath);
    public static native void ExitEngine();
    public static native void Update();
}
