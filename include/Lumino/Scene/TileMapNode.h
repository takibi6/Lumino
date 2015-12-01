
#pragma once
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Tilemap/TileMapRenderer.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
class TileMapRenderer;

/**
	@brief	
*/
class TileMapNode
	: public VisualNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		
	*/
	static TileMapNode* Create();

	/**
		@brief
	*/
	static TileMapNode* Create3D();

public:
	void SetTileMap();

protected:
	TileMapNode();
	virtual ~TileMapNode();
	void Create3DCore(SceneGraphManager* manager);
	virtual void DrawSubset(RenderingParams& params, int subsetIndex);

private:
	SpriteTileMapRenderer*	m_renderer;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END


#if 0
//==============================================================================
// Tilemap 
//------------------------------------------------------------------------------
///**
//  @file       Tilemap.h
//  @brief      Tilemap
//*/
//==============================================================================

#pragma once

#include "../../Base/Table.h"
#include "VisualNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Tilemap
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Tilemap
    : public VisualNode
{
public:
	static const int DefaultTilePixelSize = 32;

public:
    Tilemap( SceneGraph* scene );
    virtual ~Tilemap();

public:

	/// �쐬
    void create(/* lnU32 width, lnU32 height, LNTilemapDirection frontDir */);

	/// �\���ʒu�̐ݒ�
	//virtual void setPosition( int x_, int y_ );

	/// �^�C���Z�b�g�e�N�X�`���̐ݒ�
    void setTilesetTexture( Graphics::Texture* texture );

	/// �^�C���Z�b�g�e�N�X�`���̉������̃^�C�����̐ݒ�
	void setTilesetHorizontalTileCount(size_t count) { mTilesetHorizontalTileCount = count; }

	/// �^�C���ЂƂ��̃s�N�Z���T�C�Y��ݒ肷��
	void setTileSize(size_t width, size_t height);



    //virtual void setMapData( int* mapdata ) { mMapData = mapdata; }

	/// �}�b�v�f�[�^�̐ݒ�
	void setMapData(Base::IntTable* mapData) { LN_REFOBJ_SET(mMapData, mapData); }


public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }
    virtual void update( float elapsedTime );

private:
	Base::IntTable*		mMapData;
	LNTilemapDirection	mDirection;
    Graphics::Texture*	mTilesetTexture;
	size_t				mTilesetHorizontalTileCount;
	lnFloat     mTileWidth;     ///< �^�C���ЂƂ��̕�
	lnFloat     mTileHeight;    ///< �^�C���ЂƂ��̍���

    //lnU32         mXSize;         ///< X �����̃^�C���̐�
    //lnU32         mYSize;         ///< Y �����̃^�C���̐�
    //lnU32         mFaceNum;       ///< �ʐ�


    //lnFloat     mUVWidth;       ///< �}�b�v�`�b�v�ЂƂ��̃s�N�Z���T�C�Y���e�N�X�`�����W�n�ɕϊ������Ƃ��̕�
    //lnFloat     mUVHeight;      ///< �}�b�v�`�b�v�ЂƂ��̃s�N�Z���T�C�Y���e�N�X�`�����W�n�ɕϊ������Ƃ��̕�

    ////int*        mMapData;

    //lnU32         mSrcXNum;       ///< �^�C���Z�b�g�摜�̉������̃`�b�v�� (RGSS �̏ꍇ�� 8)
    
	class DrawingContext;
	friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================
#endif