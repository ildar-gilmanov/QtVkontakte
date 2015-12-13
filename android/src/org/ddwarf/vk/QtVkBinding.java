package org.ddwarf.vk;

import java.lang.String;
import android.util.Log;
import android.app.Fragment;
import android.app.FragmentManager;
import android.content.Intent;
import org.qtproject.qt5.android.bindings.QtActivity;
import java.util.ArrayList;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import com.vk.sdk.VKSdk;
import com.vk.sdk.VKScope;
import com.vk.sdk.VKCallback;
import com.vk.sdk.VKAccessToken;
import com.vk.sdk.dialogs.VKShareDialog;
import com.vk.sdk.api.model.VKPhotoArray;
import com.vk.sdk.api.model.VKApiPhoto;
import com.vk.sdk.api.photo.VKUploadImage;
import com.vk.sdk.api.photo.VKImageParameters;
import com.vk.sdk.api.VKError;

public class QtVkBinding
{

    /**
     * This has to be called inside the onCreate of main Activity
     */
    public static void onCreate(QtActivity activity) {
        m_activity = activity;
    }

    /**
     * This has to be called inside the onActivityResult of main Activity
     */
    public static void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (VKSdk.onActivityResult(requestCode, resultCode, data, new VKCallback<VKAccessToken>() {
            @Override
            public void onResult(VKAccessToken res) {
                // User passed Authorization
                operationComplete(m_loginOperationName, null);

                if(m_currentOperation == SHARE_DATA_OPERATION) {
                    openShareDialogNow();
                }
            }

            @Override
            public void onError(VKError error) {
                // User didn't pass Authorization
                operationError(m_loginOperationName, error.errorMessage);

                if(m_currentOperation == SHARE_DATA_OPERATION) {
                    m_shareData = null;
                }

                m_currentOperation = 0;
            }
        })) {
        }
    }

    /**
     * Show dialog for sharing
     * @param textToPost Post text. User can change that text
     * @param photoLinks Array of already uploaded photos from VK, that will be attached to post
     * @param photos Images that will be uploaded with post
     * @param linkTitle A small description for your link
     * @param linkUrl Url that link follows
     */
    public static void openShareDialog(java.lang.String textToPost,
                                       java.lang.String[] photoLinks,
                                       byte[][] photos,
                                       java.lang.String linkTitle,
                                       java.lang.String linkUrl) {
        if(m_activity == null) {
            Log.e("vk", "You must call QtVkBinding.onCreate() method inside the onCreate() of main Activity");
            return;
        }

        if(m_currentOperation != 0) {
            Log.i("vk", "Previous task is not completed");
            return;
        }

        if(m_shareData != null) {
            Log.i("vk", "Previous task for share is not completed");
            return;
        } else {
            m_shareData = new ShareData();
        }

        m_shareData.textToPost = textToPost;

        if(photoLinks != null) {
            VKPhotoArray photoArray = new VKPhotoArray();

            for(String photoLink : photoLinks) {
                VKApiPhoto photo = new VKApiPhoto(photoLink);
                photoArray.add(photo);
            }

            m_shareData.photoLinks = photoArray;
        }

        if(photos != null) {
            ArrayList<VKUploadImage> photoArray = new ArrayList<VKUploadImage>();

            for(byte[] img : photos) {
                Bitmap bitmap = BitmapFactory.decodeByteArray(img, 0, img.length);
                VKUploadImage photo = new VKUploadImage(bitmap, VKImageParameters.pngImage());
                photoArray.add(photo);
            }

            m_shareData.photos = photoArray.toArray(new VKUploadImage[photoArray.size()]);
        }

        m_shareData.linkTitle = linkTitle;
        m_shareData.linkUrl = linkUrl;

        m_currentOperation = SHARE_DATA_OPERATION;

        if(VKSdk.isLoggedIn()) {
            openShareDialogNow();
        } else {
            VKSdk.login(m_activity, m_scope);
        }
    }

    private static void openShareDialogNow() {
        if(m_shareData == null) {
            Log.e("vk", "Data for share dialog is null");
            return;
        }

        VKShareDialog shareDialog = new VKShareDialog();

        shareDialog.setText(m_shareData.textToPost);

        if(m_shareData.photoLinks != null) {
            shareDialog.setUploadedPhotos(m_shareData.photoLinks);
        }

        if(m_shareData.photos != null) {
            shareDialog.setAttachmentImages(m_shareData.photos);
        }

        if(m_shareData.linkTitle != null && m_shareData.linkUrl != null) {
            shareDialog.setAttachmentLink(m_shareData.linkTitle, m_shareData.linkUrl);
        }

        shareDialog.setShareDialogListener(new VKShareDialog.VKShareDialogListener() {
            public void onVkShareComplete(int postId) {
                String[] data = new String[2];
                data[0] = "postId";
                data[1] = Integer.toString(postId);
                operationComplete(m_openShareDialogOperationName, data);
            }

            public void onVkShareCancel() {
                operationCancel(m_openShareDialogOperationName);
            }

            public void onVkShareError(VKError error) {
                operationError(m_openShareDialogOperationName, error.errorMessage);
            }
        });

        shareDialog.show(m_activity.getSupportFragmentManager(), "VK_SHARE_DIALOG");
        m_shareData = null;
        m_currentOperation = 0;
    }

    private static class ShareData {
        public ShareData() {
        }

        public String textToPost;
        public VKPhotoArray photoLinks;
        public VKUploadImage[] photos;
        public String linkTitle;
        public String linkUrl;
    }

    // Emit signal for operation done
    private static native void operationComplete(String operation, String[] data);

    // Emit signal for operation cancel
    private static native void operationCancel(String operation);

    // Emit signal for operation error
    private static native void operationError(String operation, String error);

    private static QtActivity m_activity = null;
    private static String m_loginOperationName = "login";
    private static String m_openShareDialogOperationName = "openShareDialog";
    private static final String[] m_scope = new String[] {
            VKScope.FRIENDS,
            VKScope.WALL,
            VKScope.PHOTOS,
            VKScope.NOHTTPS
    };

    private static ShareData m_shareData;

    private static final int SHARE_DATA_OPERATION = 1;
    private static int m_currentOperation = 0;
}
