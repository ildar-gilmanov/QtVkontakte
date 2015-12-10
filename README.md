# QtVkontakte
Qt wrapper around **Vkontakte Android SDK**

See [Vk Android SDK Overview](https://vk.com/dev/android_sdk) and [Vk Android SDK Source](https://github.com/VKCOM/vk-android-sdk) for instructions how to use **Vkontakte Android SDK**

Before using this library you have to inherit `QtActivity` from `android.support.v4.app.FragmentActivity` instead `android.app.Activity`. It is sad, but we have to do it because vk-android-sdk uses android support library.  
Just navigate to /path-to-qt/android_armv[5,7]/src/android/java/src/org/qtproject/qt5/android/bindings/ and execute in command line:  
`$ patch QtActivity.java /path-to-qtvkontakte/patches/QtActivity.patch`  

In java you have to call `QtVkBinding.onCreate(this)` method inside the onCreate of main Activity.  
In c++ you have to call `QtVk::initStatic()`.

Using:
------

In Qml:  
`QtVk {  
id: qtVk  
}`