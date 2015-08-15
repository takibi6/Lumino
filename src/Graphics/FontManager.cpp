/*
	FreeType �߂�

	�EFT_CharMap	http://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_CharMap
		�����ƃO���t��Ή��t����}�b�v�B
		1�� FT_Face �͕����̃L�����}�b�v�������Ƃ�����B
		�ʏ�͂��̒���1���g�p���A������u�A�N�e�B�u�ȃL�����}�b�v�v�ƌĂ�ł���B
		�A�N�e�B�u�ȃL�����}�b�v�� face->charmap �Ŏ擾�ł���B

	�� �c�����t�H���g�ɂ���
		FT_Load_Glyph() ����Ƃ��̃t���O�� FT_LOAD_VERTICAL_LAYOUT ���w�肷��ƁA
		slot->advance.y �� slot->metrics.vertAdvance ���i�[�����悤�ɂȂ�B
		ftobjs.c �� 758 �s�ڂ����肪�Q�l�ɂȂ�B
*/
/*
�E�\�t�g�E�F�A�`��
BitmapFont
OutlineFont
�E�n�[�h�E�F�A�`��
PolygonFont		�c OutlineFont �̒��_�����g���B�܂͂� BitmapFont+��`
TextureFont		�c OutlineFont �܂��� BitmapFont �̃����_�����O���ʂ��g��

�� �J�[�j���O�ɂ���
GUI::RichText �ŏ����Ƃ����ASpan ���܂����ł��J�[�j���O�͈ێ������B
�� <red>i</red><blue>j</blue> �Ƃ��B

�J�[�j���O�̗ʂ�Font�N���X����Ƃ邯�ǁAGUI ���x���ł̓A���r�A��Ƃ���
TextBox.RightToLeft �v���p�e�B�ɂ���ĕ`��������ς��B

GUI �� FlowDocument(Span) ���x���ŃJ�[�j���O�͎g����悤�ɂ������B
�ʂɃJ�[�j���O�������Ȃ��Ă��ǂ����E�E�E�B

textRenderer.BeginLine()
for (span, spans) {
textRenderer.DrawLine(span.Text, span.Font, span.pen, span.blush);
}
textRenderer.EndLine()

������ textRenderer �� GUI �ŕ`�� Renderer�B

�� �����P�ʂ̃A�j���[�V�����Ƃ��̃G�t�F�N�g�́H
AfterEffects �́A"�e�L�X�g���C���[" �Ȃ���̂� "�G�t�F�N�g" ���A�^�b�`���邱�Ƃōs���B
�e�L�X�g���C���[�͕����̕`��� TextRenderer ���g�p���邪�A�ʒu�͎����Ō��߂�B
1�������Ƃɒ��_�o�b�t�@��p�ӂ��邩�A���b�V���݂����� 1�̒��_�o�b�t�@�� Subset �ŕ�������Ƃ���肴�܂͍l����K�v�����肻���B

�E�X�p�C�����_�E��
�E�u���[
�� ���Ȃ荂���x���ȕ����̋@�\�Ɗ֌W����B���b�V���Ɠ��������ɂ����ق����ǂ������B
*/
#include "../Internal.h"
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include <Lumino/Base/Hash.h>
#include <Lumino/Graphics/Font.h>
#include <Lumino/Graphics/FontManager.h>
#include "FreeTypeFont.h"

namespace Lumino
{
namespace Graphics
{
	
//=============================================================================
// FontManager
//=============================================================================

static const TCHAR* DefaultFontName = _T("MS PGothic");

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager* FontManager::Create(FileManager* fileManager)
{
	return LN_NEW FontManager(fileManager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager::FontManager(FileManager* fileManager)
{
	m_fileManager = fileManager;
	m_charToUTF32Converter.SetDestinationEncoding(Text::Encoding::GetUTF32Encoding());
	m_charToUTF32Converter.SetSourceEncoding(Text::Encoding::GetSystemMultiByteEncoding());
	m_wcharToUTF32Converter.SetDestinationEncoding(Text::Encoding::GetUTF32Encoding());
	m_wcharToUTF32Converter.SetSourceEncoding(Text::Encoding::GetWideCharEncoding());

	m_TCharToUTF32Converter.SetDestinationEncoding(Text::Encoding::GetUTF32Encoding());
	m_TCharToUTF32Converter.SetSourceEncoding(Text::Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetDestinationEncoding(Text::Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetSourceEncoding(Text::Encoding::GetUTF32Encoding());

	// FreeType ������
	FT_Error err = FT_Init_FreeType(&m_ftLibrary);
	LN_THROW(err == 0, InvalidOperationException, "failed initialize font library : %d\n", err);

	// �L���b�V�}�l�[�W��
	err = FTC_Manager_New(
		m_ftLibrary,
		0, 0, 0,
		CallbackFaceRequester,
		this,
		&m_ftCacheManager);
	LN_THROW(err == 0, InvalidOperationException, "failed initialize font cache manager : %d\n", err);

	// �L���b�V���}�b�v
	err = FTC_CMapCache_New(m_ftCacheManager, &m_ftCMapCache);
	LN_THROW(err == 0, InvalidOperationException, "failed initialize font cache map : %d\n", err);

	// �C���[�W�L���b�V��
	err = FTC_ImageCache_New(m_ftCacheManager, &m_ftImageCache);
	LN_THROW(err == 0, InvalidOperationException, "failed initialize font image cache : %d\n", err);

	// �f�t�H���g�t�H���g
	m_defaultFont = LN_NEW FreeTypeFont(this);
	m_defaultFont->SetName(DefaultFontName);
	m_defaultFont->SetSize(20);
	m_defaultFont->SetAntiAlias(true);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager::~FontManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FontManager::Dispose()
{
	LN_SAFE_RELEASE(m_defaultFont);

	// �o�^����TTF�t�@�C���̃������o�b�t�@�����ׂĉ��
	//TTFDataEntryMap::iterator itr = m_ttfDataEntryMap.begin();
	//for (; itr != m_ttfDataEntryMap.end(); ++itr)
	//{
	//	LN_SAFE_RELEASE(itr->second.DataBuffer);
	//}
	m_ttfDataEntryMap.clear();

	// �L���b�V���}�l�[�W��
	if (m_ftCacheManager != NULL)
	{
		FTC_Manager_Done(m_ftCacheManager);
		m_ftCacheManager = NULL;
	}

	// FreeType
	if (m_ftLibrary != NULL)
	{
		FT_Done_FreeType(m_ftLibrary);
		m_ftLibrary = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FontManager::RegisterFontFile(const String& fontFilePath)
{
	// �t�@�C������S�Ẵf�[�^��ǂݍ���
	RefPtr<Stream> file(m_fileManager->CreateFileStream(fontFilePath));
	ByteBuffer buffer((size_t)file->GetLength(), false);
	file->Read(buffer.GetData(), buffer.GetSize());

	// Face �쐬 (�t�@�~�����EFace ���𒲂ׂ邽�߁B�����폜����)
	FT_Face face;
	FT_Error err = FT_New_Memory_Face(
		m_ftLibrary,
		(const FT_Byte*)buffer.GetData(),
		buffer.GetSize(),
		0,
		&face);
	LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_New_Memory_Face : %d\n", err);

	// Fase �ЂƂ��� (.ttf)
	if (face->num_faces == 1)
	{
		String familyName(face->family_name);
		uint32_t key = Hash::CalcHash(familyName);
		if (m_ttfDataEntryMap.find(key) == m_ttfDataEntryMap.end())
		{
			TTFDataEntry e;
			e.DataBuffer = buffer;
//			e.DataBuffer->AddRef();
			e.CollectionIndex = 0;
			m_ttfDataEntryMap.insert(TTFDataEntryPair(key, e));

			Logger::WriteLine("Registered font file. [%s]", face->family_name);

			// ����o�^�̏ꍇ�̓f�t�H���g�t�H���g���Ƃ��ēo�^����
			if (m_ttfDataEntryMap.size() == 1) {
				m_defaultFont->SetName(familyName);
			}
		}
		FT_Done_Face(face);

	}
	// Fase ������ (.ttc)
	else if (face->num_faces > 1)
	{
		int facesCount = face->num_faces;
		FT_Done_Face(face);	// ���𒲂ׂ邽�߂����Ɏg�����B��x�폜
		face = NULL;

		for (int i = 0; i < facesCount; i++)
		{
			err = FT_New_Memory_Face(
				m_ftLibrary,
				(const FT_Byte*)buffer.GetData(),
				buffer.GetSize(),
				i,
				&face);
			LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_New_Memory_Face : %d\n", err);

			String familyName(face->family_name);
			uint32_t key = Hash::CalcHash(familyName);
			if (m_ttfDataEntryMap.find(key) == m_ttfDataEntryMap.end())
			{
				TTFDataEntry e;
				e.DataBuffer = buffer;
//				e.DataBuffer->AddRef();
				e.CollectionIndex = i;
				m_ttfDataEntryMap.insert(TTFDataEntryPair(key, e));

				Logger::WriteLine("Registered font file. [%s]", face->family_name);

				// ����o�^�̏ꍇ�̓f�t�H���g�t�H���g���Ƃ��ēo�^����
				if (m_ttfDataEntryMap.size() == 1) {
					m_defaultFont->SetName(familyName);
				}
			}
			FT_Done_Face(face);
		}
	}
	else {
		FT_Done_Face(face);
		LN_THROW(0, InvalidOperationException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FontManager::SetDefaultFont(Font* font)
{
	LN_REFOBJ_SET(m_defaultFont, font);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FT_Error FontManager::FaceRequester(
	FTC_FaceID face_id,
	FT_Library library,
	FT_Pointer request_data,
	FT_Face* aface)
{
	// �L���b�V�������̃R�[���o�b�N�Bmap ������悤�ɂ��Ă���B

	// face_id �́AFTC_Manager_LookupFace() �ɓn���� ID �������Ă���B
	// ID �͎����ōD���Ȃ悤�Ɍ��߂���B�Ƃ肠���������� (�t�@�~����)�Ƃ��Ă���

	//String family_name( (lnChar*)face_id );
	uint32_t key = (uint32_t)face_id;
	TTFDataEntryMap::iterator itr = m_ttfDataEntryMap.find(key/*family_name*/);
	if (itr != m_ttfDataEntryMap.end())
	{
		FT_Face face;
		FT_Error err = FT_New_Memory_Face(
			m_ftLibrary,
			(const FT_Byte*)itr->second.DataBuffer.GetConstData(),
			itr->second.DataBuffer.GetSize(),
			itr->second.CollectionIndex,
			&face);
		LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_New_Memory_Face : %d\n", err);

		err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
		if (err != FT_Err_Ok)
		{
			FT_Done_Face(face);
			LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_Select_Charmap : %d\n", err);
		}

		*aface = face;
		return FT_Err_Ok;
	}
#ifdef LN_WIN32
	else if (m_requesterFaceName != NULL)
	{
		// ���O����V�X�e���t�H���g����
		TSystemFontData* systemFont = GetWindowsSystemFontData(m_requesterFaceName);
		m_requesterFaceName = NULL;
		if (systemFont == NULL){
			return FT_Err_Cannot_Open_Resource;
		}

		// ���\�[�X���b�N
		size_t size = 0;
		int index = 0;
		byte_t* data = LockWindowsSystemFontData(systemFont, &size, &index);
		if (data == NULL){
			FreeWindowsSystemFontData(systemFont);
			return FT_Err_Cannot_Open_Resource;
		}

		// FreeType �̓ǂݎ��X�g���[��
		FT_Stream stream = (FT_Stream)malloc(sizeof(FT_StreamRec));
		if (stream == NULL){
			FreeWindowsSystemFontData(systemFont);
			return FT_Err_Out_Of_Memory;
		}
		memset(stream, 0, sizeof(FT_StreamRec));
		stream->base = 0;
		stream->size = size;
		stream->pos = 0;
		stream->descriptor.pointer = systemFont;
		stream->pathname.pointer = NULL;
		stream->read = StreamIoFunc;
		stream->close = StreamCloseFunc;

		FT_Open_Args args;
		memset(&args, 0, sizeof(args));
		args.flags = FT_OPEN_STREAM;
		args.stream = stream;
		args.driver = 0;
		args.num_params = 0;
		args.params = NULL;

		FT_Face face;
		FT_Error err = FT_Open_Face(m_ftLibrary, &args, index, &face);
		if (err != FT_Err_Ok) {
			// ���s�����ꍇ��stream�͏���ɉ�������
			return err;
		}

		// Charmap��ݒ肵�Ă���
		err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
		if (err != FT_Err_Ok) {
			FT_Done_Face(face);
			return err;
		}

		*aface = (FT_Face)face;
		return FT_Err_Ok;
	}
#endif
	return -1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FT_Error FontManager::CallbackFaceRequester(
	FTC_FaceID face_id,
	FT_Library library,
	FT_Pointer request_data,
	FT_Face* aface)
{
	return ((FontManager*)request_data)->FaceRequester(face_id, library, request_data, aface);
}


#ifdef LN_WIN32
// http://kikyou.info/diary/?200510#i10 ���Q�l�ɂ���
#define TVP_TT_TABLE_ttcf  (('t' << 0) + ('t' << 8) + ('c' << 16) + ('f' << 24))
#define TVP_TT_TABLE_name  (('n' << 0) + ('a' << 8) + ('m' << 16) + ('e' << 24))

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager::TSystemFontData* FontManager::GetWindowsSystemFontData(LPCTSTR name)
{
	DWORD result;
	TSystemFontData *p = (TSystemFontData*)malloc(sizeof(TSystemFontData));
	if (p == NULL){
		return NULL;
	}
	p->fontdata = NULL;
	p->index = 0;
	p->hdc = GetDC(NULL);
	if (p->hdc == NULL){
		free(p);
		return NULL;
	}
	// ���O�ȊO�K��
	p->hFont = CreateFont(
		12, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		name);
	if (p->hFont == NULL){
		ReleaseDC(NULL, p->hdc);
		free(p);
		return NULL;
	}
	p->hOldFont = (HFONT)::SelectObject(p->hdc, p->hFont);
	// �t�H���g�f�[�^������ꂻ�����`�F�b�N
	result = ::GetFontData(p->hdc, TVP_TT_TABLE_name, 0, NULL, 0);
	if (result == GDI_ERROR){
		SelectObject(p->hdc, p->hOldFont);
		DeleteObject(p->hFont);
		ReleaseDC(NULL, p->hdc);
		free(p);
		return NULL;
	}
	return p;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
unsigned char* FontManager::LockWindowsSystemFontData(TSystemFontData *fnt, size_t *size, int *index)
{
	unsigned char *name_content = NULL; // Windows ����擾���� name �^�O�̓��e
	unsigned char *name_content_ft = NULL; // FreeType ����擾���� name �^�O�̓��e
	DWORD result;

	DWORD fontsize;
	BOOL IsTTC = FALSE;
	unsigned char buf[4];
	int name_content_size;

	FT_Face face = NULL;

	int i;

	if (fnt == NULL || index == NULL){
		return NULL;
	}
	if (fnt->fontdata != NULL){
		*size = fnt->size;
		*index = fnt->index;
		return fnt->fontdata;
	}
	// �t�H���g���̎擾����
	result = GetFontData(fnt->hdc, TVP_TT_TABLE_name, 0, NULL, 0);
	if (result == GDI_ERROR){
		// �G���[; GetFontData �ł͈����Ȃ�����
		return NULL;
	}
	name_content_size = result;
	name_content = (unsigned char*)malloc(name_content_size);
	name_content_ft = (unsigned char*)malloc(name_content_size);
	GetFontData(fnt->hdc, TVP_TT_TABLE_name, 0, name_content, name_content_size);

	// �t�H���g�T�C�Y�擾����
	result = GetFontData(fnt->hdc, TVP_TT_TABLE_ttcf, 0, &buf, 1);
	if (result == 1){
		// TTC �t�@�C�����Ǝv����
		result = GetFontData(fnt->hdc, TVP_TT_TABLE_ttcf, 0, NULL, 0);
		IsTTC = TRUE;
	}
	else{
		result = GetFontData(fnt->hdc, 0, 0, NULL, 0);
		fnt->index = 0;
	}
	if (result == GDI_ERROR){
		// �G���[; GetFontData �ł͈����Ȃ�����
		free(name_content);
		free(name_content_ft);
		return NULL;
	}
	fontsize = result;
	fnt->size = fontsize;
	fnt->fontdata = (unsigned char*)malloc(fontsize);
	if (fnt->fontdata == NULL){
		free(name_content);
		free(name_content_ft);
		return NULL;
	}
	if (IsTTC){
		GetFontData(fnt->hdc, TVP_TT_TABLE_ttcf, 0, fnt->fontdata, fontsize);
	}
	else{
		GetFontData(fnt->hdc, 0, 0, fnt->fontdata, fontsize);
	}
	if (!IsTTC){
		*size = fnt->size;
		*index = fnt->index;
		free(name_content);
		free(name_content_ft);
		return fnt->fontdata;
	}
	i = 0;
	while (1){
		FT_ULong length;
		FT_Error err;
		if (FT_New_Memory_Face(
			m_ftLibrary,
			fnt->fontdata,
			fnt->size,
			i,
			&face)){
			free(name_content);
			free(name_content_ft);
			return NULL;
		}
		// FreeType ����Aname �^�O�̃T�C�Y���擾����
		length = 0;
		err = FT_Load_Sfnt_Table(face, TTAG_name, 0, NULL, &length);
		if (err){
			FT_Done_Face(face);
			free(name_content);
			free(name_content_ft);
			return NULL;
		}
		// FreeType ���瓾�� name �^�O�̒����� Windows ���瓾�������Ɣ�r
		if (length == (DWORD)name_content_size){
			// FreeType ���� name �^�O���擾
			err = FT_Load_Sfnt_Table(face, TTAG_name, 0, name_content_ft, &length);
			if (err){
				FT_Done_Face(face);
				free(name_content);
				free(name_content_ft);
				return NULL;
			}
			// FreeType ����ǂݍ��� name �^�O�̓��e�ƁAWindows ����ǂݍ���
			// name �^�O�̓��e���r����B
			// ��v���Ă���΂��� index �̃t�H���g���g���B
			if (!memcmp(name_content, name_content_ft, name_content_size)){
				FT_Done_Face(face);
				fnt->index = i;
				break;
			}
		}
		FT_Done_Face(face);
		i++;
	}

	*size = fnt->size;
	*index = fnt->index;
	free(name_content);
	free(name_content_ft);
	return fnt->fontdata;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FontManager::FreeWindowsSystemFontData(TSystemFontData *fnt)
{
	if (fnt == NULL){
		return;
	}
	if (fnt->fontdata != NULL){
		free(fnt->fontdata);
	}
	SelectObject(fnt->hdc, fnt->hOldFont);
	DeleteObject(fnt->hFont);
	ReleaseDC(NULL, fnt->hdc);
	free(fnt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
unsigned long FontManager::StreamIoFunc(
	FT_Stream stream,
	unsigned long offset,
	unsigned char* buffer,
	unsigned long count)
{
	if (count != 0){
		TSystemFontData* font = (TSystemFontData*)stream->descriptor.pointer;
		unsigned char* p;
		unsigned long copycount;
		if (font == NULL || font->fontdata == NULL)
			return 0;
		p = font->fontdata;
		if (offset > stream->size)
			return 0;	// error
		copycount = offset + count > stream->size ?
			stream->size - offset :
			count;
		p += offset;
		if (buffer != NULL)
			memcpy(buffer, p, copycount);
		return copycount;
	}
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FontManager::StreamCloseFunc(FT_Stream stream)
{
	FreeWindowsSystemFontData((TSystemFontData*)stream->descriptor.pointer);
	free(stream);
}
#endif

} // namespace Graphics
} // namespace Lumino