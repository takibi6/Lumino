
#include "Internal.h"
#include <Lumino/UI/LayoutElement.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// ILayoutElement
//==============================================================================

//------------------------------------------------------------------------------
ILayoutElement::ILayoutElement()
{
}

//------------------------------------------------------------------------------
ILayoutElement::~ILayoutElement()
{
}

//------------------------------------------------------------------------------
void ILayoutElement::UpdateLayout(const Size& viewSize)
{
	const Size& itemSize = GetLayoutSize();
	Size size(
		Math::IsNaNOrInf(itemSize.width) ? viewSize.width : itemSize.width,
		Math::IsNaNOrInf(itemSize.height) ? viewSize.height : itemSize.height);

	// サイズが定まっていない場合はレイアウトを決定できない
	// TODO: 例外の方が良いかも？
	//if (Math::IsNaNOrInf(m_size.Width) || Math::IsNaNOrInf(m_size.Height)) { return; }

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.width, size.height));
	UpdateTransformHierarchy(RectF(0, 0, size));
}

//------------------------------------------------------------------------------
void ILayoutElement::MeasureLayout(const Size& availableSize)
{
	//// 無効情報フラグをこの要素に伝播させる
	//if (m_parent != nullptr)
	//{
	//	// フォントは MeasureOverride() の中で更新する
	//	m_invalidateFlags |= (m_parent->m_invalidateFlags & detail::InvalidateFlags::Font);
	//}

	// 親要素から子要素を配置できる範囲(availableSize)を受け取り、DesiredSize を更新する。
	// �@ Pane ―[measure()   … この範囲内なら配置できるよ]→ Button
	// �A Pane ←[DesiredSize … じゃあこのサイズでお願いします]― Button		※この時点で inf を返すこともあり得る。
	// �B Pane ―[arrange()   … 他の子要素との兼ね合いで最終サイズはコレで]→ Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

	// Margin を考慮する
	const ThicknessF& margin = GetLayoutMargin();
	float marginWidth = margin.Left + margin.Right;
	float marginHeight = margin.Top + margin.Bottom;
	Size localAvailableSize(
		std::max(availableSize.width - marginWidth, 0.0f),
		std::max(availableSize.height - marginHeight, 0.0f));

	Size desiredSize = MeasureOverride(localAvailableSize);

	// Margin を考慮する
	desiredSize.width += marginWidth;
	desiredSize.height += marginHeight;

	SetLayoutDesiredSize(desiredSize);
}

//------------------------------------------------------------------------------
void ILayoutElement::ArrangeLayout(const RectF& finalLocalRect)
{
	// finalLocalRect はこの要素を配置できる領域サイズ。と、親要素内でのオフセット。
	// 要素に直接設定されているサイズよりも大きいこともある。
	// TODO: HorizontalAlignment 等を考慮して、最終的な座標とサイズを決定する。
	//		 この要素のサイズが省略されていれば、Stretch ならサイズは最大に、それ以外なら最小になる。

	const Size& areaSize = finalLocalRect.GetSize();

	ILayoutElement* parent = GetLayoutParent();
	HAlignment		hAlign = GetLayoutHAlignment();
	VAlignment		vAlign = GetLayoutVAlignment();
	const HAlignment* parentHAlign = (parent != nullptr) ? parent->GetLayoutContentHAlignment() : nullptr;
	const VAlignment* parentVAlign = (parent != nullptr) ? parent->GetLayoutContentVAlignment() : nullptr;
	if (parentHAlign != nullptr) hAlign = *parentHAlign;
	if (parentVAlign != nullptr) vAlign = *parentVAlign;

	const Size& layoutSize = GetLayoutSize();
	Size ds;// = GetLayoutDesiredSize();
	ds.width = Math::IsNaNOrInf(layoutSize.width) ? finalLocalRect.width : layoutSize.width;
	ds.height = Math::IsNaNOrInf(layoutSize.height) ? finalLocalRect.height : layoutSize.height;

	// Alignment で調整する領域は、margin 領域も含む
	const ThicknessF& margin = GetLayoutMargin();
	float marginWidth = margin.Left + margin.Right;
	float marginHeight = margin.Top + margin.Bottom;
	ds.width += marginWidth;
	ds.height += marginHeight;

	RectF arrangeRect;
	detail::LayoutHelper::AdjustHorizontalAlignment(areaSize, ds, hAlign, &arrangeRect);
	detail::LayoutHelper::AdjustVerticalAlignment(areaSize, ds, vAlign, &arrangeRect);

	// Margin を考慮する (0 以下には出来ない)
	arrangeRect.width = std::max(arrangeRect.width - marginWidth, 0.0f);
	arrangeRect.height = std::max(arrangeRect.height - marginHeight, 0.0f);


	Size renderSize = ArrangeOverride(arrangeRect.GetSize());
	RectF thisFinalLocalRect;
	thisFinalLocalRect.x = finalLocalRect.x + margin.Left + arrangeRect.x;
	thisFinalLocalRect.y = finalLocalRect.y + margin.Top + arrangeRect.y;
	thisFinalLocalRect.width = renderSize.width;
	thisFinalLocalRect.height = renderSize.height;
	SetLayoutFinalLocalRect(thisFinalLocalRect);
}

//------------------------------------------------------------------------------
Size ILayoutElement::MeasureOverride(const Size& constraint)
{
	// 戻り値は、constraint の制限の中で、子要素をレイアウトするために必要な最小サイズ。
	// ユーザー指定のサイズがある場合はそれを返す。
	// ただし、constraint を超えることはできない。

	const Size& size = GetLayoutSize();
	Size desiredSize;
	// NaN の場合、この要素として必要な最小サイズは 0 となる。
	desiredSize.width = Math::IsNaNOrInf(size.width) ? 0.0f : size.width;
	desiredSize.height = Math::IsNaNOrInf(size.height) ? 0.0f : size.height;
	desiredSize.width = std::min(desiredSize.width, constraint.width);
	desiredSize.height = std::min(desiredSize.height, constraint.height);

	return desiredSize;
}

//------------------------------------------------------------------------------
Size ILayoutElement::ArrangeOverride(const Size& finalSize)
{
	return finalSize;
}

//------------------------------------------------------------------------------
void ILayoutElement::UpdateTransformHierarchy(const RectF& parentGlobalRect)
{
	const RectF& localRect = GetLayoutFinalLocalRect();
	RectF finalGlobalRect;
	//if (m_parent != nullptr)
	//{
	finalGlobalRect.x = parentGlobalRect.x + localRect.x;
	finalGlobalRect.y = parentGlobalRect.y + localRect.y;
		//m_combinedOpacity = m_parent->m_combinedOpacity * m_opacity;	// 不透明度もココで混ぜてしまう
	//}
	//else
	//{
	//	m_finalGlobalRect.x = m_finalLocalRect.x;
	//	m_finalGlobalRect.y = m_finalLocalRect.y;
	//	m_combinedOpacity = m_opacity;
	//}
	finalGlobalRect.width = localRect.width;
	finalGlobalRect.height = localRect.height;

	SetLayoutFinalGlobalRect(finalGlobalRect);

	// update children
	int childCount = GetVisualChildrenCount();
	for (int i = 0; i < childCount; i++)
	{
		ILayoutElement* child = GetVisualChild(i);
		child->UpdateTransformHierarchy(finalGlobalRect);
	}

	// 子要素
	//UIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->UpdateTransformHierarchy(); });

}

LN_NAMESPACE_END
