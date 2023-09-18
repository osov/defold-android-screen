#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>
#include "extension_private.h"
#include "com_defold_extension_ExtensionAndroidScreen.h"
#include "extension_callback_private.h"

JNIEXPORT void JNICALL Java_com_defold_extension_ExtensionAndroidScreen_AddToQueue(JNIEnv * env, jclass cls, jint jmsg, jstring jjson)
{
    const char* json = env->GetStringUTFChars(jjson, 0);
    dmAndroidScreen::AddToQueueCallback((dmAndroidScreen::MessageId)jmsg, json);
    env->ReleaseStringUTFChars(jjson, json);
}

namespace dmAndroidScreen {

struct App
{
    jobject    m_AppJNI;
    jmethodID  m_SetMode;
};

static App       g_app;

static void CallVoidMethod(jobject instance, jmethodID method)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    env->CallVoidMethod(instance, method);
}


static void CallVoidMethodChar(jobject instance, jmethodID method, const char* cstr)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jstring jstr = env->NewStringUTF(cstr);
    env->CallVoidMethod(instance, method, jstr);
    env->DeleteLocalRef(jstr);
}


static void CallVoidMethodCharChar(jobject instance, jmethodID method, const char* cstr1, const char* cstr2)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jstring jstr1 = env->NewStringUTF(cstr1);
    jstring jstr2 = env->NewStringUTF(cstr2);
    env->CallVoidMethod(instance, method, jstr1, jstr2);
    env->DeleteLocalRef(jstr1);
    env->DeleteLocalRef(jstr2);
}

static void InitJNIMethods(JNIEnv* env, jclass cls)
{
    g_app.m_SetMode = env->GetMethodID(cls, "SetMode", "(Ljava/lang/String;)V");
}

void Initialize_Ext()
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();
    jclass cls = dmAndroid::LoadClass(env, "com.defold.extension.ExtensionAndroidScreen");

    InitJNIMethods(env, cls);

    jmethodID jni_constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");

    g_app.m_AppJNI = env->NewGlobalRef(env->NewObject(cls, jni_constructor, threadAttacher.GetActivity()->clazz));
}

void SetMode(const char* unitId)
{
     CallVoidMethodChar(g_app.m_AppJNI, g_app.m_SetMode, unitId);
}



void ActivateApp()
{
    //
}

}

#endif
