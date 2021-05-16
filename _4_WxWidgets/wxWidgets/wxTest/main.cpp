#include <wx/wx.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/msw/regconf.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>
#include <wx/event.h>
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
int flagTextOn = 0;
int ix = 0;
int iy11 = 0;
int iy22 = 0;
int iy33 = 0;
int j = 0;

class Frame : public wxFrame
{
public:
	Frame(const wxString& title);
	void OnOpen(wxCommandEvent& event);
	wxTextCtrl *tc;
private:
	void OnMenu_Graphs_Paint(wxCommandEvent &evt);
};

class Window : public wxWindow {
public:
	Window(wxWindow *p_parent);
private:
	void OnPaint(wxPaintEvent &evt);
	void SizeWindow(wxSizeEvent &evt);
};

Window::Window(wxWindow *p_parent)
	: wxWindow(p_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
	Connect(wxEVT_PAINT, wxPaintEventHandler(Window::OnPaint));
}

double x = 0, y = 90;
double y11 = 100;
double y22 = 100;
double y33 = 100;
int xRect = 0;
int yRect = 0;

double buffer[MAX_PATH] = { NULL };
double buffer11[MAX_PATH] = {NULL};
double buffer22[MAX_PATH] = { NULL };
double buffer33[MAX_PATH] = { NULL };

void Window::OnPaint(wxPaintEvent &evt) {
	wxPaintDC dc(this);
	wxRect r = GetClientRect();
	wxEXPAND;
	r.Deflate(10);
	dc.DrawRectangle(r);
	
	///Координаты 
	xRect = r.GetRight();
	yRect = r.GetBottom();
	dc.DrawLine(xRect - (0.9 * xRect), yRect - (0.9 * yRect), xRect - (0.9 * xRect), yRect - (0.1 * yRect));
	dc.DrawLine(xRect - (0.9 * xRect), yRect - (0.9 * yRect), xRect - (0.91 * xRect), yRect - (0.87 * yRect));
	dc.DrawLine(xRect - (0.9 * xRect), yRect - (0.9 * yRect), xRect - (0.89 * xRect), yRect - (0.87 * yRect));
	
	dc.DrawLine(xRect - (0.9 * xRect), yRect - (0.1 * yRect), xRect - (0.1 * xRect), yRect - (0.1 * yRect));
	dc.DrawLine(xRect - (0.1 * xRect), yRect - (0.1 * yRect), xRect - (0.12 * xRect), yRect - (0.12 * yRect));
	dc.DrawLine(xRect - (0.1 * xRect), yRect - (0.1 * yRect), xRect - (0.12 * xRect), yRect - (0.08 * yRect));

	if (flag == Paint) {
		for (int i = 0; i < iy11 - 1; i++) 
		{
			dc.SetPen(wxPen(wxColor(255, 0, 0), 2));
			dc.DrawLine(buffer[i] * 8e9 * (0.001 * xRect) + (xRect - (0.9 * xRect)), ((buffer11[i] * (-1) * 40 * (0.001 * yRect)) + (yRect - (0.15 * yRect))), (buffer[i + 1] * 8e9 * (0.001 * xRect) + (xRect - (0.9 * xRect))), ((buffer11[i + 1] * (-1) * 40 * (0.001 * yRect)) + (yRect - (0.15 * yRect))));
			dc.DrawText(wxT("0"), xRect/2.1, yRect - (0.15 * yRect));
		
			dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
			dc.DrawLine(buffer[i] * 8e9 * (0.001 * xRect) + (xRect - (0.9 * xRect)), ((buffer22[i] * (-1) * 40 * (0.001 * yRect)) + (yRect - (0.4 * yRect))), (buffer[i + 1] * 8e9 * (0.001 * xRect) + (xRect - (0.9 * xRect))), ((buffer22[i + 1] * (-1) * 40 * (0.001 * yRect)) + (yRect - (0.4 * yRect))));
			dc.DrawText(wxT("1"), xRect / 2.1, yRect - (0.4 * yRect));

			dc.SetPen(wxPen(wxColor(0, 128, 0), 2));
			dc.DrawLine(buffer[i] * 8e9 * (0.001 * xRect) + (xRect - (0.9 * xRect)), ((buffer33[i] * (-1) * 40 * (0.001 * yRect)) + (yRect - (0.6 * yRect))), (buffer[i + 1] * 8e9 * (0.001 * xRect) + (xRect - (0.9 * xRect))), ((buffer33[i + 1] * (-1) * 40 * (0.001 * yRect)) + (yRect - (0.6 * yRect))));
			dc.DrawText(wxT("2"), xRect / 2.1, yRect - (0.6 * yRect));
		}
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

	Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(Frame::OnOpen));

	Connect(wxMENU_ITEM_PAINT, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(Frame::OnMenu_Graphs_Paint));

	p_wnd->Connect(wxID_OPEN, wxCommandEventHandler(Frame::OnOpen), NULL, this);
	Center();
}

void Frame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog * FrameDialog = new wxFileDialog(this);
	if (FrameDialog->ShowModal() == wxID_OK) {
		wxString fileName = FrameDialog->GetPath();

		wxTextFile file(fileName);
		file.Open();

		wxString value = "";
		wxString s;

		for (s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine()) {
			if (flag_group == 5) {
				y33 = wxAtof(s.c_str());	// запись значений по х "time"
				flag_group = 0;
				flag = 1;
				Refresh();
				buffer33[iy33] = y33;
				iy33++;
			}
			if (flag_group == 4) {
				y22 = wxAtof(s.c_str());
				flag_group = 5;
				flag = 1;
				Refresh();
				buffer22[iy22] = y22;
				iy22++;
			}
			if (flag_group == 3) {
				y11 = wxAtof(s.c_str());
				flag_group = 4;
				Refresh();
				buffer11[iy11] = y11;
				iy11++;
			}
			if (s.Contains(wxT("VALUE"))) {
				flag = 1;
			}
			if (flag == 1) {
				if (s.Contains(wxT("\"time\""))) {
					flag = 2;
				}
			}
			if (flag == 2) {
				wxStringTokenizer tokenizer(s, " ");
				while (tokenizer.HasMoreTokens())
				{
					wxString token = tokenizer.GetNextToken();
					value = token;
				}
				x = wxAtof(value.c_str()); //запись значений по х "time"
				flag = 1;
				buffer[ix] = x;
				ix++;
			}
			if (flag == 1) {
				if (s.Contains(wxT("\"group\""))) {
					flag = 0;
					flag_group = 3;
				}
			}
			if (s.Contains(wxT("END"))) {
				break;
			}
		}

		file.Close();
	}
}

void Frame::OnMenu_Graphs_Paint(wxCommandEvent &evt) {
	flag = Paint;
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
	Frame *open = new Frame(wxT("WxWidgets Project"));
	open->Show(true);
	return true;
}

