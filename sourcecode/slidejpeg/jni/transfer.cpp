#include "server.cpp"
#include "simple_jpeg_httpd.cpp"


#include <string.h>
#include <jni.h>
#include <stdlib.h>
#include <pthread.h>
#include <android/log.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_DEBUG, "transfer", __VA_ARGS__)

int ss;
int s;

extern"C" {  
JNIEXPORT void JNICALL Java_com_example_slidejpeg_MainActivity_transfer(JNIEnv *env, jobject pj, jstring jpegPath);
}  

static char lastPath[256];
void Java_com_example_slidejpeg_MainActivity_transfer( JNIEnv* env, jobject thiz, jstring jpegPath)
{
    const char* path = env->GetStringUTFChars(jpegPath, false);  
    
    if(s && ss && memcmp(lastPath, path, strlen(path)))
    {
        //LOGE("send filepath %s to remote", path);
        
        fileNameServerSend(s, (char*)path, strlen(path));
        
        //LOGE("send filepath %s to remote after", path);
        memcpy(lastPath, path, strlen(path));
    }
    
    env->ReleaseStringUTFChars(jpegPath, path);  
}

void* syntax_wrapper(void*)
{
    s = fileNameServerInit(&ss);
}

JNIEXPORT int JNI_OnLoad(JavaVM* vm, void*) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        abort();
    }
    
    {
        pthread_t tid;
        pthread_create(&tid, NULL, syntax_wrapper, NULL);
        pthread_detach(tid);
    }
    
    {
        pthread_t tid;
        pthread_create(&tid, NULL, simple_jpeg_httpd, NULL);
        pthread_detach(tid);
    }
    
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
 {
     if(s && ss)
        fileNameServerUnit(s, ss);
 }