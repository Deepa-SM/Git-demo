/*
 * Project name : DigiMode Project
 * Programmer:Deepa.S.Malagi
 * Vertion : 1.03v
 * 1.03v:window size changed.analog Smeter added.funtion buttons added
 */
#include <stdlib.h>
#include <iostream>
#include <string> 
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Progress.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <math.h>
int cFerq_x = 50 + 10;
int cFerq_y = 20;
int cFerq_w = 600;
int cFerq_h = 200;
void Timer_CB(void*);

//static unsigned char colors[40] = {167,159,151,143,135,127,119,111,103,95,87,79,71,63, 60,61,62,63,71,79,87,86,85,95,94,93,92,91,90,89,88,82,81,80,73,72,65,64,56 };
const char* Label[11] = {"0", "1", "2", "3", "4","5","6","7","8","9","."};
//Fl_Progress* Pro = (Fl_Progress*)0;
#define Radian(ang) ang*3.14/180 
//
// TRIGGER SHAPED BAR GRAPH CLASS
//   Modified 07/25/16 to include some colors
//
static unsigned char colors[40] = { 167,159,151,143,135,127,119,111,103,95,87,79,71,63, 60,61,62,63,71,79,87,86,85,95,94,93,92,91,90,89,88,82,81,80,73,72,65,64,56 };

const char* ang[24] = { "-6","-5","-4","-3","-2","-1", "0", "1", "2","3","4","5","6","7","8","9","10","12","13","14","15","16" };
int radius_points = 130;
int radius_line = 150;
int radius_thline = 180;
int radius_boxs = 160;


class AnalogGraph : public Fl_Group
{
    float _value,Old_value; int _color;         // 0 - 100
    Fl_Box* S;
public:
    
    AnalogGraph(int X, int Y, int W, int H, const char* L = 0) : Fl_Group(X, Y, W, H, L)
    {
        
        //S->labelcolor(FL_WHITE);
        _value = 0;
    }
    
    void value(float val)
    {
        Old_value = _value;
        _value = val;
        //_color = colo;
        redraw();
        
    }

    void draw()
    {
       
        Fl_Group::draw();
        fl_push_clip(x(), y(), w(), h());
        fl_color(FL_BLACK);
        //fl_rectf(x(), y(), w(), h());
       
        fl_push_matrix();
        fl_color(FL_WHITE);
        fl_line_style(FL_SOLID, 0.5, 0);
        fl_translate(x(), y());

        //erase colour bars 
        fl_color(FL_BLACK);
        fl_line_style(FL_SOLID, 8, 0);
        fl_begin_line();
        fl_arc(w() / 2.0, h(), float(h() * 0.81), 150, 30);
        fl_end_line();
        fl_begin_line();
        fl_arc(w() / 2.0, h(), float(h() * 0.57), 150, 30);
        fl_end_line();
        fl_begin_line();
        fl_arc(w() / 2.0, h(), float(h() * 0.28), 150, 30);
        fl_end_line();
        //-----------------------------------------------------------

        fl_color(FL_WHITE);
        fl_font(FL_TIMES, 13);
        fl_draw(50,"S", x()+30,y()+105);
        fl_font(FL_TIMES, 10);//FL_NORMAL_SIZE
        fl_draw(60, "PWR", x() +40, y() + 125);
        fl_draw(60, "SWR", x() + 55, y() + 135);
        fl_draw(60, "MOD", x() + 70, y() + 140);
        fl_draw(60, "VOLT", x() + 90, y() + 150);
        //fl_color(FL_RED);
        
        fl_line_style(FL_SOLID, 3, 0);
        fl_begin_line();
        //fl_arc(w() / 2.0, h(), float(h()-5), 0, 180.0);
        fl_arc(w() / 2.0, h(), float(h()-2), 0, 180);
        fl_end_line();

        
        fl_line_style(FL_SOLID, 1, 0);
        fl_begin_line();
        fl_arc(w() / 2.0, h(), float(h() * 0.8), 30, 150.0);
        fl_end_line();
        
        fl_begin_line();
        fl_arc(w() / 2.0, h(), float(h() * 0.83), 30, 150.0);
        fl_end_line();

        

        fl_begin_line();
        fl_arc(w() / 2.0, h(), float(h() * 0.56), 30, 150.0);
        fl_end_line();
        fl_begin_line();
        fl_arc(w() / 2.0, h(), float(h() * 0.6), 150, 30);
        fl_end_line();

        fl_begin_line();
        fl_arc(w() / 2.0, h(), float(h() * 0.3), 30, 150.0);
        fl_end_line();

        

        fl_begin_complex_polygon();
        fl_arc(w() / 2.0, h(), float(h() * 0.05), 0, 180.0);
        fl_end_complex_polygon();

        int xa = x() + w() / 2;
        int ya = y() + h();
        draw_pointer_black(xa, ya, (h() - 5), Old_value + 90);//erase old pointer 
        draw_pointer(xa, ya, (h() - 5), _value + 90);//new pointer
        
        fl_color(FL_WHITE);
        draw_color_bar(xa, ya, radius_thline, _value);
        draw_devider();
        draw_points();

        fl_pop_matrix();
        fl_pop_clip();
        //a += 10;
        
    }

    void draw_color_bar(int xc, int yc, int radius, float f)
    {
        int colo = FL_BLACK;
        int colour_count = 10;
        for (float angle = 150; angle >= f && f >=30; angle -= 5.0)
        {

            //int colo = FL_BLACK;
            if (angle >= f)
            {

                colo = colors[colour_count];
                colour_count++;

            }
            else
                colo = FL_BLACK;

            fl_translate(w() / 2.0, h() / 2.0);
            fl_rotate(-360);
            fl_translate(-(w() / 2.0), -(h() / 2.0));
            fl_line_style(FL_SOLID, 5, 0);
            fl_color(colo);
            fl_begin_line();
            fl_arc(w() / 2.0, h(), float(h() * 0.81), f, angle);
            fl_end_line();
            fl_begin_line();
            fl_arc(w() / 2.0, h(), float(h() * 0.58), f, angle);
            fl_end_line();
            fl_begin_line();
            fl_arc(w() / 2.0, h(), float(h() * 0.28), f, angle);
            fl_end_line();
        }


    }
    void draw_devider()
    {
        int xa = x() + (w() / 2);
        int ya = y() + (h());

        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 5.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));

            int ponX1 = xa + int(h() * 0.83) * Sine_val;
            int ponY1 = ya + int(h() * 0.83) * Cos_val;
            int Line_X2 = xa + int(h() * 0.88) * Sine_val;
            int Line_Y2 = ya + int(h() * 0.88) * Cos_val;
            fl_color(FL_WHITE);
            fl_line_style(FL_SOLID, 0.5, 0);
            fl_line(ponX1, ponY1, Line_X2, Line_Y2);
        }
        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 30.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));
            int ponX1 = xa + int(h() * 0.8) * Sine_val;
            int ponY1 = ya + int(h() * 0.8) * Cos_val;
            int Line_X2 = xa + int(h() * 0.75) * Sine_val;
            int Line_Y2 = ya + int(h() * 0.75) * Cos_val;
            fl_color(FL_WHITE);
            fl_line_style(FL_SOLID, 0.5, 0);
            fl_line(ponX1, ponY1, Line_X2, Line_Y2);

        }
        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 5.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));
            int ponX1 = xa + int(h() * 0.6) * Sine_val;
            int ponY1 = ya + int(h() * 0.6) * Cos_val;
            int Line_X2 = xa + int(h() * 0.64) * Sine_val;
            int Line_Y2 = ya + int(h() * 0.64) * Cos_val;
            fl_color(FL_WHITE);
            fl_line_style(FL_SOLID, 1, 0);
            fl_line(ponX1, ponY1, Line_X2, Line_Y2);
        }
        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 10.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));
            int ponX1 = xa + int(h() * 0.56) * Sine_val;
            int ponY1 = ya + int(h() * 0.56) * Cos_val;
            int Line_X2 = xa + int(h() * 0.53) * Sine_val;
            int Line_Y2 = ya + int(h() * 0.53) * Cos_val;
            fl_color(FL_WHITE);
            fl_line_style(FL_SOLID, 1, 0);
            fl_line(ponX1, ponY1, Line_X2, Line_Y2);
        }
        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 10.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));
            int ponX1 = xa + int(h() * 0.3) * Sine_val;
            int ponY1 = ya + int(h() * 0.3) * Cos_val;
            int Line_X2 = xa + int(h() * 0.34) * Sine_val;
            int Line_Y2 = ya + int(h() * 0.34) * Cos_val;
            fl_color(FL_WHITE);
            fl_line_style(FL_SOLID, 1, 0);
            fl_line(ponX1, ponY1, Line_X2, Line_Y2);
        }



    }
    void draw_points()
    {
        int num = 3;
        int xa = x() + (w() / 2);
        int ya = y() + (h());
        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 20.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));
            int num_x = xa + int(h() * 0.92) * Sine_val;
            int num_y = ya + int(h() * 0.92) * Cos_val;
            fl_color(FL_WHITE);
            fl_font(FL_TIMES, 9);
            fl_draw(angle-180,ang[num], num_x, num_y);
            num += 1;

        }

        num = 7;

        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 30.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));
            int num_x = xa + int(h() * 0.70) * Sine_val;
            int num_y = ya + int(h() * 0.70) * Cos_val;
            fl_color(FL_WHITE);
            fl_font(FL_TIMES, 9);
            fl_draw(angle - 180,ang[num], num_x, num_y);
            num += 1;

        }
        num = 7;
        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 20.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));
            int num_x = xa + int(h() * 0.64) * Sine_val;
            int num_y = ya + int(h() * 0.64) * Cos_val;
            fl_color(FL_WHITE);
            fl_font(FL_TIMES, 9);
            fl_draw(angle - 180,ang[num], num_x, num_y);
            num += 1;

        }
        num = 7;
        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 30.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));
            int num_x = xa + int(h() * 0.5) * Sine_val;
            int num_y = ya + int(h() * 0.5) * Cos_val;
            fl_color(FL_WHITE);
            fl_font(FL_TIMES, 9);
            fl_draw(angle - 180,ang[num], num_x, num_y);
            num += 1;

        }
        num = 7;
        for (float angle = 150 + 90; angle >= 30 + 90; angle -= 20.0)
        {
            const float Sine_val = sin(Radian(angle)), Cos_val = cos(Radian(angle));
            int num_x = xa + int(h() * 0.35) * Sine_val;
            int num_y = ya + int(h() * 0.35) * Cos_val;
            fl_color(FL_WHITE);
            fl_font(FL_TIMES, 9);
            fl_draw(angle - 180,ang[num], num_x, num_y);
            num += 1;

        }

    }

    void draw_pointer(int xc, int yc, int radius, float f) {
        const float ang = f * 3.142 / 180;
        const float xa = sin(ang), ya = cos(ang);

        int x2 = xc + xa * (radius * .05);        // wht thin needle / wht fat pointer
        int y2 = yc + ya * (radius * .05);
        int x3 = xc + xa * (radius * .96);        // wht fat pointer / taper..
        int y3 = yc + ya * (radius * .96);
        int x4 = xc + xa * (radius * .98);
        int y4 = yc + ya * (radius * .98);
        int x5 = xc + xa * (radius * 1);
        int y5 = yc + ya * (radius * 1);
        fl_translate(w() / 2.0, h());
        fl_rotate(-360);
        fl_translate(-(w() / 2.0), -(h()));
        // Draw needle
        fl_color(FL_WHITE);
        fl_line_style(FL_SOLID, 2, 0);  fl_line(xc, yc, x2, y2);
        fl_color(FL_GREEN);
        fl_line_style(FL_SOLID, 2, 0);  fl_line(x2, y2, x3, y3);
        fl_line_style(FL_SOLID, 4, 0);  fl_line(x3, y3, x4, y4);
        fl_line_style(FL_SOLID, 2, 0);  fl_line(x4, y4, x5, y5);
        fl_line_style(0);

    }
    void draw_pointer_black(int xc, int yc, int radius, float f) {
        const float ang = f * 3.142 / 180;
        const float xa = sin(ang), ya = cos(ang);

        int x2 = xc + xa * (radius * .05);        // wht thin needle / wht fat pointer
        int y2 = yc + ya * (radius * .05);
        int x3 = xc + xa * (radius * .96);        // wht fat pointer / taper..
        int y3 = yc + ya * (radius * .96);
        int x4 = xc + xa * (radius * .98);
        int y4 = yc + ya * (radius * .98);
        int x5 = xc + xa * (radius * 1);
        int y5 = yc + ya * (radius * 1);
        fl_translate(w() / 2.0, h());
        fl_rotate(-360);
        fl_translate(-(w() / 2.0), -(h()));
        // Draw needle
        fl_color(FL_WHITE);
        fl_line_style(FL_SOLID, 2, 0);  fl_line(xc, yc, x2, y2);
        fl_color(FL_BLACK);
        fl_line_style(FL_SOLID, 2, 0);  fl_line(x2, y2, x3, y3);
        fl_line_style(FL_SOLID, 4, 0);  fl_line(x3, y3, x4, y4);
        fl_line_style(FL_SOLID, 2, 0);  fl_line(x4, y4, x5, y5);
        fl_line_style(0);

    }
   
};



int temp = 0;
class cFreqControl : public Fl_Group
{
    //friend void cbSelectDigit(Fl_Widget* btn, void* nbr);

private:
    const int bsize = 50;
    int x_pos = x(), y_pos = y();
    Fl_Button* b;
    Fl_Box* Pn;
    static void Button_CB(Fl_Widget* w, void* data) {
        Fl_Button* b = (Fl_Button*)w;
        fprintf(stderr, "Button '%s' was %s\n", b->label(), b->value() ? "Pushed" : "Released");
    }
public:
   
    
    cFreqControl(int x, int y, int w, int h, const char* lbl) :
        Fl_Group(x, y, w, h, "")
    {
        begin();
        printf("x = %d,y = %d\n", x, y);
        
        b = create_button(x_pos, y_pos, 50, 70, "1", 90, FL_FLAT_BOX   ); x_pos += b->w();
        b = create_button(x_pos, y_pos, 50, 70, "2", 90, FL_FLAT_BOX ); x_pos += b->w();
        Pn = new Fl_Box(x_pos, y_pos, 20, 70, "."); Pn->box(FL_FLAT_BOX); Pn->labelfont(FL_BOLD);
        Pn->labelsize(90); Pn->labelcolor(FL_WHITE); Pn->color(FL_BLACK); Pn->callback(Button_CB); x_pos += Pn->w();
        b = create_button(x_pos, y_pos, 50, 70, "3", 90, FL_FLAT_BOX); x_pos += b->w();
        b = create_button(x_pos, y_pos, 50, 70, "4", 90, FL_FLAT_BOX); x_pos += b->w();
        Pn = new Fl_Box(x_pos, y_pos, 20, 70, "."); Pn->box(FL_FLAT_BOX); Pn->labelfont(FL_BOLD);
        Pn->labelsize(90); Pn->labelcolor(FL_WHITE); Pn->color(FL_BLACK); Pn->callback(Button_CB); x_pos += Pn->w();
        b = create_button(x_pos, y_pos, 50, 70, "5", 90, FL_FLAT_BOX); x_pos += b->w();
        b = create_button(x_pos, y_pos, 50, 70, "6", 90, FL_FLAT_BOX); x_pos += b->w();
        clear_visible_focus();
        end();
    }
    Fl_Button* create_button(int x, int y, int w, int h, const char* lab, int Lbsize, Fl_Boxtype bo)
    {
        Fl_Button* dit1 = new Fl_Button(x, y, w, h, lab);
        dit1->box(bo);
        dit1->labelfont(FL_BOLD);
        dit1->labelsize(Lbsize);
        dit1->labelcolor(FL_WHITE);
        dit1->color(FL_BLACK);
        dit1->callback(Button_CB);
        return dit1;
    }
    
};

class ConTr_But_Gp : public Fl_Group
{
    //friend void cbSelectDigit(Fl_Widget* btn, void* nbr);

private:
    const int bsize = 70;
    int colstart = x(),
        col = x(),
        row = y();
    Fl_Button* b;
    
    static void Button_CB(Fl_Widget* w, void* data) {
        Fl_Button* b = (Fl_Button*)w;
        fprintf(stderr, "Button '%s' was %s\n", b->label(), b->value() ? "Pushed" : "Released");
    }
public:


    ConTr_But_Gp(int x, int y, int w, int h, const char* lbl) :
        Fl_Group(x, y, w, h, "")
    {
        begin();
        //printf("x = %d,y = %d\n", x, y);

        b = create_button(col, row, bsize, bsize, "1", 18, FL_GTK_UP_BOX   ); col += b->w();
        b = create_button(col, row, bsize, bsize, "2", 18, FL_PLASTIC_UP_FRAME ); col += b->w();
        b = create_button(col, row, bsize, bsize, "3", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "4", 18, FL_DOWN_BOX); col = colstart; row += b->h(); 
        b = create_button(col, row, bsize, bsize, "5", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "6", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "7", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "8", 18, FL_DOWN_BOX); col = colstart; row += b->h();
        b = create_button(col, row, bsize, bsize, "9", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "10", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "11", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "12", 18, FL_DOWN_BOX); col = colstart; row += b->h();
       
        clear_visible_focus();
        end();
    }
    Fl_Button* create_button(int x, int y, int w, int h, const char* lab, int Lbsize, Fl_Boxtype bo)
    {
        Fl_Button* dit1 = new Fl_Button(x, y, w, h, lab);
        dit1->box(FL_GLEAM_UP_BOX );
        dit1->labelfont(FL_BOLD);
        dit1->labelsize(Lbsize);
        dit1->labelcolor(FL_WHITE);
        dit1->color(FL_BLACK);
        dit1->callback(Button_CB);
        return dit1;
    }

};
class fun_But_Gp : public Fl_Group
{
    //friend void cbSelectDigit(Fl_Widget* btn, void* nbr);

private:
    const int bsize = 60;
    int colstart = x(),
        col = x(),
        row = y();
    Fl_Button* b;

    static void Button_CB(Fl_Widget* w, void* data) {
        Fl_Button* b = (Fl_Button*)w;
        fprintf(stderr, "Button '%s' was %s\n", b->label(), b->value() ? "Pushed" : "Released");
    }
public:


    fun_But_Gp(int x, int y, int w, int h, const char* lbl) :
        Fl_Group(x, y, w, h, "")
    {
        begin();
        //printf("x = %d,y = %d\n", x, y);

        b = create_button(col, row, bsize, bsize, "1", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "2", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "3", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "4", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "5", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "6", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "7", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "8", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "9", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "10", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "11", 18, FL_DOWN_BOX); col += b->w();
        b = create_button(col, row, bsize, bsize, "12", 18, FL_DOWN_BOX); col += b->w();
        
        clear_visible_focus();
        end();
    }
    Fl_Button* create_button(int x, int y, int w, int h, const char* lab, int Lbsize, Fl_Boxtype bo)
    {
        Fl_Button* dit1 = new Fl_Button(x, y, w, h, lab);
        dit1->box(FL_GLEAM_UP_BOX);
        dit1->labelfont(FL_BOLD);
        dit1->labelsize(Lbsize);
        dit1->labelcolor(FL_WHITE);
        dit1->color(FL_BLACK);
        dit1->callback(Button_CB);
        return dit1;
    }

};
cFreqControl* fre = 0;
AnalogGraph* disp = 0;

void Timer_CB(void*) {
    static float valu = 150;
    static int fl = 0;
    
    if (fl == 0 && valu >= 28)
    {
        
        valu -= 2.0;
        if (valu == 28)
            fl = 1;
    }
    if (fl == 1 && valu <= 150)
    {
        //printf("hi");
        valu += 2.0;
        if (valu == 150)
            fl = 0;
    }

       
    //if (valu < 30)
    //    valu = 150;

    disp->value(valu);
    Fl::repeat_timeout(0.1, Timer_CB);
}

int main() {
	int i = 0;
    Fl_Window *win = new Fl_Window(800, 410);
	win->begin();
    Fl_Box* freq_Box = new Fl_Box(win->x(),win->y(), win->w(), win->h() ,"");
    freq_Box->box(FL_RFLAT_BOX);
    freq_Box->color(FL_BLACK);
    
    cFreqControl *fre = new cFreqControl(freq_Box->x()+60,freq_Box->y()+ 40, 350, 100,"6");
    ConTr_But_Gp *Buttons = new ConTr_But_Gp(480, 100, 280, 210, "");
    fun_But_Gp* FunButtons = new fun_But_Gp(50, Buttons->h()+Buttons->y()+10, 720, 160, "");
    disp = new AnalogGraph(50, 130, 300, 150);
    disp->labelsize(4);
    disp->value(150);
    
    //freq_Box->h()- 385
	Fl_Box* Rx_Tx = new Fl_Box(freq_Box->w()-75, freq_Box->y()+ 20, 50, 20,"Rx 1");
	//Rx_Tx->box(FL_PLASTIC_UP_BOX);
	Rx_Tx->labelcolor(FL_YELLOW);
	Rx_Tx->tooltip("Receive");
	Fl_Box* USB = new Fl_Box(Rx_Tx->x() - 50, freq_Box->y()+ 20, 50, 20, "USB");
	//USB->box(FL_PLASTIC_UP_BOX);
	USB->labelcolor(FL_YELLOW);
	Fl_Box* freq = new Fl_Box(USB->x(),Rx_Tx->y()+Rx_Tx->h(), 100, 20, "100 - 3000 Hz");
	freq->labelcolor(FL_YELLOW);
    
    
    win->color(FL_GRAY);
	win->end();
    win->show();
    Fl::add_timeout(0.1, Timer_CB);
    
    
    return(Fl::run());
}
