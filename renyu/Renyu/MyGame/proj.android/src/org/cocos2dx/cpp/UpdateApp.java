package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import android.app.ProgressDialog;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Environment;
import android.widget.Toast;

public class UpdateApp extends AsyncTask<String, Integer, String> {

	final static String TAG = "UpdateApp";
	String fileUrl = "";
	AppActivity mContext;
	String appName;
	String apkName;
	ProgressDialog mProgressDialog;
	static AppActivity appactivity;
	
	public UpdateApp(AppActivity mContext, String appName, String apkName){
		this.mContext = mContext;
		this.appName = appName;
		this.apkName = apkName;
	}
	
	@Override
	protected void onPreExecute() {
		super.onPreExecute();
		mProgressDialog = new ProgressDialog(mContext);//实例化
		mProgressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);//设置进度条风格，风格为长形，有刻度的
		//mProgressDialog.setTitle(appName);//设置ProgressDialog 标题
		//mProgressDialog.setMessage("更新");//设置ProgressDialog 提示信息
		mProgressDialog.setProgress(0);//设置ProgressDialog 进度条进度
		mProgressDialog.setIndeterminate(false);//设置ProgressDialog 的进度条是否不明确
		mProgressDialog.setCancelable(false);//设置ProgressDialog 是否可以按退回按键取消
		mProgressDialog.show();//让ProgressDialog显示
	}
	
	/** 获取SDCARD路径 **/
	public static String getSdCardPath() {
		if (Environment.getExternalStorageState().equals(
				android.os.Environment.MEDIA_MOUNTED)
				&& Environment.getExternalStorageDirectory().canWrite())
			return Environment.getExternalStorageDirectory().getPath();
		else
			return Environment.getExternalStorageDirectory().getPath();
	}
	/** 安装APK **/
	public  void openFile(File file) {
		if (file == null)
			return;
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(android.content.Intent.ACTION_VIEW);
		
		intent.setDataAndType(Uri.fromFile(file), "application/vnd.android.package-archive");
		mContext.startActivity(intent);
	}
	
	@Override
	protected String doInBackground(String... arg0) {
		// sdcard判断
		String sdcardpath = getSdCardPath();
		if (sdcardpath == null) {
			return "error";
		}
		// 创建文件夹
		sdcardpath += "/" + appName + "/" + "download";
		File tmpFile = new File(sdcardpath);
		if (!tmpFile.exists()) {
			try {
				tmpFile.mkdirs();
			} catch (Exception e) {
				return "error";
			}
		}
		fileUrl = sdcardpath + "/" + apkName;
		File file = new File(fileUrl);
		try {
			URL url = new URL(arg0[0]);
			try {
				HttpURLConnection conn = (HttpURLConnection) url.openConnection();
				InputStream is = conn.getInputStream();
				FileOutputStream fileOutput = new FileOutputStream(file);
				byte[] buf = new byte[256];// 分配byte
				conn.connect();
				double count = 0;
				double downSize = 0;
				double totalSize = conn.getContentLength();
				if (conn.getResponseCode() >= 400) {
					conn.disconnect();
					fileOutput.close();
					is.close();
					return "error";
				} else {
					while (count <= 100) {
						if (is != null) {
							int numRead = is.read(buf);
							if (numRead <= 0) {
								break;
							} else {
								fileOutput.write(buf, 0, numRead);
								downSize += numRead;
								publishProgress((int)((downSize / totalSize) * 100));
							}
						} else {
							break;
						}
					}
				}
				conn.disconnect();// 需要记得关闭连接
				fileOutput.close();
				is.close();
			} catch (IOException e) {
				e.printStackTrace();
				return "error";
			}
		} catch (MalformedURLException e) {
			e.printStackTrace();
			return "error";
		}
		return "success";
	}

	@Override
	protected void onProgressUpdate(Integer... values) {
		super.onProgressUpdate(values);
		if(mProgressDialog != null){
			mProgressDialog.setProgress(values[0]);
		}
	}

	@Override
	protected void onPostExecute(String result) {
		super.onPostExecute(result);
		if(result.equals("error")){
			Toast.makeText(mContext, "下载失败!", Toast.LENGTH_SHORT).show();
			
		}
		if(result.equals("success")){
			if(mProgressDialog != null){
				mProgressDialog.dismiss();
			}
			Toast.makeText(mContext, "下载成功!", Toast.LENGTH_SHORT).show();
			this.openFile(new File(fileUrl));
		}
	}

}
