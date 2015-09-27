#pragma once

namespace
{
	//キー
	BYTE
		gKey[256],    //現在のキー状態
		gKeyOld[256]; //1フレーム前のキー状態

	//キーオン関数
	bool keyOn(BYTE key)
	{
		//キーが押されているかどうかを返す
		return ((gKey[key] & 0xf0) != 0);
	}

	//キータッチ関数
	bool keyTouch(BYTE key)
	{
		//現在，押されているキーが，前回，離されているかどうかを返す
		return 
			((gKey[key] & 0xf0) != 0) &&  //現在，キーが押されている
			((gKeyOld[key] & 0xf0) == 0); //前回，キーが離されている
	}

	//マウス
	DIMOUSESTATE2
		gMouseState,    //現在のマウス状態
		gMouseStateOld; //1フレーム前のマウス状態

	//マウスオン関数
	bool mouseOn(BYTE key)
	{
		//マウスが押されているかどうかを返す
		return (!!gMouseState.rgbButtons[key]);
	}

	//マウスタッチ関数
	bool mouseTouch(BYTE key)
	{
		//現在，押されているマウスが，前回，離されているかどうかを返す
		return
			(!!gMouseState.rgbButtons[key] && //現在，マウスが押されている
			!gMouseStateOld.rgbButtons[key]); //前回，マウスが離されている
	}
}