
#pragma once

LN_NAMESPACE_BEGIN

/**
	@brief	再生中の視覚効果を操作します。
*/
class VisualEffectInstance
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	virtual void Stop() = 0;
	virtual bool IsPlaying() = 0;
	virtual void SetWorldMatrix(const Matrix& matrix) = 0;

private:
};

LN_NAMESPACE_END
