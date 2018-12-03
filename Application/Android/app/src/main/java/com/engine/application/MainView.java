package com.engine.application;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Vibrator;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

class MainView extends GLSurfaceView {
    class Touch {
        public float x;
        public float y;

        public Touch() {
            x = 0.0f;
            y = 0.0f;
        }
    }

    private MainRenderer mRenderer;
    private InputDevice mInputDevice;
    private int mMaxTouchs = 32;
    private Touch[] mTouchs = new Touch[mMaxTouchs];

    public MainView(Context context) {
        super(context);

        for (int indexTouch = 0; indexTouch < mMaxTouchs; indexTouch++) {
            mTouchs[indexTouch] = new Touch();
        }
    }

    public void setRenderer() {
        mRenderer = new MainRenderer();

        setPreserveEGLContextOnPause(true);
        setRenderer(mRenderer);
    }

    protected void onSizeChanged(final int pNewSurfaceWidth, final int pNewSurfaceHeight, final int pOldSurfaceWidth, final int pOldSurfaceHeight) {
        mRenderer.setScreenWidthAndHeight(pNewSurfaceWidth, pNewSurfaceHeight);
    }

    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getAction() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_DOWN:
            {
                final int id = event.getPointerId(0);
                final float x = event.getX(0);
                final float y = event.getY(0);

                if (id < mMaxTouchs) {
                    this.queueEvent(new Runnable() {
                        public void run() {
                            mRenderer.handleTouchBegin(id, x, y);
                        }
                    });

                    mTouchs[id].x = x;
                    mTouchs[id].y = y;
                }
            }
            break;

            case MotionEvent.ACTION_UP:
            {
                final int id = event.getPointerId(0);
                final float x = event.getX(0);
                final float y = event.getY(0);

                if (id < mMaxTouchs) {
                    this.queueEvent(new Runnable() {
                        public void run() {
                            mRenderer.handleTouchEnd(id, x, y);
                        }
                    });

                    mTouchs[id].x = 0.0f;
                    mTouchs[id].y = 0.0f;
                }
            }
            break;

            case MotionEvent.ACTION_POINTER_DOWN:
            {
                final int index = event.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
                final int id = event.getPointerId(index);
                final float x = event.getX(index);
                final float y = event.getY(index);

                if (id < mMaxTouchs) {
                    this.queueEvent(new Runnable() {
                        public void run() {
                            mRenderer.handleTouchBegin(id, x, y);
                        }
                    });

                    mTouchs[id].x = x;
                    mTouchs[id].y = y;
                }
            }
            break;

            case MotionEvent.ACTION_POINTER_UP:
            {
                final int index = event.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
                final int id = event.getPointerId(index);
                final float x = event.getX(index);
                final float y = event.getY(index);

                if (id < mMaxTouchs) {
                    this.queueEvent(new Runnable() {
                        public void run() {
                            mRenderer.handleTouchEnd(id, x, y);
                        }
                    });

                    mTouchs[id].x = 0.0f;
                    mTouchs[id].y = 0.0f;
                }
            }
            break;

            case MotionEvent.ACTION_MOVE:
            {
                for (int indexTouch = 0; indexTouch < event.getPointerCount(); indexTouch++) {
                    final int id = event.getPointerId(indexTouch);
                    final float x = event.getX(indexTouch);
                    final float y = event.getY(indexTouch);

                    if (id < mMaxTouchs) {
                        if (mTouchs[id].x != x || mTouchs[id].y != y) {
                            this.queueEvent(new Runnable() {
                                public void run() {
                                    mRenderer.handleTouchMove(id, x, y);
                                }
                            });

                            mTouchs[id].x = x;
                            mTouchs[id].y = y;
                        }
                    }
                }
            }
            break;

            case MotionEvent.ACTION_CANCEL:
            {
                for (int indexTouch = 0; indexTouch < event.getPointerCount(); indexTouch++) {
                    final int id = event.getPointerId(indexTouch);
                    final float x = event.getX(indexTouch);
                    final float y = event.getY(indexTouch);

                    if (id < mMaxTouchs) {
                        this.queueEvent(new Runnable() {
                            public void run() {
                                mRenderer.handleTouchCancel(id, x, y);
                            }
                        });

                        mTouchs[id].x = 0.0f;
                        mTouchs[id].y = 0.0f;
                    }
                }
            }
            break;
        }

        return true;
    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_VOLUME_UP:
            case KeyEvent.KEYCODE_VOLUME_DOWN:
            case KeyEvent.KEYCODE_VOLUME_MUTE:
                return super.onKeyDown(keyCode, event);

            case KeyEvent.KEYCODE_DPAD_UP:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetDPadValueY(-1);
                    }
                });
                break;

            case KeyEvent.KEYCODE_DPAD_DOWN:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetDPadValueY(1);
                    }
                });
                break;

            case KeyEvent.KEYCODE_DPAD_LEFT:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetDPadValueX(-1);
                    }
                });
                break;

            case KeyEvent.KEYCODE_DPAD_RIGHT:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetDPadValueX(1);
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_A:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetActionAButtonPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_B:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetActionBButtonPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_X:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetActionXButtonPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_Y:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetActionYButtonPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_L1:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetLeft1ButtonPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_L2:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetLeft2Value(1.0f);
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_R1:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetRight1ButtonPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_R2:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetRight2Value(1.0f);
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_THUMBL:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetLeftStickPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_THUMBR:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetRightStickPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_ENTER:
            case KeyEvent.KEYCODE_BUTTON_START:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetStartButtonPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_SELECT:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetSelectButtonPassed();
                    }
                });
                break;

            case KeyEvent.KEYCODE_MENU:
                this.queueEvent(new Runnable() {
                    public void run() {
                    }
                });
                break;
        }

        return true;
//		return super.onKeyDown(keyCode, event);
    }

    public boolean onKeyUp(int keyCode, KeyEvent event) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_VOLUME_UP:
            case KeyEvent.KEYCODE_VOLUME_DOWN:
            case KeyEvent.KEYCODE_VOLUME_MUTE:
                return super.onKeyUp(keyCode, event);

            case KeyEvent.KEYCODE_DPAD_UP:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetDPadValueY(0);
                    }
                });
                break;

            case KeyEvent.KEYCODE_DPAD_DOWN:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetDPadValueY(0);
                    }
                });
                break;

            case KeyEvent.KEYCODE_DPAD_LEFT:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetDPadValueX(0);
                    }
                });
                break;

            case KeyEvent.KEYCODE_DPAD_RIGHT:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetDPadValueX(0);
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_A:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetActionAButtonReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_B:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetActionBButtonReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_X:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetActionXButtonReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_Y:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetActionYButtonReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_L1:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetLeft1ButtonReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_L2:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetLeft2Value(0.0f);
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_R1:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetRight1ButtonReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_R2:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetRight2Value(0.0f);
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_THUMBL:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetLeftStickReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_THUMBR:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetRightStickReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_ENTER:
            case KeyEvent.KEYCODE_BUTTON_START:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetStartButtonReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_BUTTON_SELECT:
                this.queueEvent(new Runnable() {
                    public void run() {
                        mRenderer.handleSetSelectButtonReleased();
                    }
                });
                break;

            case KeyEvent.KEYCODE_MENU:
                this.queueEvent(new Runnable() {
                    public void run() {
                    }
                });
                break;
        }

        return true;
//		return super.onKeyUp(keyCode, event);
    }

    public boolean onGenericMotionEvent(MotionEvent event) {
        if ((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0 && event.getAction() == MotionEvent.ACTION_MOVE) {
            if (mInputDevice == null || mInputDevice.getId() != event.getDeviceId()) {
                mInputDevice = event.getDevice();

                if (mInputDevice == null) {
                    return false;
                }
            }

            final float dPadAxisX = getCenteredAxis(event, mInputDevice, MotionEvent.AXIS_HAT_X);
            final float dPadAxisY = getCenteredAxis(event, mInputDevice, MotionEvent.AXIS_HAT_Y);
            final float leftStickAxisX = getCenteredAxis(event, mInputDevice, MotionEvent.AXIS_X);
            final float leftStickAxisY = getCenteredAxis(event, mInputDevice, MotionEvent.AXIS_Y);
            final float rightStickAxisX = getCenteredAxis(event, mInputDevice, MotionEvent.AXIS_Z);
            final float rightStickAxisY = getCenteredAxis(event, mInputDevice, MotionEvent.AXIS_RZ);
            final float left2Axis2 = getCenteredAxis(event, mInputDevice, MotionEvent.AXIS_BRAKE);
            final float right2Axis2 = getCenteredAxis(event, mInputDevice, MotionEvent.AXIS_GAS);

            this.queueEvent(new Runnable() {
                public void run() {
                    mRenderer.handleSetDPadValueX(dPadAxisX);
                    mRenderer.handleSetDPadValueY(dPadAxisY);
                    mRenderer.handleSetLeftStickValueX(leftStickAxisX);
                    mRenderer.handleSetLeftStickValueY(leftStickAxisY);
                    mRenderer.handleSetRightStickValueX(rightStickAxisX);
                    mRenderer.handleSetRightStickValueY(rightStickAxisY);
                    mRenderer.handleSetLeft2Value(left2Axis2);
                    mRenderer.handleSetRight2Value(right2Axis2);
                }
            });

            return true;
        }

        return super.onGenericMotionEvent(event);
    }

    private float getCenteredAxis(MotionEvent event, InputDevice device, int axis) {
        final InputDevice.MotionRange range = device.getMotionRange(axis, event.getSource());
        if (range != null) {
            final float flat = range.getFlat();
            final float value = event.getAxisValue(axis);

            // Ignore axis values that are within the 'flat' region of the joystick axis center.
            // A joystick at rest does not always report an absolute position of (0,0).
            if (Math.abs(value) > flat) {
                return value;
            }
        }
        return 0;
    }

    public Vibrator getVibrator() {
        if (mInputDevice != null) {
            Vibrator vibrator = mInputDevice.getVibrator();
            if (vibrator.hasVibrator()) {
                return vibrator;
            }
        }

        return (Vibrator)getContext().getSystemService(Context.VIBRATOR_SERVICE);
    }

    public void onPause(){
        super.onPause();

        this.queueEvent(new Runnable() {
            public void run() {
                mRenderer.handleOnPause();
            }
        });
    }

    public void onResume(){
        super.onResume();

        this.queueEvent(new Runnable() {
            public void run() {
                mRenderer.handleOnResume();
            }
        });
    }
}
