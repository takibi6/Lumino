﻿
#include "../Internal.h"
#include "DirectMusicAudioDevice.h"
#include "DirectMusicAudioPlayer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//==============================================================================
// DirectMusicAudioPlayer
//==============================================================================

//------------------------------------------------------------------------------
DirectMusicAudioPlayer::DirectMusicAudioPlayer( AudioDevice* manager )
	: AudioPlayer(manager)
	, m_midiDecoder(NULL)
	, m_segment(NULL)
	, m_pauseTime(0)
{
    //LN_SAFE_ADDREF( mManager );
}

//------------------------------------------------------------------------------
DirectMusicAudioPlayer::~DirectMusicAudioPlayer()
{
	LN_SAFE_DELETE(m_segment);

	//if (mManager)
	//{
	//	mManager->removeAudioPlayer(this);
	//	LN_SAFE_RELEASE(mManager);
	//}
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::Initialize(AudioStream* audioStream, bool enable3d)
{
	m_midiDecoder = dynamic_cast<MidiDecoder*>(audioStream->GetDecoder());
	LN_THROW(m_midiDecoder != NULL, ArgumentException);

	AudioPlayer::Initialize(audioStream, enable3d);

	// オンメモリ再生用に内部に持ってるバッファを埋める
	m_midiDecoder->FillOnmemoryBuffer();

	if (DirectMusicManager::GetInstance()->IsInitialized() && !m_segment)
	{
		m_segment = LN_NEW DirectMusicSegment(
			DirectMusicManager::GetInstance(),
			DirectMusicManager::GetInstance()->CreateDMPerformance(),
			m_midiDecoder);
	}
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::SetVolume(float volume)
{
	AudioPlayer::SetVolume(volume);
	if (m_segment) {
		m_segment->SetVolume(mVolume);
	}
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::SetPitch(float pitch)
{
	AudioPlayer::SetPitch(pitch);
	if (m_segment) {
		m_segment->SetPitch(mPitch);
	}
}

//------------------------------------------------------------------------------
uint64_t DirectMusicAudioPlayer::GetPlayedSamples() const
{
	if (m_segment) {
		return m_segment->GetPlayPosition();
	}
	return 0;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::SetLoopState(uint32_t loopBegin, uint32_t loopLength)
{
	mLoopBegin = loopBegin;
	mLoopLength = loopLength;
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::Play()
{
	if (!DirectMusicManager::GetInstance()->IsInitialized())
	{
		// 同じものは追加されないのでこのままで OK
		DirectMusicManager::GetInstance()->AddPlayRequest(this);
		return;
	}

	// 再生開始
	_play();
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::Stop()
{
	if (m_segment) {
		m_segment->Stop();
	}
	mIsPlaying = false;
	mIsPausing = false;
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::Pause(bool isPause)
{
	// 再生中の場合
	if (mIsPlaying)
	{
		// 一時停止する場合で、現在一時停止していない場合
		if (isPause && !mIsPausing)
		{
			// 現在の演奏位置を記憶して停止
			m_pauseTime = m_segment->GetPlayPosition();
			m_segment->Stop();
			mIsPausing = true;
		}
		else if (!isPause && mIsPausing)
		{
			// 一時停止した時の再生位置に移動
			m_segment->SetPlayPosition(m_pauseTime);
			m_segment->Play();
			mIsPausing = false;
		}
	}
}

//------------------------------------------------------------------------------
bool DirectMusicAudioPlayer::Polling()
{
	// 再生中ではない場合は中断
	if (!mIsPlaying) {
		return false;
	}
	// ポーズ中は何もしないが再生中扱いとする
	if (mIsPausing) {
		return true;
	}

	// ループ再生ではない場合
	if (!mIsLoop)
	{
		if (m_segment)
		{
			// 再生開始直後は IDirectMusicPerformance8::IsPlaying() は false になってしまい、
			// TotalTime() も実際に音が鳴る長さよりも若干短くなってしまっている。
			// (DirectMusic の仕様？というか、バグな気もする…)
			// そのため、音が鳴っている事と、再生位置による二重判定を行う。
			if (!m_segment->IsPlaying() && m_segment->GetPlayPosition() >= m_segment->GetTotalTime())
			{
				Stop();
				return false;
			}
		}
	}

	return true;
}

// 臨時用。全体の再生時間の取得
uint32_t DirectMusicAudioPlayer::getTotalTime() const
{
	if (m_segment) // 未初期化対策
	{
		return m_segment->GetTotalTime();
	}
	return 0;
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::onFinishDMInit(IDirectMusicPerformance8* dmPerformance)
{
	m_segment = LN_NEW DirectMusicSegment(DirectMusicManager::GetInstance(), dmPerformance, m_midiDecoder);

	// 初期化中に設定されたパラメータを再設定する
	SetVolume(mVolume);
	SetPitch(mPitch);

	_play();
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::_play()
{
	// とりあえず停止
	Stop();

	if (!m_segment)
	{
		m_segment = LN_NEW DirectMusicSegment(
			DirectMusicManager::GetInstance(),
			DirectMusicManager::GetInstance()->CreateDMPerformance(),
			m_midiDecoder);

		// ちなみに setAudioSource() で作成するのはダメ。
		// DirectMusic の初期化中に setAudioSource() が呼ばれた場合はもちろん m_segment = NULL だけど、
		// そのあと play() が呼ばれたときにも当然 NULL のまま。
		// 初期化中に一度でも play() を呼んで要求リストに追加していれば onFinishDMInit() が呼ばれるけど、
		// そうでない場合はずっと NULL のままになってしまう。
	}

	// ループ再生する場合
	if (mIsLoop)
	{
		// MIDI のループ位置計算は他のとはちょっと違うのでここで、mLoopBegin とかには格納せずここで計算してしまう
		uint32_t cc111time;
		uint32_t base_time;
		m_midiDecoder->GetLoopState(&cc111time, &base_time);

		uint32_t begin = cc111time * LN_MUSIC_TIME_BASE / base_time;
		m_segment->SetLoopState(true, begin, 0);	// length=0 で終端まで再生する
	}
	// ループ再生しない場合
	else {
		m_segment->SetLoopState(false, 0, 0);
	}

	m_segment->Play();

	mIsPlaying = true;
	mIsPausing = false;
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
