
#include <wx/wx.h>

#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/msw/regconf.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>

#include <fstream>
#include <iostream>

using namespace std;

enum {
	wxMENU_ITEM_GRAPHS = +1,
	wxMENU_ITEM_PAINT,
	Paint,
	TextOn,

};
int flag = 0;
int flag_time = 0;
int flag_group = 0;
int flag_end = 0;
int ix = 0;
int iy11 = 0;
int iy22 = 0;
int iy33 = 0;
int j = 0;
class Frame : public wxFrame
{
public:
	Frame(const wxString& title);
	/*void OnOpen(wxCommandEvent& event);
	wxTextCtrl *tc;*/
private:
	//void OnMenu_Graphs(wxCommandEvent &evt);
	void OnMenu_Graphs_Paint(wxCommandEvent &evt);

};

class Window : public wxWindow {
public:
	Window(wxWindow *p_parent);
	void OnOpen(wxCommandEvent& event);
	wxTextCtrl *tc;
private:
	void OnPaint(wxPaintEvent &evt);
};

Window::Window(wxWindow *p_parent)
	: wxWindow(p_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
	Connect(wxEVT_PAINT, wxPaintEventHandler(Window::OnPaint));

}

//double x = 450, y = 100;
double x = 0, y = 90;
double y11 = 100;
double y22 = 100;
double y33 = 100;
int xRect = 150;
int yRect = 150;

double buffer[MAX_PATH] = { NULL };
double buffer11[MAX_PATH] = {NULL};
double buffer22[MAX_PATH] = { NULL };
double buffer33[MAX_PATH] = { NULL };

double probbuf1[MAX_PATH];
double probbuf2[MAX_PATH];

void Window::OnPaint(wxPaintEvent &evt) {
	wxPaintDC dc(this);
	//wxRect r = GetClientRect();
	wxRect r = wxRect(400, 50, 950, 800);

	r.Deflate(10);
	dc.DrawRectangle(r);

	//x = (buffer[0]);
	/*y11 = (buffer[1]);
	y22 = (buffer[2]);
	y33 = (buffer[3]);*/

	///Координатная сетка
	//x
	dc.DrawLine(450, 750, 450, 100);
	dc.DrawLine(450, 100, 440, 130);
	dc.DrawLine(450, 100, 460, 130);
	//y
	dc.DrawLine(450, 750, 1300, 750);
	dc.DrawLine(1300, 750, 1270, 740);
	dc.DrawLine(1300, 750, 1270, 760);
	///

	//dc.DrawRectangle(450, 100, x * 1e9 + 50, y11 + 45);
	//dc.DrawLine(x * 1e9 + 450, y11 + 500, x * 1e9 + 700, y11 + 45);
	//dc.DrawLine(x * 1e9 + 450, y11 * (-1) * 50 + 700, x * 1e9 + 700, y11 * (-1) * 50 + 700);
	///
	//dc.DrawLine(x * 1e9 + 450, y11 * (-1) * 50 + 700, x * 1e9 + 700, y11 * (-1) * 50 + 700);
	///
	//dc.DrawLine(x, y, );

	//POINT probbuf1[18] = {1, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170};
	//POINT probbuf2[18] = {10, 10, 20, 20, 30, 30, 40, 50, 50, 50, 50, 40, 30, 30, 20, 20, 10, 10};

	if (flag == Paint) {
		/*for (ix; ix < iy11; j++) {
			dc.DrawLine(buffer[j] * 1e9 + 450, buffer[j] * (-1) * 50 + 700, buffer[j] * 1e9 + 700, buffer[j] * (-1) * 50 + 700);
		}*/
		///
		for (int i = 0; i < 150; i++) {
			//dc.DrawLine(buffer[i] * 1e9 + 450, buffer11[i] * (-1) * 50 + 700, buffer[i + 1] * 1e9 + 700, buffer11[i + 1] * (-1) * 50 + 700);

		dc.SetPen(wxPen(wxColor(255, 0, 0), 2));
		dc.DrawLine(buffer[i] * 8e9 + 450, buffer11[i] * (-1) * 40 + 750, buffer[i + 1] * 8e9 + 450, buffer11[i + 1] * (-1) * 40 + 750);
		dc.DrawText(wxT("0"), 800, 760);

		dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
		dc.DrawLine(buffer[i] * 8e9 + 450, buffer22[i] * (-1) * 40 + 500, buffer[i + 1] * 8e9 + 450, buffer22[i + 1] * (-1) * 40 + 500);
		dc.DrawText(wxT("1"), 800, 515);

		dc.SetPen(wxPen(wxColor(0, 128, 0), 2));
		dc.DrawLine(buffer[i] * 8e9 + 450, buffer33[i] * (-1) * 40 + 400, buffer[i + 1] * 8e9 + 450, buffer33[i + 1] * (-1) * 40 + 400);
		dc.DrawText(wxT("2"), 800, 415);

		}
		///

		/*for (int i = 0; i < 18; i++) {
			dc.DrawLine(probbuf1[i].x + 450, probbuf2[i].y + 700, probbuf1[i + 1].x + 450, probbuf2[i + 1].y + 700);
		}*/

		/*dc.DrawRectangle(450, 100, 50, 30);
		dc.DrawLine(x - 20, y + 15, x, y + 15);
		dc.DrawLine(x + 50, y + 15, x + 70, y + 15);
		dc.DrawText(wxT("Resistor"), x - 25, y - 25);*/
		/*x = (buffer[0]);
		y11 = (buffer[1]);
		y22 = (buffer[2]);
		y33 = (buffer[3]);
		dc.DrawRectangle(x * 1e9, y11, x * 1e9, y11);*/
	}
}

Frame::Frame(const wxString & title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1500, 1000))
{
	wxMenuBar *menubar = new wxMenuBar;
	wxMenu *file = new wxMenu;
	file->Append(wxID_OPEN, wxT("&Open"));
	menubar->Append(file, wxT("&File"));

	wxMenu *p_menuGraphs = new wxMenu;
	p_menuGraphs->Append(wxMENU_ITEM_PAINT, wxT("&Paint"));
	menubar->Append(p_menuGraphs, wxT("&Graphs"));

	SetMenuBar(menubar);

	Window *p_wnd = new Window(this);


	/*Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(Frame::OnOpen));*/
	Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(Window::OnOpen));

	/*Connect(wxMENU_ITEM_GRAPHS, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(Frame::OnMenu_Graphs));*/

	Connect(wxMENU_ITEM_PAINT, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(Frame::OnMenu_Graphs_Paint));

	//tc = new wxTextCtrl(this, -1, wxT(""), wxPoint(-1, -1),
	//	//wxSize(-1, -1), wxTE_MULTILINE);
	//	wxSize(300, 800), wxTE_MULTILINE);

	p_wnd->Connect(wxID_OPEN, wxCommandEventHandler(Window::OnOpen), NULL, this);
	Center();
}

//void Frame::OnOpen(wxCommandEvent& event)
void Window::OnOpen(wxCommandEvent& event)
{
	flag = TextOn;
	tc = new wxTextCtrl(this, -1, wxT(""), wxPoint(-1, -1),
		//wxSize(-1, -1), wxTE_MULTILINE);
		wxSize(300, 800), wxTE_MULTILINE);

	wxFileDialog * FrameDialog = new wxFileDialog(this);
	if (FrameDialog->ShowModal() == wxID_OK) {
		wxString fileName = FrameDialog->GetPath();
		tc->LoadFile(fileName);

		//wxTextFile file(wxT("rc_net.tr0"));

		wxTextFile file(fileName);
		file.Open();

		//wxMessageBox(wxT("First line: %s\n"), file.GetFirstLine().c_str());
		wxString value = "";
		wxString s;
		//flag = 0;
		//wchar_t buffer [5000];
		for (s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine()) {
			//wxMessageBox("Ищу \"time\"",s);
			
			if (flag_group == 5) {
				///wxMessageBox("В буфере для у33", s);
				/*if (s.Contains(wxT(""))) {
					wxMessageBox("String", s);
				}*/
				//wxStringTokenizer tokenizer(s, "");
				//while (tokenizer.HasMoreTokens())
				//{
				//	wxString token = tokenizer.GetNextToken();
				//	// process token here
				//	//wxMessageBox("Выделяю тайм",token);
				//	value = token;
				//	//wxMessageBox("Значение value в цикле", value);
				//}
				//x = wxAtoi(s);

				//y = wxAtof(value.c_str()); //запись значений по х "time"
				y33 = wxAtof(s.c_str());
				//flag = 4;
				///
				flag_group = 0;
				///
				flag = 1;
				//buffer[3] = y33;
				Refresh();
				buffer33[iy33] = y33;
				iy33++;
			}
			if (flag_group == 4) {
				///wxMessageBox("В буфере для у22", s);
				y22 = wxAtof(s.c_str());
				flag_group = 5;
				flag = 1;
				//buffer[2] = y22;
				Refresh();
				buffer22[iy22] = y22;
				iy22++;
			}
			if (flag_group == 3) {
				///wxMessageBox("В буфере для у11", s);
				y11 = wxAtof(s.c_str());
				flag_group = 4;
				//flag = 1;
				///
				//buffer[1] = y11;
				Refresh();
				///
				buffer11[iy11] = y11;
				iy11++;
			}
			if (s.Contains(wxT("VALUE"))) {
				///wxMessageBox(" Нашёл Value ", s);
				flag = 1;
				//flag_group = 1;
			}
			if (flag == 1) {
				if (s.Contains(wxT("\"time\""))) {
					///wxMessageBox(" Нашёл строку time ", s);
					flag = 2;
				}
			}
			if (flag == 2) {
				//wxMessageBox("String in buffer :", s);
				wxStringTokenizer tokenizer(s, " ");
				while (tokenizer.HasMoreTokens())
				{
					wxString token = tokenizer.GetNextToken();
					// process token here
					//wxMessageBox("Выделяю тайм",token);
					value = token;
					//wxMessageBox("Значение value в цикле", value);
				}
				//x = wxAtoi(s);
				x = wxAtof(value.c_str()); //запись значений по х "time"
				//buffer[0] = x;
				///wxMessageBox(" Значение x после поиска value", value);
				flag = 1;
				///
				buffer[ix] = x;
				ix++;
				///
			}
			if (flag == 1) {
				if (s.Contains(wxT("\"group\""))) {
					///wxMessageBox(" Нашёл строку group ", s);
					flag = 0;
					flag_group = 3;
				}
			}
			if (s.Contains(wxT("END"))) {
				///wxMessageBox(" Нашёл END ", s);
				///
				break;
				///
			}
		}

		file.Close();

		//fout << "";
		/*
		ofstream fout("ProbWrite.txt"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt
		//fout << "файлами в С++ \n"; // запись строки в файл
		//fout.close(); // закрываем файл

		char buff[500]; // буфер промежуточного хранения считываемого из файла текста
		ifstream fin("rc_net.tr0"); // открыли файл для чтения

		fin >> buff; // считали первое слово из файла
		//cout << buff << endl; // напечатали это слово

		fout << buff;



		fin.getline(buff, 50); // считали строку из файла
		fin.close(); // закрываем файл
		//cout << buff << endl; // напечатали эту строку
		fout.close(); // закрываем файл
		*/

	}
}


void Frame::OnMenu_Graphs_Paint(wxCommandEvent &evt) {
	flag = Paint;
	wxMessageBox(wxT("Clicked"));
	Refresh();
}

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};
IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	Frame *open = new Frame(wxT("Lab 5"));
	open->Show(true);
	return true;
}

