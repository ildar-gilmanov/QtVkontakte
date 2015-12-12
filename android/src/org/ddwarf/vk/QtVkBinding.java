package org.ddwarf.vk;

import android.util.Log;
import android.app.Fragment;
import android.app.FragmentManager;
import org.qtproject.qt5.android.bindings.QtActivity;
import java.util.ArrayList;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
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
    static public void onCreate(QtActivity activity) {
        m_activity = activity;
    }

    /**
     * Show dialog for sharing
     * @param textToPost Post text. User can change that text
     * @param photoLinks Array of already uploaded photos from VK, that will be attached to post
     * @param photos Images that will be uploaded with post
     * @param linkTitle A small description for your link
     * @param linkUrl Url that link follows
     */
    public void openShareDialog(java.lang.String textToPost,
                                java.lang.String[] photoLinks,
                                byte[][] photos,
                                java.lang.String linkTitle,
                                java.lang.String linkUrl) {
        if(m_activity == null) {
            Log.e("vk", "You must call QtVkBinding.onCreate() method inside the onCreate() of main Activity");
            return;
        }

        VKShareDialog shareDialog = new VKShareDialog();

        shareDialog.setText(textToPost);

        if(photoLinks != null) {
            VKPhotoArray photoArray = new VKPhotoArray();

            for(java.lang.String photoLink : photoLinks) {
                VKApiPhoto photo = new VKApiPhoto(photoLink);
                photoArray.add(photo);
            }

            shareDialog.setUploadedPhotos(photoArray);
        }

        if(photos != null) {
            ArrayList<VKUploadImage> photoArray = new ArrayList<VKUploadImage>();

            for(byte[] img : photos) {
                Bitmap bitmap = BitmapFactory.decodeByteArray(img, 0, img.length);
                VKUploadImage photo = new VKUploadImage(bitmap, VKImageParameters.pngImage());
                photoArray.add(photo);
            }

            shareDialog.setAttachmentImages(photoArray.toArray(new VKUploadImage[photoArray.size()]));
        }

        if(linkTitle != null && linkUrl != null) {
            shareDialog.setAttachmentLink(linkTitle, linkUrl);
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
    }

    // Emit signal for operation done
    private static native void operationComplete(String operation, String[] data);

    // Emit signal for operation cancel
    private static native void operationCancel(String operation);

    // Emit signal for operation error
    private static native void operationError(String operation, String error);

    private static QtActivity m_activity = null;
    private static String m_openShareDialogOperationName = "openShareDialog";
}
