/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;


import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Toast;
import android.text.TextUtils;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;
import java.io.BufferedOutputStream;

import java.io.File;
import java.io.FileOutputStream;
import java.util.*;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxBitmap;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Toast;
import android.text.TextUtils;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences.Editor;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.WindowManager;

import com.hongyu.renyu.R;
import com.sina.weibo.sdk.utils.LogUtil;
import com.alipay.sdk.app.*;
import com.junnet.heepay.service.*;
@SuppressLint("NewApi")
public class AppActivity extends Cocos2dxActivity {
	static AppActivity appactivity;
	private static final int	RESULT_LOAD_IMAGE	= 3000;
	public static final int RESULT_CLIP_IMAGE = 8;
	// 保存拍照临时文件的文件路径
	public String LOCAL_PATH;
	private Editor editor;
	
	//更新模块
	public static String URL;//
	private static String version;
	private static AsyncTask<String, Integer, String> execute;
	public static final String PARTNER = "2088711585855270";
	// 鍟嗘埛鏀舵璐﹀彿
	public static final String SELLER = "3070356366@qq.com";
	// 鍟嗘埛绉侀挜锛宲kcs8鏍煎紡
	public static final String RSA_PRIVATE = "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAKFey4phCBIOGQU2e4M9VJ8OXnoO2AwnEZcK2KE/N+fwCQIcNaztKsTXGy7tQmtFJQZ3Buw7N9D6QqgQI6SSgv+DdI0WY9rJ9q0U17hjOXd75fUm9USIAfATqoYYZRGbKfRBH/TSkYyf0x4ktQYtdfN1KRsv1QgLylVlbjmK0gaTAgMBAAECgYEAgtzYHUkg/VtXaQXM1ho3dKF3dm5xQDGyrxWJpgIZeUsMUxzDXpAanfR/Q+f+c/Vn2D1QbOR9GEjLI3WrIfv6pKdkZvgB1Iwz1KpblFUFPFKizpAOuqByw9LBf2RCEwDSpCZAJl+t7PXp+DQ3dGH4jkzedMEW7Jn7aN7dTAoSWQECQQDUO4uOuSWWC96Jxqfh/1gs+edGWdZKp5mcyGmMJq/e6pUvWGXZ2nb3BoMue3Pk1ot8fGjE12f9G/yevGgZzmoTAkEAwqYP4w63iyrclByKJqJbWIppS+8v6fKWNm1+0S9I2iM+/C3ursj4okmHehKhGSOAZT4FwY41acgbntC1e5mBgQJBALQRtY4p7wcJGQ7C26x3jyPSAPWCwbvjygTpUgxUwsGbPWrHUNtvbaKYBvOIUUOm+QBtOiNRNGzfsw4ngxfqN7sCQBKJP+AOiboOBd1w7lMJiQtgevCJQZXUzSs3uCjxiM2FJ71aHm20o8DIocclOQQs1lBN3Lx27204ajDwjSfNEYECQBMPQz4w5hP9n044PPt18c2yrKbC/hjaW1DFkiW38x5fsu320KzjL8lNsrcqvTI3GyZGYV8gHhgaw82qbEQdKro=";
	// 鏀粯瀹濆叕閽�
	public static final String RSA_PUBLIC = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQChXsuKYQgSDhkFNnuDPVSfDl56DtgMJxGXCtihPzfn8AkCHDWs7SrE1xsu7UJrRSUGdwbsOzfQ+kKoECOkkoL/g3SNFmPayfatFNe4Yzl3e+X1JvVEiAHwE6qGGGURmyn0QR/00pGMn9MeJLUGLXXzdSkbL9UIC8pVZW45itIGkwIDAQAB";
	private static final int SDK_PAY_FLAG = 1;

	private static final int SDK_CHECK_FLAG = 2;

	public static final int SHOW_RESPONSE=0; 
	public static String Gold_price ="1";
	public static String WebUrl ="";
	public static String orderInfo ="";
	
	public static native void UserHeadChange(final String headFile);
	public static native void UpdateConfirm();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);// 应用运行时，保持屏幕高亮，不锁屏
		SinaAccessToken.ptest = new SinaAccessToken(this);
		appactivity = this;
	}
	
	public static AppActivity getInstance()
	{
		return appactivity;
	}
	
	//电量更新
    public static String getMachineID() {
		TelephonyManager tm = (TelephonyManager) AppActivity.getInstance()
				.getSystemService(Context.TELEPHONY_SERVICE);
		final String tmDevice, tmSerial, androidId;
		tmDevice = "" + tm.getDeviceId();
		tmSerial = "" + tm.getSimSerialNumber();
		androidId = ""
				+ android.provider.Settings.Secure.getString(AppActivity.getInstance().getContentResolver(),
						android.provider.Settings.Secure.ANDROID_ID);
		UUID deviceUuid = new UUID(androidId.hashCode(),
				((long) tmDevice.hashCode() << 32) | tmSerial.hashCode());
		Log.e("111", deviceUuid.toString());
		return deviceUuid.toString();
	}
    
    public static boolean openphoto()
    {
    	appactivity.LOCAL_PATH = appactivity.getFilesDir().getPath();
    	LogUtil.e("LOCAL_PATH", appactivity.LOCAL_PATH);
    	Intent intent = new Intent(Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
    	appactivity.startActivityForResult(intent, RESULT_LOAD_IMAGE);
    	return true;
    }
    public static void openUrl(String szAccount,String szUerID)
    {
//    	Intent it = new Intent(Intent.ACTION_VIEW);
//    	it.setClassName("com.android.browser",
//				"com.android.browser.BrowserActivity");
//    	
//    	it.setData(Uri.parse("http://m.youxihongyu.com/onlinepay.aspx?"));
//    	appactivity.startActivity(it);
    	Uri uri = Uri.parse("http://m.youxihongyu.com/onlinepay.aspx");
    	Intent it = new Intent(Intent.ACTION_VIEW,uri);
    	appactivity.startActivity(it);
    	Log.v("###############appName","appName#############" + szAccount  + szUerID );
    }
    
    private Handler callBack = new Handler(){
    	public void handleMessage(Message message)
    	{
    		switch(message.what){
    		case HeepaySDKConstant.REQUEST_PAY:
    			
    		}
    	}
    };
    public static void openHuiFuBao()
    {
    	HeepayServiceHelper test = new HeepayServiceHelper(appactivity);
//    	test.isHeepayServiceInstalled(callBack);
    }
    
    
    public static  String getAppInfo() {
// 		List<AppInfo> arrayList = new ArrayList<AppInfo>();  
 		List<PackageInfo> list = appactivity.getPackageManager().getInstalledPackages(0);  
 		for (int i = 0; i < list.size(); i++) {  
 		    PackageInfo packageInfo = list.get(i);  
// 		    AppInfo tmpInfo = new AppInfo();  
 		    String appName = packageInfo.applicationInfo.loadLabel(  
 		    		appactivity.getPackageManager()).toString(); 
 		    String packageName = packageInfo.packageName;
 		    String gameName = "红宇捕鱼";
 		    if(packageName.compareTo(gameName) == 0)
 		    {
 		    	 Log.v("###############appName","appName#############" + appName );
 		 		 Log.v("###############appName","appName#############"+ packageName );
 		    }		  
 		} 
 		return null;
 	}
  
    public void startPhotoZoom(Uri uri) {

		Intent testintent = new Intent("com.android.camera.action.CROP");
		testintent.setDataAndType(uri, "image/*");
		// 下面这个crop=true是设置在开启的Intent中设置显示的VIEW可裁剪
		testintent.putExtra("crop", "true");
		// aspectX aspectY 是宽高的比例
		testintent.putExtra("aspectX", 1);
		testintent.putExtra("aspectY", 1);
		// outputX outputY 是裁剪图片宽高
		testintent.putExtra("outputX", 48);
		testintent.putExtra("outputY", 48);
		testintent.putExtra("return-data", true);
		appactivity.startActivityForResult(testintent, RESULT_CLIP_IMAGE);
	}
    
    /**
	 * 保存裁剪之后的图片数据
	 * 
	 * @param picdata
	 */
	private void setPicToView(Intent picdata) {
		Bundle extras = picdata.getExtras();
		Log.v("Head", "头像");
		if (extras != null) {
			Log.v("Head", "头像");
			final Bitmap m_UpLoadHead = extras.getParcelable("data");
			try {
				File myCaptureFile = new File(LOCAL_PATH, "head.png");
				BufferedOutputStream bos = new BufferedOutputStream(
	                                             new FileOutputStream(myCaptureFile));
				m_UpLoadHead.compress(Bitmap.CompressFormat.PNG, 100, bos);
				bos.flush();
				bos.close();
				Log.v("Head", "保存头像" + m_UpLoadHead.getByteCount());
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				Log.v("Head", "保存头像");
			}
			Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {  
			    @Override  
			      public void run() {  
			    	
			    	UserHeadChange(LOCAL_PATH+"/head.png");  
			      }  
			    });  
			this.runOnUiThread(new Runnable() {
			      @Override
			      public void run() {
			        
			      }
			    });
		}
	}
			
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, final Intent data) {
    	// TODO Auto-generated method stub
    	super.onActivityResult(requestCode, resultCode, data);
    	if (requestCode == RESULT_LOAD_IMAGE && resultCode == RESULT_OK && null != data){
			
			new Handler() {
				public void handleMessage(android.os.Message msg) {
					startPhotoZoom(data.getData());
				};
			}.sendEmptyMessageDelayed(0, 200);
			
			return;
			//setPicToView(data);
		} 
		// 系统图片获取
		if (requestCode == RESULT_CLIP_IMAGE && resultCode == RESULT_OK && null != data) {
			setPicToView(data);
		}
    }
    
    Handler m_UpdateHandler = new Handler(){
		public void handleMessage(android.os.Message msg) {
			switch (msg.what) {
			case 1:
				String appName = appactivity.getString(R.string.app_name);
				UpdateApp updateApp = new UpdateApp(appactivity, appName, appName + ".apk");
				execute = updateApp.execute(URL);
				break;
			default:
				break;
			}
		};
	};
	
	public static void SureUpdate()
	{
		
		appactivity.m_UpdateHandler.obtainMessage(1).sendToTarget();
		
	}
	public static void checkVersion(final String msg,final String downURL) 
	{
		
		PackageManager manager = appactivity.getPackageManager();
		PackageInfo info = null;
		try {
			info = manager.getPackageInfo(appactivity.getPackageName(), 0);
			version = info.versionName;
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		if (version != null)
		{
			//匹配
			if (version.equals(msg))
			{
				Log.v("version", "已经是最新版本");
			}else
			{
				URL = downURL;
				UpdateConfirm();
			}
		}
	}
}
//	
//private Handler mHandler = new Handler() {
//		public void handleMessage(Message msg) {
//				switch (msg.what) {
//				case SDK_PAY_FLAG: {
//					PayResult payResult = new PayResult((String) msg.obj);
//
//					// 鏀粯瀹濊繑鍥炴娆℃敮浠樼粨鏋滃強鍔犵锛屽缓璁鏀粯瀹濈鍚嶄俊鎭嬁绛剧害鏃舵敮浠樺疂鎻愪緵鐨勫叕閽ュ仛楠岀
//					String resultInfo = payResult.getResult();
//
//					String resultStatus = payResult.getResultStatus();
//
//					// 9000代表支付成功，具体含义参看接口文档
//					if (TextUtils.equals(resultStatus, "9000")) {
//						Toast.makeText(Recharge_Scene.this, "鏀粯鎴愬姛",
//								Toast.LENGTH_SHORT).show();
//					} else {
//						// 8000支付结果确认中
//						if (TextUtils.equals(resultStatus, "8000")) {
//							Toast.makeText(Recharge_Scene.this, "鏀粯缁撴灉纭涓�",
//									Toast.LENGTH_SHORT).show();
//
//						} else {
//							// 支付失败
//							Toast.makeText(Recharge_Scene.this, "鏀粯澶辫触",
//									Toast.LENGTH_SHORT).show();
//
//						}
//					}
//					break;
//				}
//				case SDK_CHECK_FLAG: {
//					Toast.makeText(Recharge_Scene.this, "妫�鏌ョ粨鏋滀负锛�" + msg.obj,
//							Toast.LENGTH_SHORT).show();
//					break;
//				}
//				case SHOW_RESPONSE:  
//				{
//					String response=(String) msg.obj; 
//					String _res ="true";
//					if(!response.equals(_res))
//					{
//						Toast.makeText(Recharge_Scene.this, "鍐欏叆璁㈠崟澶辫触锛佽鍙婃椂鍏徃瀹㈡埛鑱旂郴", Toast.LENGTH_SHORT).show();
//						break;
//					}
//					//鍦ㄨ繖閲岃繘琛孶I鎿嶄綔锛屽皢缁撴灉鏄剧ず鍒扮晫闈笂                 
//					//responseText.setText(response);    
//					
//					// 瀵硅鍗曞仛RSA 绛惧悕
//					String sign = sign(orderInfo);
//					try {
//						// 浠呴渶瀵箂ign 鍋歎RL缂栫爜
//						sign = URLEncoder.encode(sign, "UTF-8");
//					} catch (UnsupportedEncodingException e) {
//						e.printStackTrace();
//					}
//
//					// 瀹屾暣鐨勭鍚堟敮浠樺疂鍙傛暟瑙勮寖鐨勮鍗曚俊鎭�
//					final String payInfo = orderInfo + "&sign=\"" + sign + "\"&"
//							+ getSignType();
//
//					Runnable payRunnable = new Runnable() {
//
//						@Override
//						public void run() {
//							// 鏋勯�燩ayTask 瀵硅薄
//							PayTask alipay = new PayTask(Recharge_Scene.this);
//							// 璋冪敤鏀粯鎺ュ彛锛岃幏鍙栨敮浠樼粨鏋�
//							String result = alipay.pay(payInfo);
//
//							Message msg = new Message();
//							msg.what = SDK_PAY_FLAG;
//							msg.obj = result;
//							mHandler.sendMessage(msg);
//						}
//					};
//
//					// 蹇呴』寮傛璋冪敤
//					Thread payThread = new Thread(payRunnable);
//					payThread.start();
//					break; 
//				}
//				default:
//					break;
//				}
//			};
//};
//
//	//鍚姩鏀粯
//	public void sendRequestWithHttpURLConnection(String Userid,String Username,String OrderID,String PayAmount){
//		
//		WebUrl = "http://wx.youxihongyu.com/ws/mobileinterface.ashx?action=getorder&userid="+Userid+"&payAccounts="+Username+"&payOrderId="+OrderID+"&payAmount="+PayAmount;
//		
//		//寮�鍚嚎绋嬫潵鍙戣捣缃戠粶璇锋眰         
//		new Thread(new Runnable(){               
//			@Override            public void run() 
//			{                 
//				// TODO Auto-generated method stub                 
//				HttpURLConnection connection=null;                                   
//				try {   
//					URL url=new URL(WebUrl);                     
//					connection =(HttpURLConnection) url.openConnection();                     
//					connection.setRequestMethod("GET");                     
//					connection.setConnectTimeout(8000);                     
//					connection.setReadTimeout(8000);                     
//					InputStream in=connection.getInputStream();                     
//					//涓嬮潰瀵硅幏鍙栧埌鐨勮緭鍏ユ祦杩涜璇诲彇                     
//					BufferedReader reader=new BufferedReader(new InputStreamReader(in));                     
//					StringBuilder response=new StringBuilder();                     
//					String line;                     
//					while((line=reader.readLine())!=null){                         
//						response.append(line);                     
//						}                     
//					Message message=new Message();                     
//					message.what=SHOW_RESPONSE;                     
//					//灏嗘湇鍔″櫒杩斿洖鐨勭粨鏋滃瓨鏀惧埌Message涓�                     
//					message.obj=response.toString();                     
//					mHandler.sendMessage(message);   
//					
//				} catch (MalformedURLException e) {                     
//						// TODO Auto-generated catch block                    
//					e.printStackTrace();                 
//				}catch(Exception e){                     
//					e.printStackTrace();                 
//				}finally{                     
//					if(connection!=null){                         
//						connection.disconnect();                     
//					}                 
//				}             
//			}                       
//			}).start(); 
//}
//		
//	
//	/**
//	 * get the sdk version. 鑾峰彇SDK鐗堟湰鍙�
//	 * 
//	 */
//	public void getSDKVersion() {
//		PayTask payTask = new PayTask(this);
//		String version = payTask.getVersion();
//		Toast.makeText(this, version, Toast.LENGTH_SHORT).show();
//	}
//
//	/**
//	 * create the order info. 鍒涘缓璁㈠崟淇℃伅
//	 * 
//	 */
//	public String getOrderInfo(String subject, String body, String price, String OrderID) {
//
//		// 绛剧害鍚堜綔鑰呰韩浠絀D
//		String orderInfo = "partner=" + "\"" + PARTNER + "\"";
//
//		// 绛剧害鍗栧鏀粯瀹濊处鍙�
//		orderInfo += "&seller_id=" + "\"" + SELLER + "\"";
//
//		// 鍟嗘埛缃戠珯鍞竴璁㈠崟鍙�
//		orderInfo += "&out_trade_no=" + "\"" +"PAP"+ OrderID + "\"";
//
//		// 鍟嗗搧鍚嶇О
//		orderInfo += "&subject=" + "\"" + subject + "\"";
//
//		// 鍟嗗搧璇︽儏
//		orderInfo += "&body=" + "\"" + body + "\"";
//
//		// 鍟嗗搧閲戦
//		orderInfo += "&total_fee=" + "\"" + price + "\"";
//
//		// 鏈嶅姟鍣ㄥ紓姝ラ�氱煡椤甸潰璺緞
//		//orderInfo += "&notify_url=" + "\"" + "http://notify.msp.hk/notify.htm"
//		orderInfo += "&notify_url=" + "\"" + "http://wx.youxihongyu.com/pay/alipay/notify_url.aspx"
//				+ "\"";
//
//		// 鏈嶅姟鎺ュ彛鍚嶇О锛� 鍥哄畾鍊�
//		orderInfo += "&service=\"mobile.securitypay.pay\"";
//
//		// 鏀粯绫诲瀷锛� 鍥哄畾鍊�
//		orderInfo += "&payment_type=\"1\"";
//
//		// 鍙傛暟缂栫爜锛� 鍥哄畾鍊�
//		orderInfo += "&_input_charset=\"utf-8\"";
//
//		// 璁剧疆鏈粯娆句氦鏄撶殑瓒呮椂鏃堕棿
//		// 榛樿30鍒嗛挓锛屼竴鏃﹁秴鏃讹紝璇ョ瑪浜ゆ槗灏变細鑷姩琚叧闂��
//		// 鍙栧�艰寖鍥达細1m锝�15d銆�
//		// m-鍒嗛挓锛宧-灏忔椂锛宒-澶╋紝1c-褰撳ぉ锛堟棤璁轰氦鏄撲綍鏃跺垱寤猴紝閮藉湪0鐐瑰叧闂級銆�
//		// 璇ュ弬鏁版暟鍊间笉鎺ュ彈灏忔暟鐐癸紝濡�1.5h锛屽彲杞崲涓�90m銆�
//		orderInfo += "&it_b_pay=\"30m\"";
//
//		// extern_token涓虹粡杩囧揩鐧绘巿鏉冭幏鍙栧埌鐨刟lipay_open_id,甯︿笂姝ゅ弬鏁扮敤鎴峰皢浣跨敤鎺堟潈鐨勮处鎴疯繘琛屾敮浠�
//		// orderInfo += "&extern_token=" + "\"" + extern_token + "\"";
//
//		// 鏀粯瀹濆鐞嗗畬璇锋眰鍚庯紝褰撳墠椤甸潰璺宠浆鍒板晢鎴锋寚瀹氶〉闈㈢殑璺緞锛屽彲绌�
//		orderInfo += "&return_url=\"m.alipay.com\"";
//
//		// 璋冪敤閾惰鍗℃敮浠橈紝闇�閰嶇疆姝ゅ弬鏁帮紝鍙備笌绛惧悕锛� 鍥哄畾鍊� 锛堥渶瑕佺绾︺�婃棤绾块摱琛屽崱蹇嵎鏀粯銆嬫墠鑳戒娇鐢級
//		// orderInfo += "&paymethod=\"expressGateway\"";
//
//		return orderInfo;
//	}
//	/**
//	 * get the out_trade_no for an order. 鐢熸垚鍟嗘埛璁㈠崟鍙凤紝璇ュ�煎湪鍟嗘埛绔簲淇濇寔鍞竴锛堝彲鑷畾涔夋牸寮忚鑼冿級
//	 * 
//	 */
//	public String getOutTradeNo() {
//		SimpleDateFormat format = new SimpleDateFormat("MMddHHmmss",
//				Locale.getDefault());
//		Date date = new Date();
//		String key = format.format(date);
//
//		Random r = new Random();
//		key = key + r.nextInt();
//		key = key.substring(0, 15);
//		return key;
//	}
//	/**
//	 * sign the order info. 瀵硅鍗曚俊鎭繘琛岀鍚�
//	 * 
//	 * @param content
//	 *            寰呯鍚嶈鍗曚俊鎭�
//	 */
//	public String sign(String content) {
//		return SignUtils.sign(content, RSA_PRIVATE);
//	}
//	/**
//	 * get the sign type we use. 鑾峰彇绛惧悕鏂瑰紡
//	 * 
//	 */
//
//	public String getSignType() {
//		return "sign_type=\"RSA\"";
//	}
//}

