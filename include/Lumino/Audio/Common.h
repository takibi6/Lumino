/*
	@file	Common.h
*/
#pragma once

#include <Lumino/Math/Vector3.h>
#include <Lumino/Base/Common.h>

#define LN_NAMESPACE_AUDIO_BEGIN	//namespace Audio {
#define LN_NAMESPACE_AUDIO_END		//}

#define LN_MUSIC_TIME_BASE	768	// TODO

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

/// 音声ファイルフォーマット
enum StreamFormat
{
	StreamFormat_Unknown = 0,		///< 不明なファイル
	StreamFormat_Wave,				///< WAVE
	StreamFormat_Mp3,				///< MP3
	StreamFormat_Ogg,				///< OGG
	StreamFormat_Midi,				///< MIDI

	StreamFormat_Max,				///< (terminator)
};

/// PCM フォーマット
struct WaveFormat
{
	uint16_t	FormatTag;
	uint16_t	Channels;
	uint32_t	SamplesPerSec;
	uint32_t	AvgBytesPerSec;
	uint16_t	BlockAlign;
	uint16_t	BitsPerSample;
	uint16_t	EXSize;
};

/** DirectMusic の初期化方法。*/
enum class DirectMusicMode
{
	NotUse = 0,			/**< DirectMusic を使用しない */
	Normal,				/**< 通常 */
	ThreadWait,			/**< 別スレッドで初期化して、再生時に未完了の場合は待つ */
	ThreadRequest,		/**< 別スレッドで初期化して、再生時に未完了の場合は再生を予約する */

	TERMINATOR,
};

/** 音声データの読み込み方法を表します。*/
enum class SoundLoadingMode
{
	Sync = 0,		/**< 同期的に読み込む */
	ASync,			/**< 非同期的に読み込む */

	TERMINATOR,
};

/// 再生方法
enum class SoundPlayingMode
{
	Unknown = 0,		///< 不明な再生方法 (自動選択)	TODO: 名前を Auto にする
	//SoundPlayType_Auto,				///< 自動選択 ( デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります )
	OnMemory,			///< オンメモリ
	Streaming,	    ///< ストリーミング
	Midi,  			///< SMF

	TERMINATOR,
};

/** 音声の再生状態を表します。*/
enum class SoundPlayingState
{
	Stopped = 0,		/**< 停止中 */
	Playing,			/**< 再生中 */
	Pausing,			/**< 一時停止中 */

	TERMINATOR,
};

/** 音量フェード完了時の動作を表します。*/
enum class SoundFadeBehavior
{
	Continue = 0,		/**< 再生を継続する */
	Stop,				/**< 停止する */
	StopReset,			/**< 停止して、次の再生に備えてサウンドの音量を元の値に戻す */
	Pause,				/**< 一時停止する */
	PauseReset,			/**< 一時停止して、次の再生に備えてサウンドの音量を元の値に戻す */

	TERMINATOR,
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
