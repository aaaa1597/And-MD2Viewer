package com.tks.md2viewer;

import android.view.Surface;

public class NativeLibIf {
    static { System.loadLibrary("md2viewer"); }

    public native static void surfaceCreated(int id, Surface surface);
    public native static void surfaceChanged(int id, int width, int height);
    public native static void surfaceDestroyed(int id);
}
