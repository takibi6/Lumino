﻿
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class ShaderVariable;

/**
	@brief	グラフィックスモジュールの基本的なリソースオブジェクトのベースクラスです。デバイスの状態変化を通知する機能を実装します。
*/
class GraphicsResourceObject
	: public tr::ReflectionObject
{
protected:
	friend class detail::GraphicsManager;
	friend class ShaderVariable;

	void Initialize(detail::GraphicsManager* manager);
	void Finalize();

	virtual void ApplyModifies();

	// デバイスが変更される場合、まずは NULL が渡されて呼ばれる。このとき、必要なリソースを保存する。
	// 次に新しいデバイスが渡されて呼ばれる。このとき、保存したリソースをデバイスオブジェクトにロードする。
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device) = 0;
	
	GraphicsResourceObject();
	virtual ~GraphicsResourceObject();


	detail::GraphicsManager*	m_manager;
};

LN_NAMESPACE_END
