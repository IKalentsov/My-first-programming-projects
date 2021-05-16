#include "Dumper.hpp"


Dumper::Dumper() : p_file(nullptr), fileName("") {
}

void Dumper::AddViewpoint(std::string &_name, ViewpointType _vpt, double *_val) {
	Viewpoint vp;
	vp.name = _name;
	vp.type = _vpt;
	vp.valuePtr = _val;
	viewpoints.push_back(vp);
}

ofstream fout;
void Dumper_My::BeginDump(std::string &_fileName) {
	fout;
}

void Dumper_My::WriteHeader() {
	fout.open("myFile.sp");
	if (fout.is_open()) {
		fout << "HEADER" << endl;
		fout << "\"PSFversion\" \"1.00\"" << endl;
		fout << "\"simulator\" \"KSI\"" << endl;
		fout << "\"runtype\" \"Transient Analysis\"" << endl;
		fout << "TYPE" << endl;
		fout << "\"node\" FLOAT DOUBLE PROP(" << endl;
		fout << "\"key\" \"node\" \n )" << endl;
		fout << "\"branch\" FLOAT DOUBLE PROP(" << endl;
		fout << "\"key\" \"branch\" \n )" << endl;
		fout << "\"sweep\" FLOAT DOUBLE" << endl;

		fout << "SWEEP" << endl;
		fout << "\"time\" \"sweep\"" << endl;

		fout << "TRACE" << endl;
		fout << "\"group\" GROUP 3" << endl;
		fout << "\"1\" \"node\" \n\"0\" \"node\" \n\"2\" \"node\"" << endl;

		fout << "VALUE" << endl;
	}
}

void Dumper_My::WriteValuesAtTime(double _t) {
	fout << "\"time\" " << _t << endl << "\"group\"";
	for (int i = 0; i < viewpoints.size(); ++i) {
		if (viewpoints[i].name != "VALUE")
			fout  << "\n"  <<  *viewpoints[i].valuePtr;
	}
	fout << endl;
}

void Dumper_My::EndDump() {
	fout << "END" << endl;
	fout.close();
}

PluginType GetType() {
	return PluginType::dumper;
}

void GetStringID(std::string &_id) {
	_id = "MyDumper";
}

Dumper *GetDumper() {
	return new Dumper_My;
}

void FreeDumper(Dumper *_p_dumper) {
	if (_p_dumper) {
		delete _p_dumper;
		_p_dumper = nullptr;
	}
}

