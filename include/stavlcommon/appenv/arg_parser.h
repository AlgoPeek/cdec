// -------------------------------------------------------------------------- //
// Application Argument Parser
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// ArgumentParser
// -------------------------------------------------------------------------- //

interface IArgumentAccept : public Object
{
	virtual bool AddSwitch(stringx key, stringx value) = 0;
	virtual bool AddName(stringx name) = 0;
	virtual bool Complete() = 0;
};

class STAVLCOMMONEXPORT ArgumentParser
{
public:
	static bool Parse(int argc, char* argv[], ref<IArgumentAccept> accp);
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
