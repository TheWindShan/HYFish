package org.cocos2dx.cpp;

import java.io.File;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Environment;
import android.util.Log;



/**
 * 宸ュ�风被
 * 
 * @author hukl
 * 
 */
public class Tools {

	/**
	 * 浠ユ��������瀛������瑰��璇诲�������拌��婧������剧��
	 * 
	 * @param context
	 * @param resId
	 * @return
	 */
	public final static Bitmap readBitMap(int resId) {
		BitmapFactory.Options opt = new BitmapFactory.Options();
		opt.inPreferredConfig = Bitmap.Config.RGB_565;
		opt.inPurgeable = true;
		opt.inInputShareable = true;
		// ��峰��璧�婧���剧��
		InputStream is = AppActivity.getInstance().getResources()
				.openRawResource(resId);
		return BitmapFactory.decodeStream(is, null, opt);

		// return
		// BitmapFactory.decodeResource(ClientApp.getInstance().getResources(),
		// resId);
	}

	/**
	 * dip杞����px
	 * 
	 * @param dipValue
	 * @return
	 */
	public static int dip2px(float dipValue) {
		final float scale = AppActivity.getInstance().getResources()
				.getDisplayMetrics().density;
		return (int) (dipValue * scale + 0.5f);
	}
	
	/** SD��℃�����瀛���� **/
	public final static boolean ifSDExist() {
		return Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED);
	}

	/** ��峰��SD��¤矾寰� **/
	public final static String getSDPath() {

		File sdDir = null;
		try {
			boolean sdCardExist = ifSDExist(); // ��ゆ��SD��℃�����瀛����
			if (sdCardExist) {
				sdDir = Environment.getExternalStorageDirectory();// ��峰����圭��褰�
				return sdDir.toString();
			} else {
				return "sdcard";
			}
		} catch (Exception e) {
			return "sdcard";
		} finally {
			// return "sdcard";
		}
	}
	
	/**
	 * ��峰��褰����绯荤����堕��
	 * 
	 * @return 杩���������堕�村��绗�涓叉�煎�� 12:12
	 */
	@SuppressLint("SimpleDateFormat")
	public static String getStringDateShort() {
		Date currentTime = new Date();
		SimpleDateFormat formatter = new SimpleDateFormat("HH:mm");
		String dateString = formatter.format(currentTime);
		return dateString; 
	}
	
	/**��峰��杩���ュ��缃�缁�绫诲��**/
	public static String getNetworkType(Context context){   
        ConnectivityManager connManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);   
        NetworkInfo networkinfo = connManager.getActiveNetworkInfo();   
        String networkType = "";   
        if(networkinfo != null){   
            networkType = networkinfo.getTypeName();               
        }   
        return networkType;   
    }  

	//RGBA��版��杞����涓哄��绱���扮��
	public static int[] convertByteToColor(byte[] data)
	{
		int size = data.length;
		if(size%4 != 0)
		{
			Log.v("Head","澶村����版��涓�姝ｇ‘");
			return null;
		}
		int []color = new int[size/4];
		for(int i =0; i < color.length; ++i)
		{
			color[i] = (data[i * 4] << 16 & 0x00FF0000) |   
	                   (data[i * 4 + 1] << 8 & 0x0000FF00 ) |   
	                   (data[i * 4 + 2] & 0x000000FF ) |   
	                    0xFF000000;
		}
		return color;
	}

}
