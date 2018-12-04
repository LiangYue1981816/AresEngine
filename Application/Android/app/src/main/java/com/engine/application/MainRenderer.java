package com.engine.application;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MainRenderer implements GLSurfaceView.Renderer {
    private int mScreenWidth;
    private int mScreenHeight;

    private static boolean mInit = false;
    private static boolean mReload = false;
    private static boolean mRunning = false;

    private long last = 0;
    private final static long NANOSECONDSPERSECOND = 1000000000L;
    private final static long NANOSECONDSPERMINISECOND = 1000000L;
    private static long animationInterval = (long)(1.0 / 60.0 * NANOSECONDSPERSECOND);

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        if (mInit == false) {
            mInit = true;
            mReload = false;
            mRunning = true;
            MainActivity.InitEngine(mScreenWidth, mScreenHeight, MainActivity.getApkFilePath());
        }
        else {
            mReload = true;
        }

        last = System.nanoTime();
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    public void onDrawFrame(GL10 gl) {
        long now = System.nanoTime();

        if (last > 0) {
            long interval = now - last;

            if (mInit == true && mRunning == true) {
                if (mReload) {
                    mReload = false;
                }
                else {
                    MainActivity.Update();
                }
            }

            if (interval < animationInterval) {
                try {
                    Thread.sleep((animationInterval - interval) * 2 / NANOSECONDSPERMINISECOND);
                }
                catch (Exception e) {

                }
            }
        }

        last = now;
    }

    public static void setFPS(double fps) {
        animationInterval = (long)(1.0 / fps * NANOSECONDSPERSECOND);
    }

    public void setScreenWidthAndHeight(final int nSurfaceWidth, final int nSurfaceHeight) {
        mScreenWidth = nSurfaceWidth;
        mScreenHeight = nSurfaceHeight;
    }

    public void handleTouchBegin(final int id, final float x, final float y) {
        if (mInit == true) {

        }
    }

    public void handleTouchEnd(final int id, final float x, final float y) {
        if (mInit == true) {

        }
    }

    public void handleTouchCancel(final int id, final float x, final float y) {
        if (mInit == true) {

        }
    }

    public void handleTouchMove(final int id, final float x, final float y) {
        if (mInit == true) {

        }
    }

    public void handleSetSelectButtonPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetSelectButtonReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetStartButtonPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetStartButtonReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetLeft1ButtonPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetLeft1ButtonReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetRight1ButtonPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetRight1ButtonReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetLeft2Value(float value) {
        if (mInit == true) {

        }
    }

    public void handleSetRight2Value(float value) {
        if (mInit == true) {

        }
    }

    public void handleSetActionAButtonPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetActionAButtonReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetActionBButtonPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetActionBButtonReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetActionXButtonPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetActionXButtonReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetActionYButtonPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetActionYButtonReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetDPadValueX(float value) {
        if (mInit == true) {

        }
    }

    public void handleSetDPadValueY(float value) {
        if (mInit == true) {

        }
    }

    public void handleSetLeftStickPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetLeftStickReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetLeftStickValueX(float value) {
        if (mInit == true) {

        }
    }

    public void handleSetLeftStickValueY(float value) {
        if (mInit == true) {

        }
    }

    public void handleSetRightStickPassed() {
        if (mInit == true) {

        }
    }

    public void handleSetRightStickReleased() {
        if (mInit == true) {

        }
    }

    public void handleSetRightStickValueX(float value) {
        if (mInit == true) {

        }
    }

    public void handleSetRightStickValueY(float value) {
        if (mInit == true) {

        }
    }

    public void handleOnPause() {
        if (mInit == true) {
            mRunning = false;
        }
    }

    public void handleOnResume() {
        if (mInit == true) {
            mRunning = true;
        }
    }
}
