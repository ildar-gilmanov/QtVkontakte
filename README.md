# QtVkontakte
Qt wrapper around Vkontakte Android SDK

see https://vk.com/dev/android_sdk and https://github.com/VKCOM/vk-android-sdk

Before using this library you have to inherit QtActivity from android.support.v4.app.FragmentActivity instead android.app.Activity. It is sad, but we have to do it because vk-android-sdk uses android support library.
Just navigate to /path-to-qt/android_armv[5,7]/src/android/java/src/org/qtproject/qt5/android/bindings/ and execute in command line:
patch QtActivity.java /path-to-qtvkontakte/patches/QtActivity.patch
