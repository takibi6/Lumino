
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

/**
	@brief
*/
class DocumentsManager
	: public RefObject
{
public:
	struct ConfigData
	{
		GraphicsManager*	graphicsManager = nullptr;
	};

public:
	DocumentsManager();
	virtual ~DocumentsManager();
	void Initialize(const ConfigData& configData);
	GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	EncodingConverter* GetTCharToUTF32Converter() { return &m_TCharToUTF32Converter; }	// TODO: fontmanager の使えばいいかも
	EncodingConverter* GetUTF32ToTCharConverter() { return &m_UTF32ToTCharConverter; }

private:
	GraphicsManager*		m_graphicsManager;
	EncodingConverter		m_TCharToUTF32Converter;
	EncodingConverter		m_UTF32ToTCharConverter;
};

} // namespace detail
LN_NAMESPACE_END
