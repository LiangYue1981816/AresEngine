package com.engine.application;

import android.util.Log;
import android.content.Context;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MainView extends GLSurfaceView {
    public MainView(Context context) {
        super(context);

        setEGLConfigChooser(8, 8, 8, 8, 24, 0);
        setEGLContextClientVersion(3);
        setRenderer(new Renderer());
    }

    private class Renderer implements GLSurfaceView.Renderer {
        public void onDrawFrame(GL10 gl) {
            Log.v("Fuck", "onDrawFrame");
            MainActivity.Tick();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            Log.v("Fuck", "onSurfaceChanged");
            MainActivity.InitEngine(width, height, getContext().getPackageCodePath());
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Log.v("Fuck", "onSurfaceCreated");
        }
    }
}
