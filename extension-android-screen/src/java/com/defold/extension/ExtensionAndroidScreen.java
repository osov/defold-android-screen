package com.defold.extension;

import android.util.Log;
import android.app.Activity;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;
import android.content.pm.ActivityInfo;

public class ExtensionAndroidScreen {

  private static String TAG = "ExtensionAndroidScreen";
  public static native void AddToQueue(int msg, String json);
  private static final int MSG_TYPE_NONE = 1;
  private static final int EVENT_JSON_ERROR = 101;
  private Activity activity;


  public ExtensionLogCat(Activity mainActivity) {
    activity = mainActivity;
  }

  public void SetMode(final String key) {
    activity.runOnUiThread(new Runnable() {
      @Override
      public void run() {
        Log.d(TAG, "SetMode:"+key);
        if (key.equals("portrait"))
        	activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
     	else
     		activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
      }
    });
  }


  private String getJsonConversionErrorMessage(String messageText) {
    String message = null;
    try {
      JSONObject obj = new JSONObject();
      obj.put("error", messageText);
      obj.put("event", EVENT_JSON_ERROR);
      message = obj.toString();
    } catch (JSONException e) {
      message = "{ \"error\": \"Error while converting simple message to JSON.\", \"event\": " + EVENT_JSON_ERROR
          + " }";
    }
    return message;
  }


  private void sendSimpleMessage(int msg, String key, String value) {
    String message = null;
    try {
      JSONObject obj = new JSONObject();
      obj.put(key, value);
      message = obj.toString();
    } catch (JSONException e) {
      message = getJsonConversionErrorMessage(e.getLocalizedMessage());
    }
    AddToQueue(msg, message);
  }


}
