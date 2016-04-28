﻿
#include "Internal.h"
#include <Lumino/Base/Exception.h>
#include <Lumino/Math/MathUtils.h>
#include "AudioStream.h"
#include "AudioDevice.h"
#include "AudioPlayer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

//=============================================================================
// AudioPlayerState 
//=============================================================================
AudioPlayerState::AudioPlayerState()
	: m_modified(ModifiedFlags_None)
	, m_volume(1.0f)
	, m_pitch(1.0f)
	, m_loopEnabled(false)
	, m_loopBegin(0)
	, m_loopLength(0)
	, m_playingState(SoundPlayingState::Stopped)
{
}

//=============================================================================
// AudioPlayer 
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioPlayer::AudioPlayer(AudioDevice* device)
	: mDevice(device)
	, m_audioStream(NULL)
	, mVolume(1.0f)
	, mPitch(1.0f)
	, mLoopBegin(0)
	, mLoopLength(0)
	, mIsPlaying(false)
	, mIsPausing(false)
	, mIsLoop(false)
{
	LN_SAFE_ADDREF(mDevice);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioPlayer::~AudioPlayer()
{
	LN_SAFE_RELEASE(m_audioStream);
	LN_SAFE_RELEASE(mDevice);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioPlayer::Initialize(AudioStream* audioStream, bool enable3d)
{ 
	LN_THROW(audioStream, ArgumentException);
	LN_REFOBJ_SET(m_audioStream, audioStream);
	m_decoder = m_audioStream->GetDecoder();

    // ソースからループ位置を取得
    uint32_t loop_begin, loop_length;
	m_decoder->GetLoopState(&loop_begin, &loop_length);
    mLoopBegin  = loop_begin;
	mLoopLength = loop_length;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioPlayer::SetVolume(float volume)
{
	mVolume = Math::Clamp(volume, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioPlayer::SetPitch(float pitch)
{
	mPitch = Math::Clamp(pitch, 0.5f, 2.0f);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioPlayer::SetLoopState(uint32_t loop_begin, uint32_t loop_length)
{
    if ( loop_begin == 0 && loop_length == 0 )
    {
        // ソースからループ位置を取得して設定する
		uint32_t begin, length;
		m_decoder->GetLoopState(&begin, &length);
        mLoopBegin  = begin;
		mLoopLength = length;
    }
    else
    {
        mLoopBegin = loop_begin;
		mLoopLength = loop_length;
    }
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
