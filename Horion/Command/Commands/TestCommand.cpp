#include "TestCommand.h"

#define DECL_API(f) decltype(f)* f##_ = _dll[#f]

class ChakraApi {
	DllHelper _dll{ "Chakra.dll" };

public:
	DECL_API(JsCreateRuntime);
	DECL_API(JsCreateContext);
	DECL_API(JsSetCurrentContext);
	DECL_API(JsDisposeRuntime);
	DECL_API(JsRunScript);
	DECL_API(JsConvertValueToString);
	DECL_API(JsStringToPointer);
};

TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "")
{
}


TestCommand::~TestCommand()
{
}

ChakraApi chakra;

bool TestCommand::execute(std::vector<std::string>* args)
{
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef result;
	unsigned currentSourceContext = 0;

	// Your script; try replace hello-world with something else
	std::wstring script = L"(()=>{return \'Hello world!\';})()";

	// Create a runtime. 
	chakra.JsCreateRuntime_(JsRuntimeAttributeNone, nullptr, &runtime);

	// Create an execution context. 
	chakra.JsCreateContext_(runtime, &context);

	// Now set the current execution context.
	chakra.JsSetCurrentContext_(context);

	// Run the script.
	chakra.JsRunScript_(script.c_str(), currentSourceContext++, L"", &result);

	// Convert your script result to String in JavaScript; redundant if your script returns a String
	JsValueRef resultJSString;
	chakra.JsConvertValueToString_(result, &resultJSString);

	// Project script result back to C++.
	const wchar_t* resultWC;
	size_t stringLength;
	chakra.JsStringToPointer_(resultJSString, &resultWC, &stringLength);
	
	clientMessageF("Result: %S", resultWC);

	// Dispose runtime
	chakra.JsSetCurrentContext_(JS_INVALID_REFERENCE);
	chakra.JsDisposeRuntime_(runtime);
	return true;
}
