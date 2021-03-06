
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class PlatformFileOpenDialog;
using PlatformFileOpenDialogPtr = RefPtr<PlatformFileOpenDialog>;

/**
	@brief	
*/
class PlatformFileOpenDialog
	: public Object
{
public:
	static PlatformFileOpenDialogPtr Create();

public:
	virtual bool ShowDialog(PlatformWindow* parent) = 0;

	virtual PathName GetFilePath() = 0;

protected:
	PlatformFileOpenDialog();
	virtual ~PlatformFileOpenDialog();
};

} // namespace tr
LN_NAMESPACE_END
