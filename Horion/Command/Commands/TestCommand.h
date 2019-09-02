#pragma once
#include "ICommand.h"
#include "../../../include/chakra/ChakraCore.h"

class ProcPtr {
public:
	explicit ProcPtr(FARPROC ptr) : _ptr(ptr) {}

	template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
	operator T * () const {
		return reinterpret_cast<T*>(_ptr);
	}

private:
	FARPROC _ptr;
};

class DllHelper {
public:
	explicit DllHelper(LPCSTR filename) : _module(GetModuleHandleA(filename)) {}

	~DllHelper() {}

	ProcPtr operator[](LPCSTR proc_name) const {
		return ProcPtr(GetProcAddress(_module, proc_name));
	}

	static HMODULE _parent_module;

private:
	HMODULE _module;
};

class TestCommand :
	public IMCCommand
{
public:
	TestCommand();
	~TestCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};

