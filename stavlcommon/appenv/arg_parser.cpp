#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// ArgumentParser
// -------------------------------------------------------------------------- //

const WCHAR cc[] = { '=', ':' };

bool ArgumentParser::Parse(int argc, char* argv[], ref<IArgumentAccept> accp)
{
	ASSERT(sizeof(cc) == 2 * sizeof(WCHAR));
	ref<Encoding> e = Encoding::get_UTF8();
	bool fAccpAll = true;
	for (int i = 1; i < argc; ++i)
	{
		stringx arg = e->ToUnicode(argv[i]);
		WCHAR prefix = arg[0];
		bool fAccp;

		if (prefix == '/' || prefix == '-')
		{
			int pos = arg.IndexOfAny(cc, sizeof(cc) / sizeof(cc[0]));
			if (pos >= 0)
				fAccp = accp->AddSwitch(arg.Substring(1, pos - 1), arg.Substring(pos + 1));
			else
				fAccp = accp->AddSwitch(arg.Substring(1), NULL);
		}
		else
			fAccp = accp->AddName(arg);

		if (!fAccp)
		{
			fAccpAll = false;
			KLOGT << "Invalid argument: " << arg;
		}
	}

	if (!accp->Complete())
	{
		fAccpAll = false;
		KLOGT << "Invaid argument found";
	}

	return fAccpAll;
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
