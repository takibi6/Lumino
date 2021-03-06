
#pragma once
#include <Shobjidl.h>
#include <Lumino/Platform/Win32/Win32PlatformWindow.h>

LN_NAMESPACE_BEGIN
namespace tr
{

class Win32PlatformFileOpenDialog
	: public PlatformFileOpenDialog
{
public:
	enum class Type
	{
		Open,
		Save,
	};

	Win32PlatformFileOpenDialog()
		: m_fileDialog(NULL)
	{
	}

	virtual ~Win32PlatformFileOpenDialog()
	{
		Finalize();
	}

	void Initialize(Type type)
	{
		if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))	// multi-threaded is not supported
		{
			HRESULT hr;
			if (type == Type::Open)
			{
				hr = CoCreateInstance(
					CLSID_FileOpenDialog,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_PPV_ARGS(&m_fileDialog));
			}
			else
			{
				hr = CoCreateInstance(
					CLSID_FileSaveDialog,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_PPV_ARGS(&m_fileDialog));

			}
			LN_THROW(SUCCEEDED(hr), COMException, hr);
		}
	}

	void Finalize()
	{
		if (m_fileDialog != NULL) m_fileDialog->Release();
		CoUninitialize();
	}

	virtual bool ShowDialog(PlatformWindow* parent) override
	{
		LN_CHECK_ARG(parent != nullptr);
		auto* win32Window = static_cast<Win32PlatformWindow*>(parent);
		HWND hParent = win32Window->GetWindowHandle();
		HRESULT hr = m_fileDialog->Show(hParent);
		if (SUCCEEDED(hr))
		{
			return true;
		}
		return false;
	}

	virtual PathName GetFilePath() override
	{
		LPWSTR wcFileName;
		HRESULT hr = m_fileDialog->GetFileName(&wcFileName);
		ln::detail::StaticallyLocalPath path(wcFileName);
		return path.GetPath();
	}

private:
	IFileDialog*	m_fileDialog;
};

} // namespace tr
LN_NAMESPACE_END
