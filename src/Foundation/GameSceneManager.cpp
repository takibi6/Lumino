﻿
#include "../Internal.h"
#include <Lumino/Foundation/GameScene.h>
#include "GameSceneManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// GameObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GameObject, Object);

//------------------------------------------------------------------------------
GameObjectPtr GameObject::Create()
{
	auto ptr = GameObjectPtr::MakeRef();
	detail::GameSceneManager::GetInstance()->GetActiveScene()->AddGameObject(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
GameObject::GameObject()
{
	printf("const \n");
}

//------------------------------------------------------------------------------
GameObject::~GameObject()
{
	printf("~ \n");
}

//------------------------------------------------------------------------------
void GameObject::AddComponent(Component* component)
{
	LN_FAIL_CHECK_ARG(component != nullptr) return;
	LN_FAIL_CHECK_ARG(component->m_owner == nullptr) return;
	component->Attach(this);
	m_components.Add(component);
}

//------------------------------------------------------------------------------
void GameObject::OnStart()
{
}

//------------------------------------------------------------------------------
void GameObject::OnUpdate()
{
	for (auto& c : m_components)
		c->OnUpdate();
}

//------------------------------------------------------------------------------
void GameObject::OnDestroy()
{
}

//------------------------------------------------------------------------------
void GameObject::ReleaseComponents()
{
	for (auto& c : m_components)
		c->Detach();
	m_components.Clear();
}

//==============================================================================
// GameScene
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GameScene, Object);

//------------------------------------------------------------------------------
GameScene::GameScene()
{
}

//------------------------------------------------------------------------------
GameScene::~GameScene()
{
}

//------------------------------------------------------------------------------
void GameScene::OnStart()
{
}

//------------------------------------------------------------------------------
void GameScene::OnUpdate()
{
	for (auto& ptr : m_gameObjectList)
	{
		ptr->OnUpdate();
	}
}

//------------------------------------------------------------------------------
void GameScene::OnTerminate()
{
}

//------------------------------------------------------------------------------
void GameScene::AddGameObject(GameObject* obj)
{
	LN_FAIL_CHECK_ARG(obj != nullptr) return;
	m_gameObjectList.Add(obj);
}

//------------------------------------------------------------------------------
void GameScene::Update()
{

	OnUpdate();
	//tr::ReflectionHelper::GCObjectList(&m_gameObjectList);
}


//==============================================================================
// GameSceneManager
//==============================================================================
namespace detail {

static GameSceneManager* g_gameSceneManager;

//------------------------------------------------------------------------------
GameSceneManager* GameSceneManager::GetInstance(GameSceneManager* priority)
{
	return (priority != nullptr) ? priority : g_gameSceneManager;
}

//------------------------------------------------------------------------------
GameSceneManager::GameSceneManager()
{
	g_gameSceneManager = this;
}

//------------------------------------------------------------------------------
GameSceneManager::~GameSceneManager()
{
	g_gameSceneManager = nullptr;
}

//------------------------------------------------------------------------------
void GameSceneManager::Finalize()
{
	ReleaseAndTerminateAllRunningScenes();
}

//------------------------------------------------------------------------------
void GameSceneManager::GotoScene(GameScene* scene)
{
	LN_FAIL_CHECK_ARG(scene != nullptr) return;
	EventCommsnd c;
	c.type = EventType::Goto;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

//------------------------------------------------------------------------------
void GameSceneManager::CallScene(GameScene* scene)
{
	LN_FAIL_CHECK_ARG(scene != nullptr) return;
	EventCommsnd c;
	c.type = EventType::Call;
	c.scene = scene;
	m_eventQueue.push_back(c);
}

//------------------------------------------------------------------------------
void GameSceneManager::ReturnScene()
{
	LN_FAIL_CHECK_STATE(m_activeScene != nullptr) return;
	EventCommsnd c;
	c.type = EventType::Return;
	c.scene = nullptr;
	m_eventQueue.push_back(c);
}

//------------------------------------------------------------------------------
void GameSceneManager::UpdateFrame()
{
	ExecuteCommands();
	if (m_activeScene != nullptr)
	{
		m_activeScene->Update();
	}
}

//------------------------------------------------------------------------------
void GameSceneManager::ExecuteCommands()
{
	while (!m_eventQueue.empty())
	{
		const EventCommsnd& cmd = m_eventQueue.front();

		switch (cmd.type)
		{
			/////////////// 直接遷移
			case EventType::Goto:
			{
				// 現在の全てのシーンを解放 (OnTerminate() 呼び出し)
				ReleaseAndTerminateAllRunningScenes();

				m_activeScene = cmd.scene;
				if (m_activeScene != nullptr)
				{
					m_activeScene->OnStart();
				}
				break;
			}
			/////////////// 呼び出し
			case EventType::Call:
			{
				m_sceneStack.push(m_activeScene);
				m_activeScene = cmd.scene;
				m_activeScene->OnStart();
				break;
			}
			/////////////// 呼び出し元へ戻る
			case EventType::Return:
			{
				RefPtr<GameScene> oldScene = m_activeScene;
				m_activeScene = m_sceneStack.top();
				oldScene->OnTerminate();
				break;
			}
		}

		m_eventQueue.pop_front();
	}
}

//------------------------------------------------------------------------------
void GameSceneManager::ReleaseAndTerminateAllRunningScenes()
{
	if (m_activeScene != nullptr)
	{
		m_activeScene->OnTerminate();
		m_activeScene = nullptr;
	}

	while (!m_sceneStack.empty())
	{
		m_sceneStack.top()->OnTerminate();
		m_sceneStack.pop();
	}
}

} // namespace detail
LN_NAMESPACE_END
