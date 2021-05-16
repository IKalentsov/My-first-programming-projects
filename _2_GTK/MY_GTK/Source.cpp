#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

static void do_drawing(cairo_t *);
static void do_drawing_my(cairo_t *);

GtkWidget *window = nullptr;

enum {
	r = +1,
	y,
	br,
	g,
	bl,
	p,
};

int flagBrush = 0;
int flagPen = 0;

static gboolean onDraw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
	switch (flagBrush) {
	case r: //Red
		cairo_set_source_rgb(cr, 1, 0, 0);
		cairo_rectangle(cr, 20, 40, 100, 100);
		cairo_fill(cr);
		break;

	case y: //Yellow
		cairo_set_source_rgb(cr, 1, 1, 0);
		cairo_rectangle(cr, 20, 40, 100, 100);
		cairo_fill(cr);
		break;

	case br: //Brown
		cairo_set_source_rgb(cr, 0.5, 0.2, 0.1);
		cairo_rectangle(cr, 20, 40, 100, 100);
		cairo_fill(cr);
		break;
	}

	switch (flagPen) {
		///The outlines of shapes
	case g: //Green
		cairo_set_source_rgb(cr, 0, 1, 0);
		cairo_rectangle(cr, 20, 40, 100, 100);
		break;
	case bl: //Blue
		cairo_set_source_rgb(cr, 0, 0, 1);
		cairo_rectangle(cr, 20, 40, 100, 100);
		break;
	case p: //Pink
		cairo_set_source_rgb(cr, 1, 0.4, 0.5);
		cairo_rectangle(cr, 20, 40, 100, 100);
		break;
	}
	cairo_stroke(cr);
	return TRUE;
}


void OnMenuBrushDrawRed() {
	flagBrush = r;
	//перерисовать
	gtk_widget_queue_draw(window);
}

void OnMenuBrushDrawYellow() {
	flagBrush = y;
	//перерисовать
	gtk_widget_queue_draw(window);
}

void OnMenuBrushDrawBrown() {
	flagBrush = br;
	//перерисовать
	gtk_widget_queue_draw(window);
}

void OnMenuPenDrawGreen() {
	flagPen = g;
	//перерисовать
	gtk_widget_queue_draw(window);
}

void OnMenuPenDrawBlue() {
	flagPen = bl;
	//перерисовать
	gtk_widget_queue_draw(window);
}

void OnMenuPenDrawPink() {
	flagPen = p;
	//перерисовать
	gtk_widget_queue_draw(window);
}


int main(int argc, char *argv[])
{
	GtkWidget *p_draw;
	GtkWidget *drawing;
	
	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *fileMenu;
	GtkWidget *fileMi;
	GtkWidget *quitMi;
	GtkWidget *Brush;
	GtkWidget *Pen;
	GtkWidget *Red;
	GtkWidget *Yellow;
	GtkWidget *Brown;
	GtkWidget *Green;
	GtkWidget *Blue;
	GtkWidget *Pink;
	GtkWidget *BrushMenu;
	GtkWidget *PenMenu;


	gtk_init(&argc, &argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	menubar = gtk_menu_bar_new();
	fileMenu = gtk_menu_new();
	BrushMenu = gtk_menu_new();
	PenMenu = gtk_menu_new();

	//create menu
	fileMi = gtk_menu_item_new_with_label("File");
	quitMi = gtk_menu_item_new_with_label("Exit");

	Brush =  gtk_menu_item_new_with_label("Brush");
	Red =    gtk_menu_item_new_with_label("Red");
	Yellow = gtk_menu_item_new_with_label("Yellow");
	Brown =  gtk_menu_item_new_with_label("Brown");

	Pen =    gtk_menu_item_new_with_label("Pen");
	Green =  gtk_menu_item_new_with_label("Green");
	Blue =   gtk_menu_item_new_with_label("Blue");
	Pink =   gtk_menu_item_new_with_label("Pink");

	//ћеню File, Brush, Pen
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(Brush), BrushMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(BrushMenu), Red);
	gtk_menu_shell_append(GTK_MENU_SHELL(BrushMenu), Yellow);
	gtk_menu_shell_append(GTK_MENU_SHELL(BrushMenu), Brown);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), Brush);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(Pen), PenMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(PenMenu), Green);
	gtk_menu_shell_append(GTK_MENU_SHELL(PenMenu), Blue);
	gtk_menu_shell_append(GTK_MENU_SHELL(PenMenu), Pink);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), Pen);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

	//¬иджет, определ€ющий область рисовани€
	p_draw = gtk_drawing_area_new();
	gtk_box_pack_start(GTK_BOX(vbox), p_draw, TRUE, TRUE, 0);

	g_signal_connect(p_draw, "draw", (GCallback)onDraw, NULL);


	//закрытие окна по крестику в тулбаре
	g_signal_connect(window, "destroy",
		G_CALLBACK(gtk_main_quit), NULL);

	//выход по кнопке Exit
	g_signal_connect(G_OBJECT(quitMi), "activate",
		G_CALLBACK(gtk_main_quit), NULL);

	g_signal_connect(G_OBJECT(Red), "activate",
		G_CALLBACK(OnMenuBrushDrawRed), NULL);
	g_signal_connect(G_OBJECT(Yellow), "activate",
		G_CALLBACK(OnMenuBrushDrawYellow), NULL);
	g_signal_connect(G_OBJECT(Brown), "activate",
		G_CALLBACK(OnMenuBrushDrawBrown), NULL);

	g_signal_connect(G_OBJECT(Green), "activate",
		G_CALLBACK(OnMenuPenDrawGreen), NULL);
	g_signal_connect(G_OBJECT(Blue), "activate",
		G_CALLBACK(OnMenuPenDrawBlue), NULL);
	g_signal_connect(G_OBJECT(Pink), "activate",
		G_CALLBACK(OnMenuPenDrawPink), NULL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
	gtk_window_set_title(GTK_WINDOW(window), "Original title");

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
