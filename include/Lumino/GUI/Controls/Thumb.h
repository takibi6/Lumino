
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		マウスドラッグが関係するイベント引数です。
*/
class DragEventArgs
	: public EventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	DragEventArgs(float xOffset, float yOffset) { XOffset = xOffset; YOffset = yOffset; }
	virtual ~DragEventArgs() {}

public:
	float XOffset;		///< ドラッグ開始点からの水平距離
	float YOffset;		///< ドラッグ開始点からの垂直距離
};

/**
	@brief
	@note		Thumb はマウスドラッグによる移動量を通知するためのコントロール。
				Thumb 自体の位置を移動するものではない点に注意。
				ScrollBar はもちろん、GridSpritter も Thumb のサブクラス。
*/
class Thumb
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Thumb);
public:
	static const Property*		IsDraggingProperty;		///< ドラッグ中であるかを示す値

	static const RoutedEvent*	DragStartedEvent;		///< ドラッグ開始イベント
	static const RoutedEvent*	DragDeltaEvent;			///< ドラッグ中の移動イベント
	static const RoutedEvent*	DragCompletedEvent;		///< ドラッグ終了イベント
	static const RoutedEvent*	DragCanceledEvent;		///< ドラッグキャンセルイベント

	// TODO:仮
	Event01<DragEventArgs*>	DragStarted;
	Event01<DragEventArgs*>	DragDelta;
	Event01<DragEventArgs*>	DragCompleted;
	Event01<DragEventArgs*>	DragCanceled;

public:
	static Thumb* Create(GUIManager* manager);

	Thumb(GUIManager* manager);
	virtual ~Thumb();

	/// ドラッグ中であるかを確認する
	bool IsDragging() const { return m_isDragging; }

	/// ドラッグをキャンセルする
	void CancelDrag();

protected:
	virtual void OnDragStarted(DragEventArgs* e)	{ if (!e->Handled) { RaiseEvent(DragStartedEvent, this, e); } }
	virtual void OnDragDelta(DragEventArgs* e)		{ if (!e->Handled) { RaiseEvent(DragDeltaEvent, this, e); } }
	virtual void OnDragCompleted(DragEventArgs* e)	{ if (!e->Handled) { RaiseEvent(DragCompletedEvent, this, e); } }
	virtual void OnDragCanceled(DragEventArgs* e)	{ if (!e->Handled) { RaiseEvent(DragCanceledEvent, this, e); } }

private:
	void Handler_MouseMove(MouseEventArgs* e);
	void Handler_MouseDown(MouseEventArgs* e);
	void Handler_MouseUp(MouseEventArgs* e);

private:
	PointF	m_lastScreenPosition;
	bool	m_isDragging;
};

/**
	@brief		Thumb の外観
*/
class ThumbChrome
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ThumbChrome);

public:
	static const Property*	RenderMouseOverProperty;
	static const Property*	RenderPressedProperty;

public:
	static ThumbChrome* Create(GUIManager* manager);

	ThumbChrome(GUIManager* manager);
	virtual ~ThumbChrome();

	void SetRenderMouseOver(bool enabled) { m_renderMouseOver = enabled; }
	bool GetRenderMouseOver() const { return m_renderMouseOver; }
	void SetRenderPressed(bool enabled) { m_renderPressed = enabled; }
	bool GetRenderPressed() const { return m_renderPressed; }

protected:
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	//virtual Size MeasureOverride(Size availableSize);	// 必要ないかも
	//virtual Size ArrangeOverride(Size finalSize);		// 必要ないかも
	virtual void OnRender(Graphics::Painter* painter);

private:
	RefPtr<Graphics::TextureBrush>	m_bgFrameBrush;
	RefPtr<Graphics::TextureBrush>	m_bgInnerBrush;

	bool	m_renderMouseOver;
	bool	m_renderPressed;
};

} // namespace GUI
} // namespace Lumino
