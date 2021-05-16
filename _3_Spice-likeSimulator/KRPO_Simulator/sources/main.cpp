#include <iostream>
#include <string>
#include <cstring>
#include <Windows.h>

#include "PluginManager.hpp"
#include "Netlist.hpp"
#include "NetlistReader.hpp"
#include "Simulator.hpp"
#include "Solver.hpp"
#include "Dumper.hpp"

bool ParseCmdLine(int &argc, char *argv[], PluginManager &_pm, std::string &_netlistFileName, std::string &_solverName, std::string &_dumperName);
bool ListPlugins(Plugins &plugins);

int main(int argc, char *argv[]) {
	setlocale(LC_CTYPE, "rus");

	std::cout << "SPICE-like simulator for KRPO labs" << std::endl;
	std::cout << "DICD, NRU MIET, 2020" << std::endl << std::endl;

	PluginManager pluginManager;
	std::string   netlistFileName,
		solverName,
		dumperName;
	if (!pluginManager.IsOk())
		return EXIT_FAILURE;

	if (!ParseCmdLine(argc, argv, pluginManager, netlistFileName, solverName, dumperName))
		return EXIT_FAILURE;

	Solver *p_solver = nullptr;
	Dumper *p_dumper = nullptr;
	if (!pluginManager.SelectSolver(solverName))
		return EXIT_FAILURE;
	p_solver = pluginManager.GetSolver();

	if (!pluginManager.SelectDumper(dumperName))
		return EXIT_FAILURE;
	p_dumper = pluginManager.GetDumper();

	Netlist      *p_netlist = new Netlist;
	Netlistreader netlistReader;

	if (!netlistReader.readNetlist(netlistFileName, p_netlist)) {
		delete p_netlist;
		return EXIT_FAILURE;
	}

	Simulator  simulator(p_netlist);
	for (size_t i = 0; i < p_netlist->analyses.size(); ++i)
		switch (p_netlist->analyses[i]->type) {
		case AnalysisType::Tran:
			simulator.runTran(static_cast<AnalysisTran *>(p_netlist->analyses[i]), p_solver, p_dumper);
			break;
		}

	pluginManager.FreeSolver(p_solver);
	pluginManager.FreeDumper(p_dumper);

	std::cout << std::endl;
	return EXIT_SUCCESS;
}


bool ParseCmdLine(int &argc, char *argv[], PluginManager &_pm, std::string &_netlistFileName, std::string &_solverName, std::string &_dumperName) {
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i], "--list")) {
			_pm.List();
			continue;
		}
		if (!strcmp(argv[i], "--input")) {
			if (i < argc) {
				_netlistFileName = argv[++i];
				continue;
			}
			else {
				std::cout << "Error. Parameter '--input' should be followed by a filename" << std::endl << std::endl;
				return false;
			}
		}
		if (!strcmp(argv[i], "--solver")) {
			if (i < argc) {
				_solverName = argv[++i];
				continue;
			}
			else {
				std::cout << "Error. Parameter '--solver' should be followed by a filename" << std::endl << std::endl;
				return false;
			}
		}
		if (!strcmp(argv[i], "--dumper")) {
			if (i < argc) {
				_dumperName = argv[++i];
				continue;
			}
			else {
				std::cout << "Error. Parameter '--dumper' should be followed by a filename" << std::endl << std::endl;
				return false;
			}
		}
	}

	if (_solverName.empty()) {
		_solverName = _pm.GetDefaultSolverName();
		if (_solverName.empty()) {
			std::cout << "Error. Can't choose default solver name. No solvers found." << std::endl;
			return false;
		}
		std::cout << "Default solver filename was set to : '" << _solverName << "'" << std::endl;
	}

	if (_dumperName.empty()) {
		_dumperName = _pm.GetDefaultDumperName();
		if (_dumperName.empty()) {
			std::cout << "Error. Can't choose default dumper name. No dumpers found." << std::endl;
			return false;
		}
		std::cout << "Default dumper filename was set to  : '" << _dumperName << "'" << std::endl;
	}

	if (_netlistFileName.empty()) {
		std::cout << "Error. Filename to simulate was not set." << std::endl;
		std::cout << "        Use this program as the following:" << std::endl;
		std::cout << "            KRPO_Simulator --input <netlist filename>:" << std::endl << std::endl;
		return false;
	}

	std::cout << std::endl;

	return true;
}

