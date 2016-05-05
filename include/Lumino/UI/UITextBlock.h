
#pragma once
#include "UITextElement.h"

LN_NAMESPACE_BEGIN
class UITextBlock;
using UITextBlockPtr = RefPtr<UITextBlock>;

/**
	@brief		
*/
class UITextBlock
	: public UITextElement
{
	LN_UI_TYPEINFO_DECLARE();
public:

	static UITextBlockPtr Create();

public:
	void SetText(const StringRef& text);


LN_PROTECTED_INTERNAL_ACCESS:
	UITextBlock();
	virtual ~UITextBlock();
	void Initialize(detail::UIManager* manager);

	virtual SizeF MeasureOverride(const SizeF& availableSize) override;
	virtual void OnRender(GraphicsContext* g) override;

private:
	String	m_text;
};

LN_NAMESPACE_END