/*
・ソフトウェア描画
BitmapFont
OutlineFont
・ハードウェア描画
PolygonFont		… OutlineFont の頂点情報を使う。まはた BitmapFont+矩形
TextureFont		… OutlineFont または BitmapFont のレンダリング結果を使う

■ カーニングについて
GUI::RichText で書くときも、Span をまたいでもカーニングは維持される。
→ <red>i</red><blue>j</blue> とか。

カーニングの量はFontクラスからとるけど、GUI レベルではアラビア語とかは
TextBox.RightToLeft プロパティによって描画方向が変わる。

GUI の FlowDocument(Span) レベルでカーニングは使えるようにしたい。
別にカーニングを見せなくても良いが・・・。

textRenderer.BeginLine()
for (span, spans) {
textRenderer.DrawLine(span.Text, span.Font, span.pen, span.blush);
}
textRenderer.EndLine()

↑この textRenderer は GUI で描く Renderer。

■ 文字単位のアニメーションとかのエフェクトは？
AfterEffects は、"テキストレイヤー" なるものに "エフェクト" をアタッチすることで行う。
テキストレイヤーは文字の描画に TextRenderer を使用するが、位置は自分で決める。
1文字ごとに頂点バッファを用意するか、メッシュみたいに 1つの頂点バッファを Subset で分割するとかやりざまは考える必要がありそう。

・スパイラルダウン
・ブラー
→ かなり高レベルな部分の機能と関係する。メッシュと同じ扱いにしたほうが良いかも。
*/
#include "../Internal.h"
#include "FontManagerImpl.h"

namespace Lumino
{
namespace Imaging
{

//=============================================================================
// FontManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager* FontManager::Create(FileManager* fileManager)
{
	return LN_NEW FontManagerImpl(fileManager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager::FontManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager::~FontManager()
{
}

//=============================================================================
// FontManagerImpl
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManagerImpl::FontManagerImpl(FileManager* fileManager)
	: m_fileManager(fileManager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManagerImpl::~FontManagerImpl()
{
}

} // namespace Imaging
} // namespace Lumino
