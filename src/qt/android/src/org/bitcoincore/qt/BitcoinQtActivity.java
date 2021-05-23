package org.thecoffeecoinscore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class thecoffeecoinsQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File thecoffeecoinsDir = new File(getFilesDir().getAbsolutePath() + "/.thecoffeecoins");
        if (!thecoffeecoinsDir.exists()) {
            thecoffeecoinsDir.mkdir();
        }

        try {
            Os.setenv("QT_QPA_PLATFORM", "android", true);
        } catch (ErrnoException e) {
            e.printStackTrace();
        }

        super.onCreate(savedInstanceState);
    }
}
