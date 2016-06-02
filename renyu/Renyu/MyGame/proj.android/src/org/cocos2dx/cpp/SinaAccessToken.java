package org.cocos2dx.cpp;

import java.util.UUID;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.SparseArray;
import android.widget.Toast;

import com.hongyu.renyu.R;
import com.sina.weibo.sdk.auth.Oauth2AccessToken;
import com.sina.weibo.sdk.auth.WeiboAuth;
import com.sina.weibo.sdk.auth.WeiboAuthListener;
import com.sina.weibo.sdk.constant.WBConstants;
import com.sina.weibo.sdk.exception.WeiboException;
import com.sina.weibo.sdk.net.AsyncWeiboRunner;
import com.sina.weibo.sdk.net.RequestListener;
import com.sina.weibo.sdk.net.WeiboParameters;
import com.sina.weibo.sdk.utils.LogUtil;

/** 第三方登陆 新浪微博 **/
public class SinaAccessToken implements WeiboAuthListener {
	public String	szToken;
	public String		lUID;
	public String	szNickName;
	public String	sImgUrl;
	public byte		gender = 1;
	protected Context	context;
	/** SINA KEY **/
	public static  String	SINA_CONSUMER_KEY	= "3031330717";// 替换为开发者的appkey
	/** SINA callback url **/
	public static  String	SINA_REDIRECT_URL	= "http://www.foxuc.net/";// 回调页面
	
	public static  String	SINA_APP_SECRET		= "40a9cd790cc82200ae1c7c13226e65e3";
	
	
	// 新支持scope 支持传入多个scope权限，用逗号分隔  
	public static final String	SCOPE				= "statuses_to_me_read," + "follow_app_official_microblog";
	public static final String	TAG					= "SinaAccessToken";
	/** 微博实例 **/
	private WeiboAuth			m_Weibo;

    /** 通过 code 获取 Token 的 URL */
    private static final String OAUTH2_ACCESS_TOKEN_URL = "https://open.weibo.cn/oauth2/access_token";
    /** 访问微博服务接口的地址 */
    protected static final String API_SERVER       = "https://api.weibo.com/2";
    /** POST 请求方式 */
    protected static final String HTTPMETHOD_POST  = "POST";
    /** GET 请求方式 */
    protected static final String HTTPMETHOD_GET   = "GET";
    /** HTTP 参数 */
    protected static final String KEY_ACCESS_TOKEN = "access_token";
    
    private static final int READ_USER           = 0;
    private static final int READ_USER_BY_DOMAIN = 1;
    private static final int READ_USER_COUNT     = 2;

    private static final String API_BASE_URL 	 = API_SERVER + "/users";
    
    private static final int	SUCCESS 		 = 0;

    private static final SparseArray<String> sAPIList = new SparseArray<String>();
    static {
        sAPIList.put(READ_USER,           API_BASE_URL + "/show.json");
        sAPIList.put(READ_USER_BY_DOMAIN, API_BASE_URL + "/domain_show.json");
        sAPIList.put(READ_USER_COUNT,     API_BASE_URL + "/counts.json");
    }
    
    public static SinaAccessToken ptest;
    public static boolean getInstance()
    {
    	AppActivity.getInstance().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				ptest.Login();
			}
		});
    	
    	return true;
    }
    public static native void sinaLoginSuccess(final String useid,final String headurl,final String nickname,
    		final byte c);
    
	public SinaAccessToken(Context context) {
		if (m_Weibo == null) {
			m_Weibo = new WeiboAuth(context, SINA_CONSUMER_KEY, SINA_REDIRECT_URL, SCOPE);
		}
		this.context = context;
		szToken = "";
		lUID = "";
	}

	public String getNickName() {
		return szNickName;
	}

	public String getToken() {
		return szToken;
	}

	public String getUID() {
		return lUID;
	}
	
	public boolean Login() {
		m_Weibo.authorize(ptest, WeiboAuth.OBTAIN_AUTH_CODE);
		return true;
	}

	public void onCancel() {
		
	}

	@Override
	public void onComplete(Bundle values) {

		String code = values.getString("code");
		if (code != null) {
			fetchTokenAsync(code);
		}
	}

	@Override
	public void onWeiboException(WeiboException arg0) {
		// TODO Auto-generated method stub
		
	}
	
    Handler handler = new Handler(new Handler.Callback() {
		
		@Override
		public boolean handleMessage(Message arg0) {
			// TODO Auto-generated method stub
			if(arg0.what == SUCCESS){
			    //获取用户信息
			    WeiboParameters params = new WeiboParameters();
			    params.put("uid", lUID);
			    params.put(KEY_ACCESS_TOKEN, szToken);
			    AsyncWeiboRunner.requestAsync(sAPIList.get(READ_USER), params, HTTPMETHOD_GET, mListener);
			}
			return false;
		}
	});
	
    /**
     * 异步获取 Token。
     * 
     * @param authCode  授权 Code，该 Code 是一次性的，只能被获取一次 Token
     * @param appSecret 应用程序的 APP_SECRET，请务必妥善保管好自己的 APP_SECRET，
     *                  不要直接暴露在程序中。
     */
    public void fetchTokenAsync(String authCode) {
        
        WeiboParameters requestParams = new WeiboParameters();
        requestParams.put(WBConstants.AUTH_PARAMS_CLIENT_ID,     SINA_CONSUMER_KEY);
        requestParams.put(WBConstants.AUTH_PARAMS_CLIENT_SECRET, SINA_APP_SECRET);
        requestParams.put(WBConstants.AUTH_PARAMS_GRANT_TYPE,    "authorization_code");
        requestParams.put(WBConstants.AUTH_PARAMS_CODE,          authCode);
        requestParams.put(WBConstants.AUTH_PARAMS_REDIRECT_URL,  SINA_REDIRECT_URL);
        
        // 异步请求，获取 Token
        AsyncWeiboRunner.requestAsync(OAUTH2_ACCESS_TOKEN_URL, requestParams, "POST", new RequestListener() {
            @Override
            public void onComplete(String response) {
                LogUtil.d(TAG, "Response: " + response);
                
                // 获取 Token 成功
                Oauth2AccessToken token = Oauth2AccessToken.parseAccessToken(response);
                if (token != null && token.isSessionValid()) {
                    szToken = token.getToken();
                    lUID = token.getUid();
                    handler.sendEmptyMessage(SUCCESS);
                } else {
                    LogUtil.d(TAG, "Failed to receive access token");
                }
            }

            @Override
            public void onWeiboException(WeiboException e) {
                LogUtil.e(TAG, "onWeiboException： " + e.getMessage());
            }
        });
    }
  
    /**
     * 微博 OpenAPI 回调接口。
     */
    private RequestListener mListener = new RequestListener() {
        @Override
        public void onComplete(String response) {
            if (!TextUtils.isEmpty(response)) {
            	JSONObject jsonObject;
				try {
					jsonObject = new JSONObject(response);
					szNickName =  jsonObject.optString("screen_name", "");
					sImgUrl = jsonObject.optString("profile_image_url", "");
					String sgender = jsonObject.optString("gender", "");
					gender = (byte)(sgender.equals("m") ? 1 : 0);
					LogUtil.e(TAG, response);
					sinaLoginSuccess(lUID, sImgUrl, szNickName, gender);

					
					//gender = jsonObject.optstrin
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            }
        }

        @Override
        public void onWeiboException(WeiboException e) {
            LogUtil.e(TAG, e.getMessage());
            ErrorInfo info = new ErrorInfo();
            info = info.parse(e.getMessage());
            Toast.makeText(context, info.toString(), Toast.LENGTH_LONG).show();
        }
    };
    
    class ErrorInfo {
        public String error;
        public String error_code;
        public String request;

        public ErrorInfo parse(String jsonString) {
            if (TextUtils.isEmpty(jsonString)) {
                return null;
            }

            ErrorInfo errorInfo = new ErrorInfo();
            try {
                JSONObject jsonObject = new JSONObject(jsonString);
                errorInfo.error      = jsonObject.optString("error");
                errorInfo.error_code = jsonObject.optString("error_code");
                errorInfo.request    = jsonObject.optString("request");
            } catch (JSONException e) {
                e.printStackTrace();
            }
            return errorInfo;
        }
        
        @Override
        public String toString() {
            return "error: " + error + 
                   ", error_code: " + error_code + 
                   ", request: " + request;
        }
    }
    
}