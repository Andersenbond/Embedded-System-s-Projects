package android.uminho.balancerobot;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;


public class Utils {
    public static void log(String message) {
        if (BuildConfig.DEBUG) {
            if (message != null) Log.i(Const.TAG, message);
        }
    }
    // ============================================================================
    public static byte[] concat(byte[] A, byte[] B) {
        byte[] C = new byte[A.length + B.length];
        System.arraycopy(A, 0, C, 0, A.length);
        System.arraycopy(B, 0, C, A.length, B.length);
        return C;
    }
    // ============================================================================
    public static String getPrefence(Context context, String item) {
        final SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(context);
        return settings.getString(item, Const.TAG);
    }
    // ============================================================================
}
