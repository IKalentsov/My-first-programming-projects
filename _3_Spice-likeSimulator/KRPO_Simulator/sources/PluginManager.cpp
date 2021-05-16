#include "PluginManager.hpp"

#include <iostream>

void wchar_t2string(const wchar_t *wchar, std::string &dest) {
	dest.erase();
	size_t index = 0;
	while (0 != wchar[index])
		dest += (char)wchar[++index];
}

void string2wchar_t(const std::string &str, wchar_t dest[256]) {
	size_t index = 0;
	while (index < str.size()) {
		dest[index] = (wchar_t)str[index];
		++index;
	}
	dest[index] = 0;
}

bool PluginManager::ListPluginsOnStartup() {
	std::cout << "Searching for available modules..." << std::endl;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = nullptr;

	PluginInfo pi;
	do {
		if (hFind == nullptr) {
			hFind = FindFirstFile("modules\\*.dll", &FindFileData);

			if (INVALID_HANDLE_VALUE == hFind) {
				std::cout << "\tNo modules found" << std::endl << std::endl;
				return false;
			}
		}
		pi.fileName = std::string("modules\\") + std::string(FindFileData.cFileName);
		HMODULE hDll = LoadLibrary(pi.fileName.c_str());
		if (NULL == hDll) {
			std::cout << "\tWarning. Found file " << pi.fileName << ", but it's not our plugin." << std::endl;
			continue;
		}
		type_func_ptr funcType = (type_func_ptr)GetProcAddress(hDll, "GetType");
		if (!funcType) {
			std::cout << "\tWarning. Found file " << pi.fileName << ", but it's not our plugin." << std::endl;
			continue;
		}
		pi.type = funcType();

		idstr_func_ptr funcId = (idstr_func_ptr)GetProcAddress(hDll, "GetStringID");
		if (!funcId) {
			std::cout << "\tWarning. Found file " << pi.fileName << ", but it's not our plugin." << std::endl;
			continue;
		}
		funcId(pi.idString);
		FreeLibrary(hDll);

		plugins.push_back(pi);
	} while (FindNextFile(hFind, &FindFileData));

	int numSolvers = 0,
		numDumpers = 0;

	for (size_t i = 0; i < plugins.size(); ++i)
		switch (plugins[i].type) {
		case PluginType::solver:
			++numSolvers;
			break;
		case PluginType::dumper:
			++numDumpers;
			break;
		}

	if (!numSolvers)
		std::cout << "\tError. No solvers found. Don't know how to solve SLEs." << std::endl;
	if (!numDumpers)
		std::cout << "\tError. No dumpers found. Don't know how to dump results." << std::endl;

	if (numSolvers && numDumpers)
		std::cout << "\tSolvers found: " << numSolvers << std::endl << "\tDumpers found: " << numDumpers << std::endl;

	std::cout << "Modules search completed." << std::endl << std::endl;

	return (numSolvers && numDumpers);
}

PluginManager::PluginManager() : ok(false), hSolverDll(nullptr), hDumperDll(nullptr), p_getSolverFunc(nullptr), p_getDumperFunc(nullptr), p_freeSolverFunc(nullptr), p_freeDumperFunc(nullptr) {
	ok = ListPluginsOnStartup();
}

PluginManager::~PluginManager() {
	if (hSolverDll) {
		FreeLibrary(hSolverDll);
		hSolverDll = nullptr;
	}
	if (hDumperDll) {
		FreeLibrary(hDumperDll);
		hDumperDll = nullptr;
	}
}

bool PluginManager::IsOk() {
	return ok;
}

void PluginManager::List() {
	std::cout << "modules list:" << std::endl;
	for (size_t i = 0; i < plugins.size(); ++i) {
		switch (plugins[i].type) {
		case PluginType::solver:
			std::cout << i + 1 << ". '" << plugins[i].fileName << "' - solver, ID string='" << plugins[i].idString << "'" << std::endl;
			break;
		case PluginType::dumper:
			std::cout << i + 1 << ". '" << plugins[i].fileName << "' - dumper, ID string='" << plugins[i].idString << "'" << std::endl;
			break;
		default:
			std::cout << "*" << i + 1 << ". '" << plugins[i].fileName << "' - unknown type: " << (int)plugins[i].type << ", ID string='" << plugins[i].idString << "'" << std::endl;
		}
	}
	std::cout << std::endl;
}

bool PluginManager::SelectSolver(std::string &_solverName) {
	for (size_t i = 0; i < plugins.size(); ++i) {
		if (plugins[i].type != PluginType::solver)
			continue;
		if (plugins[i].idString == _solverName) {
			hSolverDll = LoadLibrary(plugins[i].fileName.c_str());
			p_getSolverFunc = (get_solver_func_ptr)GetProcAddress(hSolverDll, "GetSolver");
			p_freeSolverFunc = (free_solver_func_ptr)GetProcAddress(hSolverDll, "FreeSolver");
		}
	}
	if (!p_getSolverFunc || !p_freeSolverFunc) {
		std::cout << "Error. Can't find required functions in solver." << std::endl;
		return false;
	}
	return true;
}

bool PluginManager::SelectDumper(std::string &_dumperName) {
	for (size_t i = 0; i < plugins.size(); ++i) {
		if (plugins[i].type != PluginType::dumper)
			continue;
		if (plugins[i].idString == _dumperName) {
			hDumperDll = LoadLibrary(plugins[i].fileName.c_str());
			p_getDumperFunc = (get_dumper_func_ptr)GetProcAddress(hDumperDll, "GetDumper");
			p_freeDumperFunc = (free_dumper_func_ptr)GetProcAddress(hDumperDll, "FreeDumper");
		}
	}
	if (!p_getDumperFunc || !p_freeDumperFunc) {
		std::cout << "Error. Can't find required functions in dumper." << std::endl;
		return false;
	}
	return true;
}

std::string PluginManager::GetDefaultSolverName() {
	for (size_t i = 0; i < plugins.size(); ++i)
		if (plugins[i].type == PluginType::solver)
			return plugins[i].idString;
	return std::string("");
}

std::string PluginManager::GetDefaultDumperName() {
	for (size_t i = 0; i < plugins.size(); ++i)
		if (plugins[i].type == PluginType::dumper)
			return plugins[i].idString;
	return std::string("");
}

Solver *PluginManager::GetSolver() {
	Solver *p_solver = p_getSolverFunc();
	return p_solver;
}

Dumper *PluginManager::GetDumper() {
	Dumper *p_dumper = p_getDumperFunc();
	return p_dumper;
}

void PluginManager::FreeSolver(Solver *_p_solver) {
	p_freeSolverFunc(_p_solver);
}

void PluginManager::FreeDumper(Dumper *_p_dumper) {
	p_freeDumperFunc(_p_dumper);
}
