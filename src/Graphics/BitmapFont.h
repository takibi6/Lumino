﻿
#pragma once
#include <Lumino/Graphics/Font.h>
#include <Lumino/Graphics/FontManager.h>

namespace Lumino
{
namespace Graphics
{
	
/**
	@brief		ビットマップフォントのクラスです。
*/
class BitmapFont
    : public Font
{
public:
	BitmapFont(FontManager* manager, Bitmap* bitmap);
	virtual ~BitmapFont();

public:
	virtual void SetName(const String& fontName) { LN_THROW(0, InvalidOperationException); }
	virtual const String& GetName() const { return m_name; }
	virtual void SetSize(int size) { LN_THROW(0, InvalidOperationException); }
	virtual int GetSize() const { return 0; }
	virtual void SetEdgeSize(int size) { LN_THROW(0, InvalidOperationException); }
	virtual int GetEdgeSize() const { return 0; }
	virtual void SetBold(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool IsBold() const { return false; }
	virtual void SetItalic(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool IsItalic() const { return false; }
	virtual void SetAntiAlias(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool IsAntiAlias() const { return false; }
	virtual Font* Copy() const;
	virtual int GetLineHeight() { return m_charHeight; }
	virtual FontGlyphLocation* AdvanceKerning(UTF32 utf32code, FontGlyphLocation* prevData);
	virtual FontGlyphBitmap* LookupGlyphBitmap(UTF32 utf32code);
	virtual FontManager* GetManager() const { return m_manager; }

private:
    void Dispose();

private:
	FontManager*			m_manager;
	String					m_name;					///< 名前。グリフテクスチャのキャッシュで使用する
	RefPtr<Bitmap>			m_fontBitmap;
	int						m_charWidth;			///< 1文字分の幅
	int						m_charHeight;		///< 1文字分の高さ
	FontGlyphLocation		m_fontGlyphLocation;	///< AdvanceKerning() で返すデータ
	RefPtr<Bitmap>			m_glyphBitmap;
	FontGlyphBitmap			m_fontGlyphBitmap;		///< LookupGlyphBitmap() で返すデータ
};

} // namespace Graphics
} // namespace Lumino