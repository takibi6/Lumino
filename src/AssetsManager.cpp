﻿
#include "Internal.h"
#include <Lumino/Assets.h>
#include "Graphics/GraphicsManager.h"
#include "Graphics/Text/FreeTypeFont.h"
#include "EngineManager.h"
#include "AssetsManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// AssetsManager
//==============================================================================

static AssetsManager* g_managerInstance = nullptr;

//------------------------------------------------------------------------------
AssetsManager* AssetsManager::GetInstance(AssetsManager* priority)
{
	return (priority == nullptr) ? g_managerInstance : priority;
}

//------------------------------------------------------------------------------
AssetsManager::AssetsManager()
	: m_engineManager(nullptr)
{
}

//------------------------------------------------------------------------------
AssetsManager::~AssetsManager()
{
	if (g_managerInstance == this)
	{
		g_managerInstance = nullptr;
	}
}

//------------------------------------------------------------------------------
void AssetsManager::Initialize(EngineManager* manager)
{
	m_engineManager = manager;

	m_textureCache = RefPtr<CacheManager>::MakeRef(32, 0);	// TODO
	m_fontCache = RefPtr<CacheManager>::MakeRef(32, 0);	// TODO

	if (g_managerInstance == nullptr)
	{
		g_managerInstance = this;
	}
}

//------------------------------------------------------------------------------
void AssetsManager::Finalize()
{
	m_textureCache->Finalize();
	m_fontCache->Finalize();
}

//------------------------------------------------------------------------------
Texture2DPtr AssetsManager::LoadTexture(const StringRef& filePath)
{
	CacheKey key(filePath);
	Texture2D* ptr = static_cast<Texture2D*>(m_textureCache->FindObjectAddRef(key));
	if (ptr != nullptr) { return Texture2DPtr(ptr, false); }

	// TODO: mipmap ON にしたい。ただ、サンプラステートを変更しないとならないようなので動作確認してから。
	Texture2DPtr ref = Texture2DPtr::MakeRef();
	ref->Initialize(m_engineManager->GetGraphicsManager(), filePath, TextureFormat::R8G8B8A8, false);

	m_textureCache->RegisterCacheObject(key, ref);
	return ref;
}

//------------------------------------------------------------------------------
//RawFontPtr AssetsManager::LoadFont(const StringRef& name, int size, bool isBold, bool isItalic, bool isAntiAlias)
//{
//	CacheKey key(String::Format(_T("{0}-{1}{2}{3}{4}"), name, size, isBold, isItalic, isAntiAlias));
//
//	RawFont* ptr = static_cast<RawFont*>(m_fontCache->FindObjectAddRef(key));
//	if (ptr != nullptr) { return RawFontPtr(ptr, false); }
//
//	RawFontPtr ref;
//	if (name.IsEmpty())
//	{
//		ref = m_engineManager->GetGraphicsManager()->GetFontManager()->GetDefaultFont()->Copy();
//	}
//	else
//	{
//		auto ftFont = RefPtr<FreeTypeFont>::MakeRef(m_engineManager->GetGraphicsManager()->GetFontManager());
//		ref = ftFont;
//	}
//
//	ref->SetName(name);
//	ref->SetSize(size);
//	ref->SetBold(isBold);
//	ref->SetItalic(isItalic);
//	ref->SetAntiAlias(isAntiAlias);
//
//	m_fontCache->RegisterCacheObject(key, ref);
//	return RawFontPtr::StaticCast(ref);
//}

//==============================================================================
// Assets
//==============================================================================

//------------------------------------------------------------------------------
Texture2DPtr Assets::LoadTexture(const StringRef& filePath)
{
	return AssetsManager::GetInstance()->LoadTexture(filePath);
}

LN_NAMESPACE_END
