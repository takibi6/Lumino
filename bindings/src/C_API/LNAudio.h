﻿
#pragma once

#include "LNCommon.h"
#include "LNTypedef.h"

extern "C" {

/**
	@brief	音声機能
*/
LN_MODULE(Audio)
	
//==============================================================================
/**
	@brief	音声再生のユーティリティクラスです。
*/
LN_STATIC_CLASS(LNAudio)
	
	/**
		@brief		BGM を演奏します。
		@param[in]	filePath	: ファイルパス
		@param[in]	volume		: ボリューム (0 ～ 100)
		@param[in]	pitch		: ピッチ (50 ～ 200)
		@param[in]	fadeTime	: フェードインにかける時間 (秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayBGM(const LNChar* filePath, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100), double fadeTime LN_DEFAULT_ARG(0.0));
	
	/**
		@brief		メモリ上の音声ファイルデータを使用して BGM を演奏します。
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
		@param[in]  fadeTime	: フェードインにかける時間 (秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayBGMMem(const void* data, int dataSize, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100), double fadeTime LN_DEFAULT_ARG(0.0));

	/**
		@brief		BGM の演奏を停止します。
		@param[in]  fadeTime	: フェードアウトにかける時間 (秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_StopBGM(double fadeTime LN_DEFAULT_ARG(0.0));

	/**
		@brief		BGS を演奏します。
		@param[in]  filePath	: ファイルパス
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
		@param[in]  fadeTime	: フェードインにかける時間 (秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayBGS(const LNChar* filePath, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100), double fadeTime LN_DEFAULT_ARG(0.0));

	/**
		@brief		メモリ上の音声ファイルデータから BGS を演奏します。
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
		@param[in]  fadeTime	: フェードインにかける時間 (秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayBGSMem(const void* data, int dataSize, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100), double fadeTime LN_DEFAULT_ARG(0.0));

	/**
		@brief		BGS の演奏を停止します。、
		@param[in]  fadeTime	: フェードアウトにかける時間 (秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_StopBGS(double fadeTime LN_DEFAULT_ARG(0.0));

	/**
		@brief		ME を演奏します。
		@param[in]  filePath	: ファイルパス
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayME(const LNChar* filePath, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief		メモリ上の音声ファイルデータから ME を演奏します。
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayMEMem(const void* data, int dataSize, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief	  ME の演奏を停止します。
	*/
	LN_STATIC_API
	LNResult LNAudio_StopME();

	/**
		@brief		SE を演奏します。
		@param[in]  filePath	: ファイルパス
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySE(const LNChar* filePath, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief		SE を演奏します。(3D サウンド)
		@param[in]  filePath	: ファイルパス
		@param[in]  position	: 3D 空間上の座標
		@param[in]  distance	: 減衰距離
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySE3D(const LNChar* filePath, const LNVector3* position, float distance, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief		SE を演奏します。(3D サウンド)
		@param[in]  filePath	: ファイルパス
		@param[in]  x			: 3D 空間上の X 座標
		@param[in]  y			: 3D 空間上の Y 座標
		@param[in]  z			: 3D 空間上の Z 座標
		@param[in]  distance	: 減衰距離
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LN_ATTR_OVERLOAD(LNAudio_PlaySE3D)
	LNResult LNAudio_PlaySE3DXYZ(const LNChar* filePath, float x, float y, float z, float distance, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief		メモリ上の音声データから SE を演奏します。
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySEMem(const void* data, int dataSize, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief		メモリ上の音声ファイルデータから SE を演奏します。 (3D サウンド)
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  position	: 3D 空間上の座標
		@param[in]  distance	: 減衰距離
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySE3DMem(const void* data, int dataSize, const LNVector3* position, float distance, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief		メモリ上の音声ファイルデータから SE を演奏します。(3D サウンド)
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  x			: 3D 空間上の X 座標
		@param[in]  y			: 3D 空間上の Y 座標
		@param[in]  z			: 3D 空間上の Z 座標
		@param[in]  distance	: 減衰距離
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LN_ATTR_OVERLOAD(LNAudio_PlaySE3DMem)
	LNResult LNAudio_PlaySE3DMemXYZ(const void* data, int dataSize, float x, float y, float z, float distance, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief		すべての SE の演奏を停止します。
	*/
	LN_STATIC_API
	LNResult LNAudio_StopSE();

	/**
		@brief		再生中のBGMの音量を設定します。(フェードアウト中は無効)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  fadeTime	: フェードアウトにかける時間 (秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_SetBGMVolume(int volume, double fadeTime LN_DEFAULT_ARG(0.0));

	/**
		@brief		再生中のBGSの音量を設定します。(フェードアウト中は無効)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  fadeTime	: フェードアウトにかける時間 (秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_SetBGSVolume(int volume, double fadeTime LN_DEFAULT_ARG(0.0));


LN_CLASS_END

//==============================================================================
/**
	@brief		3D音声のリスナーに関する情報を表します。
*/
LN_STATIC_CLASS(LNSoundListener)

	/**
		@brief		3D音声のリスナーの位置を設定します。
		@param[in]	position	: 3D 空間上の座標
	*/
	LN_STATIC_API
	LN_ATTR_PROPERTY
	LNResult LNSoundListener_SetPosition(const LNVector3* position);

	/**
		@brief		3D音声のリスナーの位置を設定します。
		@param[in]	x			: 3D 空間上の X 座標
		@param[in]	y			: 3D 空間上の Y 座標
		@param[in]	z			: 3D 空間上の Z 座標
	*/
	LN_STATIC_API
	LN_ATTR_PROPERTY
	LN_ATTR_OVERLOAD(LNSoundListener_SetPositionXYZ)
	LNResult LNSoundListener_SetPositionXYZ(float x, float y, float z);

	/**
		@brief		3D音声のリスナーの正面方向を設定します。
		@param[in]	direction		: 向き
	*/
	LN_STATIC_API
	LN_ATTR_PROPERTY
	LNResult LNSoundListener_SetDirection(const LNVector3* direction);

	/**
		@brief		3D音声のリスナーの正面方向を設定します。
		@param[in]	x			: 向きの X 成分
		@param[in]	y			: 向きの Y 成分
		@param[in]	z			: 向きの Z 成分
	*/
	LN_STATIC_API
	LN_ATTR_PROPERTY
	LN_ATTR_OVERLOAD(LNSoundListener_SetDirection)
	LNResult LNSoundListener_SetDirectionXYZ(float x, float y, float z);

	/**
		@brief		3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
		@param[in]	direction	: 上方向
	*/
	LN_STATIC_API
	LN_ATTR_PROPERTY
	LNResult LNSoundListener_SetUpDirection(const LNVector3* direction);

	/**
		@brief		3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
		@param[in]	x			: 向きの X 成分
		@param[in]	y			: 向きの Y 成分
		@param[in]	z			: 向きの Z 成分
	*/
	LN_STATIC_API
	LN_ATTR_PROPERTY
	LN_ATTR_OVERLOAD(LNSoundListener_SetUpDirection)
	LNResult LNSoundListener_SetUpDirectionXYZ(float x, float y, float z);

	/**
		@brief		3D音声のリスナーの速度を設定します。
		@param[in]	velocity	: 速度
	*/
	LN_STATIC_API
	LN_ATTR_PROPERTY
	LNResult LNSoundListener_SetVelocity(const LNVector3* velocity);

	/**
		@brief		3D音声のリスナーの速度を設定します。
		@param[in]	x			: 速度の X 成分
		@param[in]	y			: 速度の Y 成分
		@param[in]	z			: 速度の Z 成分
	*/
	LN_STATIC_API
	LN_ATTR_PROPERTY
	LN_ATTR_OVERLOAD(LNSoundListener_Velocity)
	LNResult LNSoundListener_SetVelocityXYZ(float x, float y, float z);

LN_CLASS_END
	
//==============================================================================
/**
	@brief		音声データひとつ分を表し、再生などの操作を行うクラスです。
*/
LN_CLASS(LNSound, LNObject)

	/**
		@brief		ファイルからサウンドオブジェクトを作成します。
		@param[in]	filePath	: 音声ファイルのパス
		@param[out]	sound		: 作成されたサウンドオブジェクトのハンドルを格納する変数のアドレス
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNSound_Create(const LNChar* filePath, LN_OUT LN_HANDLE(LNSound)* sound);
		
	/**
		@brief		メモリ上の音声ファイルデータからサウンドオブジェクトを作成します。
		@param[in]	data		: メモリ上の音声データへのポインタ
		@param[in]	dataSize	: データサイズ (バイト単位)
		@param[out]	sound		: 作成されたサウンドオブジェクトのハンドルを格納する変数のアドレス
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNSound_CreateMem(const void* data, int dataSize, LN_OUT LN_HANDLE(LNSound)* sound);

	/**
		@brief		サウンドのボリュームを設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	volume		: ボリューム (0～100)
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_SetVolume(LN_HANDLE(LNSound) sound, int volume);

	/**
		@brief		サウンドのボリュームを取得します。
		@param[in]	sound		: サウンドハンドル
		@param[out]	outVolume	: ボリュームを格納する変数
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_GetVolume(LN_HANDLE(LNSound) sound, int* outVolume);

	/**
		@brief		サウンドのピッチを設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	pitch		: ピッチ (50～200)
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_SetPitch(LN_HANDLE(LNSound) sound, int pitch);

	/**
		@brief		サウンドのピッチを取得します。
		@param[in]	sound		: サウンドハンドル
		@param[out]	outPitch	: ピッチを格納する変数
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_GetPitch(LN_HANDLE(LNSound) sound, int* outPitch);

	/**
		@brief		サウンドのループ再生の有無を設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	loopEnable	: LN_TRUE = ループ再生する / LN_FALSE = しない
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_SetLoopEnabled(LN_HANDLE(LNSound) sound, LNBool loopEnable);

	/**
		@brief		サウンドのループ再生が有効であるかを確認します。
		@param[in]	sound		: サウンドハンドル
		@param[out]	outEnabled	: ループ再生の有無状態を格納する変数
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_IsLoopEnabled(LN_HANDLE(LNSound) sound, LNBool* outEnabled);
	
	/**
		@brief		サウンドのループ再生の範囲を設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	begin		: ループ領域の先頭位置 (サンプル数単位)
		@param[in]	length		: ループ領域長さ (サンプル数単位)
		@details	begin と length に 0 を指定すると、全体をループ領域として設定します。
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_SetLoopRange(LN_HANDLE(LNSound) sound, int begin, int length);

	/**
		@brief		サウンドを 3D 音源として再生するかを設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	enabled		: LN_TRUE = 3D音声 / LN_FALSE = 非3D
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_Set3DEnabled(LN_HANDLE(LNSound) sound, LNBool enabled);

	/**
		@brief		サウンドが 3D 音源であるかを確認します。
		@param[in]	sound		: サウンドハンドル
		@param[out]	outEnabled	: 状態を格納する変数のアドレス (LN_TRUE = 3D音声 / LN_FALSE = 非 3D)
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_Is3DEnabled(LN_HANDLE(LNSound) sound, LNBool* outEnabled);

	/**
		@brief		サウンド再生時の音声データの再生方法を設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	mode		: 読み込み方法
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_SetPlayingMode(LN_HANDLE(LNSound) sound, LNSoundPlayingMode mode);

	/**
		@brief		サウンド再生時の音声データの再生方法を取得します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	outMode		: 再生方法を格納する変数のアドレス
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_GetPlayingMode(LN_HANDLE(LNSound) sound, LNSoundPlayingMode* outMode);

	/**
		@brief		サウンドの再生状態を取得します。
		@param[in]	sound		: サウンドハンドル
		@param[out]	outState	: 状態を格納する変数
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_GetPlayingState(LN_HANDLE(LNSound) sound, LNSoundPlayingState* outState);

	/**
		@brief		サウンドを再生します。
		@param[in]	sound		: サウンドハンドル
	*/
	LN_INSTANCE_API
	LNResult LNSound_Play(LN_HANDLE(LNSound) sound);

	/**
		@brief		サウンドの再生を停止します。
		@param[in]	sound		: サウンドハンドル
	*/
	LN_INSTANCE_API
	LNResult LNSound_Stop(LN_HANDLE(LNSound) sound);

	/**
		@brief		サウンドの再生を一時停止します。
		@param[in]	sound		: サウンドハンドル
	*/
	LN_INSTANCE_API
	LNResult LNSound_Pause(LN_HANDLE(LNSound) sound);

	/**
		@brief		サウンドの再生を一時停止します。
		@param[in]	sound		: サウンドハンドル
	*/
	LN_INSTANCE_API
	LNResult LNSound_Resume(LN_HANDLE(LNSound) sound);

	/**
		@brief		サウンド音量のフェード操作を行います。
		@param[in]	sound			: サウンドハンドル
		@param[in]	targetVolume	: 変更先の音量
		@param[in]	time			: フェードにかける時間 (秒)
		@param[in]	behavior		: フェード完了後の動作の指定
	*/
	LN_INSTANCE_API
	LNResult LNSound_FadeVolume(LN_HANDLE(LNSound) sound, int targetVolume, double time, LNSoundFadeBehavior behavior);

	/**
		@brief		サウンドの再生したサンプル数を取得します。
		@param[in]	sound			: サウンドハンドル
		@param[out]	outSamples		: 再生したサンプル数を格納する変数
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_GetPlayedSamples(LN_HANDLE(LNSound) sound, int64_t* outSamples);

	/**
		@brief		サウンドの音声データ全体のサンプル数を取得します。
		@param[in]	sound			: サウンドハンドル
		@param[out]	outSamples		: 音声データ全体のサンプル数を格納する変数
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_GetTotalSamples(LN_HANDLE(LNSound) sound, int64_t* outSamples);

	/**
		@brief		サウンドのサンプリングレートを取得します。
		@param[in]	sound		: サウンドハンドル
		@param[out]	outRate		: サンプリングレートを格納する変数
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_GetSamplingRate(LN_HANDLE(LNSound) sound, int32_t* outRate);

	/**
		@brief		サウンドの 3D 音源としての位置を設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	position	: 3D 空間上の座標
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_SetEmitterPosition(LN_HANDLE(LNSound) sound, const LNVector3* position);

	/**
		@brief		サウンドの 3D 音源としての位置を設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	x			: 3D 空間上の X 座標
		@param[in]	y			: 3D 空間上の Y 座標
		@param[in]	z			: 3D 空間上の Z 座標
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LN_ATTR_OVERLOAD(LNSound_SetEmitterPosition)
	LNResult LNSound_SetEmitterPositionXYZ(LN_HANDLE(LNSound) sound, float x, float y, float z);

	/**
		@brief		サウンドの 3D 音源としての速度を設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	velocity	: 速度
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_SetEmitterVelocity(LN_HANDLE(LNSound) sound, const LNVector3* velocity);

	/**
		@brief		サウンドの 3D 音源としての速度を設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	x			: 速度の X 成分
		@param[in]	y			: 速度の Y 成分
		@param[in]	z			: 速度の Z 成分
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LN_ATTR_OVERLOAD(LNSound_SetEmitterVelocity)
	LNResult LNSound_SetEmitterVelocityXYZ(LN_HANDLE(LNSound) sound, float x, float y, float z);

	/**
		@brief		サウンドの 3D 音源の減衰距離 (聴こえなくなる距離) を設定します。
		@param[in]	sound		: サウンドハンドル
		@param[in]	distance	: 距離
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSound_SetEmitterDistance(LN_HANDLE(LNSound) sound, float distance);

LN_CLASS_END
	
LN_MODULE_END

} // extern "C"
