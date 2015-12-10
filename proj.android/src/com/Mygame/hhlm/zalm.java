/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

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
package com.Mygame.hhlm;


import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import cn.cmgame.billing.api.BillingResult;
import cn.cmgame.billing.api.GameInterface;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Toast;


public class zalm extends Cocos2dxActivity{
	
	public static zalm Instance = null;

	Activity thisActivity;
	
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		
		Instance = this;
		thisActivity = this;
		
		GameInterface.initializeApp(this);
		
	}
    
    final GameInterface.IPayCallback payCallback = new GameInterface.IPayCallback() {
		@Override
		public void onResult(int resultCode, String billingIndex, Object obj) {
			String result = "";
			switch (resultCode) {
			case BillingResult.SUCCESS:
				result = "购买道具：[" + billingIndex + "] 成功！";
				zalm.orderBombSuccess();
				break;
			case BillingResult.FAILED:
				result = "购买道具：[" + billingIndex + "] 失败！";
				zalm.orderFaild();
				break;
			default:
				result = "购买道具：[" + billingIndex + "] 取消！";
				zalm.orderFaild();
				break;
			}
			Toast.makeText(zalm.this, result, Toast.LENGTH_SHORT).show();
		}
	};

	public void orderBomb(int buyid) {
		System.out.println("buyid:" + buyid);
		String billingIndex = getBillingIndex(buyid);
		GameInterface.doBilling(zalm.this, true, true, billingIndex, null,
				payCallback);
	}

	private String getBillingIndex(int i) {
		if (i < 9) {
			return "00" + (++i);
		} else {
			return "0" + (++i);
		}
	}

	public static boolean isSound() {
		System.out.println("jbl:" + GameInterface.isMusicEnabled());
		return GameInterface.isMusicEnabled();
	}

	private void exitGame() {
		// 移动退出接口，含确认退出UI
		// 如果外放渠道（非移动自有渠道）限制不允许包含移动退出UI，可用exitApp接口（无UI退出）
		GameInterface.exit(this, new GameInterface.GameExitCallback() {
			@Override
			public void onConfirmExit() {
				zalm.this.finish();
				System.exit(0);
			}

			@Override
			public void onCancelExit() {
				Toast.makeText(zalm.this, "取消退出", Toast.LENGTH_SHORT).show();
			}
		});
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// wllm should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

	public static Object getIntence() {
		return Instance;
	}

	static {
		System.loadLibrary("cocos2dcpp");
	}

	public native static void orderBombSuccess();

	public native static void orderFaild();   
}

