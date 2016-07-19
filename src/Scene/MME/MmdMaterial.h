
#pragma once
#include <Lumino/Graphics/Material.h>
#include <Lumino/Scene/Material.h>

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class MmdMaterial
	: public Material2
{
public:
	void SetToonTexture(Texture* texture);
	Texture* GetToonTexture() const { return m_toonTexture; }
	void SetSphereTexture(Texture* texture);
	Texture* GetSphereTexture() const { return m_sphereTexture; }
	
LN_INTERNAL_ACCESS:
	MmdMaterial();
	virtual ~MmdMaterial();

public:	// TODO:
	Color		m_diffuse;			// ���̂̐F
	Color		m_ambient;			// ����
	Color		m_specular;			// ����
	Color		m_emissive;			// ���̂̔����F ( �����̉e�����󂯂Ȃ��F )
	float		m_power;			// ����̋���

	Texture*	m_toonTexture;
	Texture*	m_sphereTexture;

	Color						ToonColor;			///< [PMD] �g�D�[���J���[
	Color						EdgeColor;			///< [PMX] �G�b�W�J���[
	float						EdgeSize;			///< [PMX] �G�b�W�T�C�Y
	Color						TextureCoe;			///< [PMX] �e�N�X�`���W��
	Color						SphereTextureCoe;	///< [PMX] �X�t�B�A�e�N�X�`���W��
	Color						ToonTextureCoe;		///< [PMX] Toon�e�N�X�`���W��
	uint32_t					DrawingFlags;		///< [PMX] �`��I�v�V���� (MMDDrawingFlags �̑g�ݍ��킹)
	int/*Material::SphereMode*/		SphereMode;			///< [PMX] �X�t�B�A�e�N�X�`���̍������[�h
};



LN_NAMESPACE_END
