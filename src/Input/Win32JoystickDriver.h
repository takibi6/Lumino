﻿
#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define INITGUID
#include <dinput.h>
#include <xinput.h>
#include <Lumino/Base/DllLoader.h>
#include "InputDriver.h"

LN_NAMESPACE_BEGIN

/// ジョイスティック1つ分
class Win32JoystickDriver
    : public RefObject
{
public:

	/// スティックの状態を表す構造体
	struct AnalogState
	{
		int32_t	X1;
		int32_t	Y1;
		int32_t	X2;
		int32_t	Y2;
	};

public:
	Win32JoystickDriver();
	virtual ~Win32JoystickDriver();

public:

	/// 初期化 (xinputNumber は XInput 用コントローラの割り当て番号。XInput 以外のものは -1 を渡すこと)
	void Initialize(IDirectInputDevice8* device, HWND hwnd, int xinputNumber, bool forcefeedback);

	/// 解放
	void Dispose();

	/// ジョイパッドの状態を取得する
	void GetJoystickDeviceState(JoystickDeviceState* state);

	/// XInput デバイスかを判定する
	bool IsXInputDevice() const { return mXInputNo >= 0; }

	/// ジョイパッドの振動を開始する
	void StartVibration(int power, int time);

	/// ジョイパッドの振動を停止する
	void StopVibration();

	/// フレーム更新
	void Update();

private:

	/// ジョイスティックの軸を列挙する関数
	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID thisDevice);

public:

	static const int AXIS_RANGE = 1000;							///< DirectInput に設定する軸の範囲 ( -1000～1000 )
	static const int MAX_BUTTON_NUM = JoystickDeviceState::MaxButtons;	///< ボタンの数
	static const int MAX_AXIS_NUM = JoystickDeviceState::MaxButtons;			///< 軸の数
	

private:
	IDirectInputDevice8*	mDevice;

	IDirectInputEffect*     mDeviceEffect;
	DWORD                   mDIAxes[2];
	LONG                    mDIDirection[2];
	DIPERIODIC              mDIPeriodic;
	DIENVELOPE              mDIEnvelope;
	DIEFFECT                mDIEffect;
	uint32_t				mVibrationStartTime;
	uint32_t				mVibrationTime;

	float                   mPlayRange;
	int                     mXInputNo;
};

/// XInput DLL モジュール
class XInputModule
{
public:

	/// 初期化
	static void Initialize();

public:

	typedef DWORD(WINAPI *MD_XInputGetState)(
		DWORD         dwUserIndex,
		XINPUT_STATE* pState
		);

	typedef DWORD(WINAPI *MD_XInputSetState)(
		DWORD             dwUserIndex,
		XINPUT_VIBRATION* pVibration
		);

	static MD_XInputGetState   XInputGetState;
	static MD_XInputSetState   XInputSetState;

private:
	static DllLoader			m_XInputModule;   ///< XINPUT_DLL ("XInput～.dll")
};

LN_NAMESPACE_END
