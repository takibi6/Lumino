﻿
#include "../../Internal.h"
#include "ShaderVariableBase.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// ShaderVariableBase
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariableBase::ShaderVariableBase()
	: m_modifid(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariableBase::~ShaderVariableBase()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::Initialize(ShaderVariableTypeDesc desc, const String& name, const String& semanticName)
{
	m_desc = desc;
	m_name = name;
	m_semanticName = semanticName;
	m_modifid = false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------;
void ShaderVariableBase::SetBool(bool value)
{
	m_value.SetBool(value);
	m_modifid = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ShaderVariableBase::GetBool()
{
	return m_value.GetBool();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::SetInt(int value)
{
	m_value.SetInt(value);
	m_modifid = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ShaderVariableBase::GetInt()
{
	return m_value.GetInt();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::SetFloat(float value)
{
	m_value.SetFloat(value);
	m_modifid = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float ShaderVariableBase::GetFloat()
{
	return m_value.GetFloat();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::SetVector(const Vector4& vec)
{
	m_value.SetVector(vec);
	m_modifid = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector4& ShaderVariableBase::GetVector()
{
	return m_value.GetVector();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::SetVectorArray(const Vector4* vectors, int count)
{
	m_value.SetVectorArray(vectors, count);
	m_modifid = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector4* ShaderVariableBase::GetVectorArray()
{
	return m_value.GetVectorArray();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::SetMatrix(const Matrix& matrix)
{
	m_value.SetMatrix(matrix);
	m_modifid = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Matrix& ShaderVariableBase::GetMatrix()
{
	return m_value.GetMatrix();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::SetMatrixArray(const Matrix* matrices, int count)
{
	m_value.SetMatrixArray(matrices, count);
	m_modifid = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Matrix* ShaderVariableBase::GetMatrixArray()
{
	return m_value.GetMatrixArray();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::SetTexture(ITexture* texture)
{
	m_value.SetTexture(texture);
	m_modifid = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* ShaderVariableBase::GetTexture()
{
	return m_value.GetTexture();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::SetString(const char* str)
{
	m_value.SetString(str);
	m_modifid = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const TCHAR* ShaderVariableBase::GetString()
{
	return m_value.GetString();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::OnLostDevice()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderVariableBase::OnResetDevice()
{
	switch (m_desc.Type)
	{
	case ShaderVariableType_Bool:			SetBool(m_value.GetBool()); break;
	case ShaderVariableType_Int:			SetInt(m_value.GetInt()); break;
	case ShaderVariableType_Float:			SetFloat(m_value.GetFloat()); break;
	case ShaderVariableType_Vector:			SetVector(m_value.GetVector()); break;
	case ShaderVariableType_VectorArray:	SetVectorArray(m_value.GetVectorArray(), m_value.GetArrayLength()); break;
	case ShaderVariableType_Matrix:			SetMatrix(m_value.GetMatrix()); break;
	case ShaderVariableType_MatrixArray:	SetMatrixArray(m_value.GetMatrixArray(), m_value.GetArrayLength()); break;
	case ShaderVariableType_Texture:		SetTexture(m_value.GetTexture()); break;
	//case ShaderVariableType_String:			SetString(m_value.String); break;
	}
}

} // namespace Device
} // namespace Graphics
} // namespace Lumino
