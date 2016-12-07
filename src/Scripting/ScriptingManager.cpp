
#include "../Internal.h"
#include "ScriptingManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// NlGraphNode
//==============================================================================

//------------------------------------------------------------------------------
NlGraphNode::NlGraphNode()
	: m_category(NlGraphNodeCategory::Command)
{
}

//------------------------------------------------------------------------------
void NlGraphNode::Initialize(NlGraphNodeCategory category)
{
	m_category = category;
}

//------------------------------------------------------------------------------
NlGraphPin* NlGraphNode::CreatePin(NlGraphPinCategory category, NlGraphPinDirection direction, const StringRef& name)
{
	if (category == NlGraphPinCategory::CommandFlow)
	{
		if (direction == NlGraphPinDirection::Input)
		{
			LN_FAIL_CHECK_STATE(m_inputCommandFlowPin == nullptr) return nullptr;
			m_inputCommandFlowPin = NlGraphPinPtr::MakeRef();
			m_inputCommandFlowPin->Initialize(this, category, direction);
			return m_inputCommandFlowPin;
		}
		else if (direction == NlGraphPinDirection::Output)
		{
			auto ptr = NlGraphPinPtr::MakeRef();
			ptr->Initialize(this, category, direction);
			m_outputCommandFlowPinList.Add(ptr);
			return ptr;
		}
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}

	return nullptr;
}

//==============================================================================
// EntryPointNode
//==============================================================================

//------------------------------------------------------------------------------
EntryPointNode::EntryPointNode()
{
}

//------------------------------------------------------------------------------
void EntryPointNode::Initialize(const StringRef& name)
{
	m_flowOutput = CreatePin(NlGraphPinCategory::CommandFlow, NlGraphPinDirection::Output, _T("name"));
}

//------------------------------------------------------------------------------
void EntryPointNode::Execute(NlContext* sc)
{
	sc->Goto(m_flowOutput);
}

//==============================================================================
// NlGraphPin
//==============================================================================

//------------------------------------------------------------------------------
NlGraphPin::NlGraphPin()
	: m_ownerNode(nullptr)
	, m_category(NlGraphPinCategory::CommandFlow)
	, m_direction(NlGraphPinDirection::Input)
{
}

//------------------------------------------------------------------------------
void NlGraphPin::Initialize(NlGraphNode* ownerNode, NlGraphPinCategory category, NlGraphPinDirection direction)
{
	m_ownerNode = ownerNode;
	m_category = category;
	m_direction = direction;
}

//------------------------------------------------------------------------------
NlGraphNode* NlGraphPin::GetLinkedToNode()
{
	LN_FAIL_CHECK_STATE(m_linkedTo.GetCount() <= 1) return nullptr;
	if (m_linkedTo.IsEmpty()) return nullptr;
	return m_linkedTo[0]->GetOwnerNode();
}

//------------------------------------------------------------------------------
void NlGraphPin::MakeLinkTo(NlGraphPin* toPin)
{
	LN_FAIL_CHECK_ARG(toPin != nullptr) return;
	LN_FAIL_CHECK_STATE(!m_linkedTo.Contains(toPin)) return;
	LN_FAIL_CHECK_STATE(!toPin->m_linkedTo.Contains(this)) return;

	// 双方向接続
	m_linkedTo.Add(toPin);
	toPin->m_linkedTo.Add(this);
}

//------------------------------------------------------------------------------
void NlGraphPin::BreakLinkTo(NlGraphPin* toPin)
{
	LN_FAIL_CHECK_ARG(toPin != nullptr) return;

	m_linkedTo.Remove(toPin);
	toPin->m_linkedTo.Remove(this);
}

//==============================================================================
// NlContext
//==============================================================================

//------------------------------------------------------------------------------
NlContext::NlContext()
	: m_pc(nullptr)
	, m_lastExecutePC(nullptr)
{
}

//------------------------------------------------------------------------------
void NlContext::CallInterface(NlGraphInterface* inter)
{
	GotoNode(inter->GetEntryPoint());

	while (!IsCompleted())
	{
		Step();
	}
}

//------------------------------------------------------------------------------
void NlContext::Goto(NlGraphPin* nextFlowPin)
{
	LN_FAIL_CHECK_ARG(nextFlowPin != nullptr) return;

	m_pc = nullptr;
	NlGraphNode* next = nextFlowPin->GetLinkedToNode();

	// 接続先 pin が無ければ nullptr
	GotoNode(next);
}

//------------------------------------------------------------------------------
bool NlContext::IsCompleted() const
{
	return m_pc == nullptr;
}

//------------------------------------------------------------------------------
void NlContext::GotoNode(NlGraphNode* next)
{
	if (next != nullptr)
	{
		LN_FAIL_CHECK_ARG(next->GetCategory() == NlGraphNodeCategory::Command)
		{
			m_pc = nullptr;
			return;
		}
	}

	m_pc = next;
}

//------------------------------------------------------------------------------
void NlContext::Step()
{
	LN_FAIL_CHECK_STATE(m_pc != nullptr) return;
	m_pc->Execute(this);
}

//==============================================================================
// NlGraph
//==============================================================================

//------------------------------------------------------------------------------
void NlGraph::AddGraphNode(NlGraphNode* node)
{
	LN_FAIL_CHECK_ARG(node != nullptr) return;
	m_nodeList.Add(node);
}

//==============================================================================
// NlGraphInterface
//==============================================================================

//------------------------------------------------------------------------------
NlGraphInterface::NlGraphInterface()
	: m_entryPoint(nullptr)
{
}

//------------------------------------------------------------------------------
void NlGraphInterface::Initialize()
{
	m_graph = NlGraphPtr::MakeRef();

	// 1つのエントリーポイントは必ず存在している
	auto ep = RefPtr<EntryPointNode>::MakeRef();
	ep->Initialize(_T(""));
	m_graph->AddGraphNode(ep);
	m_entryPoint = ep;
}




//==============================================================================
// NlNode_Print
//==============================================================================

//------------------------------------------------------------------------------
void NlNode_Print::Execute(NlContext* sc)
{
	printf("test\n");

	sc->Goto(GetFlowOutputPin());
}


namespace detail {

//==============================================================================
// ScriptingManager
//==============================================================================

//------------------------------------------------------------------------------
ScriptingManager::ScriptingManager()
{
}

//------------------------------------------------------------------------------
ScriptingManager::~ScriptingManager()
{
}

//------------------------------------------------------------------------------
void ScriptingManager::Initialize(const ConfigData& configData)
{
}


} // namespace detail
LN_NAMESPACE_END
