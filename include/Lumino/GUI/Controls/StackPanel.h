
#pragma once
#include "Panel.h"

namespace Lumino
{
namespace GUI
{



/**
	@brief		
*/
class StackPanel
	: public Panel
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(StackPanel);
public:
	static const String	OrientationProperty;

public:
	StackPanel(GUIManager* manager);
	virtual ~StackPanel();

public:
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }
	Orientation GetOrientation() const { return m_orientation; }

protected:
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);

private:
	Orientation	m_orientation;
};


} // namespace GUI
} // namespace Lumino