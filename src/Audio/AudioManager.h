
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Base/RefObject.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Audio/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
class GameAudio;
class AudioDevice;
class AudioStream;
class AudioPlayer;
class Sound;
class GameAudioImpl;

/*
	@brief	�����@�\�̊Ǘ��N���X�ł��B
*/
class AudioManagerImpl
	: public RefObject
{
public:
	/// initialize() �ɓn���������f�[�^
	struct Settings
	{
		ln::FileManager*		FileManager;					///< �t�@�C������̓ǂݍ��݂Ɏg���t�@�C���Ǘ��N���X
		uint32_t				StreamCacheObjectCount;			///< �L���b�V���T�C�Y (�t�@�C����)
		uint32_t				StreamSourceCacheMemorySize;	///< �L���b�V���T�C�Y (��������(byte))
		DirectMusicMode			DMInitMode;						///< DirectMusic �̏��������@
		void*					hWnd;							///< DirectMusic �̏������Ɏg���E�B���h�E�n���h��
		float					DirectMusicReverbLevel;

		Settings()
			: FileManager(NULL)
			, StreamCacheObjectCount(32)
			, StreamSourceCacheMemorySize(0)
			, DMInitMode(DirectMusicMode::NotUse)
			, hWnd(NULL)
			, DirectMusicReverbLevel(0.75f)
		{}
	};

public:
	static AudioManagerImpl* GetInstance(AudioManagerImpl* priority = nullptr);

	static AudioManagerImpl* Create(const Settings& settings);

public:

	/// �I������
	void Finalize();

	/// �I��������or�X�g���[�~���O�����I���̉����f�[�^�o�C�g��臒l
	void SetAutoPlayTypeSelectThreshold(uint32_t threshold) { mOnMemoryLimitSize = threshold; }

	GameAudioImpl* GetGameAudio() const { return m_gameAudio; }

	AudioDevice* GetAudioDevice() const { return m_audioDevice; }

//LN_INTERNAL_ACCESS:
public:	// TODO
	friend class Sound;	// TODO
	FileManager* GetFileManager() { return m_fileManager; }
	//AudioStream* CreateAudioStream(const TCHAR* filePath);
	AudioStream* CreateAudioStream(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode);
	AudioPlayer* CreateAudioPlayer(AudioStream* stream, SoundPlayingMode mode, bool enable3D);		// �����������ς݂� AudioStream ��n������
	Sound* CreateSound(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode);



	/// �L�[�ɑΉ�����I�[�f�B�I�\�[�X���������� (���������ꍇ�� addRef ���ĕԂ�)
	//AudioStream* FindAudioSource(lnSharingKey key);

	/// �I�[�f�B�I�\�[�X�̍쐬 TODO:internal ��
	//AudioStream* CreateAudioStream(const TCHAR* filePath);
	//AudioStream* CreateAudioStream(Stream* stream, const CacheKey& key);

	//AudioPlayer* CreateAudioPlayer(AudioStream* stream, SoundPlayType type, bool enable3D);

	//

	///// Sound �̍쐬 ( stream_ = NULL �ŃL�[���g�������������s�� )
	//Sound* createSound(FileIO::Stream* stream, SoundPlayType type, bool enable_3d, lnSharingKey key);

	///// Sound �̍쐬 ( �t�@�C�����w�� )
	//Sound* createSound(const lnChar* filename, SoundPlayType type, bool enable_3d);

	///// Sound �̍쐬 ( IAudioSource �w�� )
	//Sound* createSound(AudioSourceBase* source, SoundPlayType type, bool enable_3d);

	/// �O���[�v�̒�~
	//void stopGroup(lnU32 group);

private:
	AudioManagerImpl(const Settings& configData);
	virtual ~AudioManagerImpl();
	void Thread_Polling();

private:

	FileManager*			m_fileManager;
	AudioDevice*			m_audioDevice;		///< PCM �Đ��p�f�o�C�X�N���X
	AudioDevice*			m_midiAudioDevice;	///< MIDI �Đ��p�f�o�C�X�N���X (���������� PCM �Ƃ͑S�R�Ⴄ�̂ŁA1�� AudioDevice �ɂ܂Ƃ߂Ȃ������Ǘ����y)
	GameAudioImpl*				m_gameAudio;
	uint32_t				mOnMemoryLimitSize;
	Threading::Mutex			m_resourceMutex;

	CacheManager*				m_audioStreamCache;

	Array<Sound*>				m_addingSoundList;
	Array<Sound*>				m_soundList;
	Threading::Mutex			m_soundListMutex;
	Threading::EventFlag		m_endRequested;
	Threading::DelegateThread	m_pollingThread;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END