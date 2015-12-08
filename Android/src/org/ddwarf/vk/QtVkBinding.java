package org.ddwarf.vk;

import android.util.Log;
import android.app.Fragment;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import org.qtproject.qt5.android.bindings.QtActivity;
import com.vk.sdk.dialogs.VKShareDialog;
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
     * @param photos Array of already uploaded photos from VK, that will be attached to post
     * @param attachmentImages Images that will be uploaded with post
     * @param linkTitle A small description for your link
     * @param linkUrl Url that link follows
     */
    public void openShareDialog(java.lang.CharSequence textToPost,
                                com.vk.sdk.api.model.VKPhotoArray photos,
                                com.vk.sdk.api.photo.VKUploadImage[] attachmentImages,
                                java.lang.String linkTitle,
                                java.lang.String linkUrl) {
        if(m_activity == null) {
            Log.e("vk", "You must call QtVkBinding.onCreate() method inside the onCreate() of main Activity");
            return;
        }

        VKShareDialog shareDialog = new VKShareDialog();

        shareDialog.setText(textToPost);

        if(photos != null) {
            shareDialog.setUploadedPhotos(photos);
        }

        if(attachmentImages != null) {
            shareDialog.setAttachmentImages(attachmentImages);
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

        FragmentTransaction ft = m_activity.getFragmentManager().beginTransaction();
        ft.add(shareDialog, "VK_SHARE_DIALOG");
        ft.commit();
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
