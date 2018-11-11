#include <utility>

//
// Created by mats on 2018-11-06.
//

#include "mytextarea.h"
#include <pangomm/fontmap.h>
#include <iostream>
#include <functional>


void foo_commit_callback(GtkIMContext */*context*/,
                                 gchar        *str,
                                 gpointer      user_data) {
  auto length = strlen(str);
  auto textView = static_cast<MyTextView*>(user_data);
  textView->insert(str, length);
}


MyTextArea::MyTextArea(BaseObjectType* cobject,
                         const Glib::RefPtr<Gtk::Builder>& refBuilder)
        : Gtk::DrawingArea(cobject)
        , m_refBuilder(refBuilder)
{
  initialize();
}

MyTextArea::MyTextArea()
{
  initialize();
}

MyTextArea::~MyTextArea() = default;

void MyTextArea::set_file(const std::string& filename) {
  m_textView.set_file(filename);
  if (m_adjustment) {
    m_adjustment->set_upper(m_textView.size());
    m_adjustment->set_value(0);
  }
}

void MyTextArea::set_adjustment(Glib::RefPtr<Gtk::Adjustment> adjustment) {
  m_adjustment = std::move(adjustment);
  m_adjustment->configure(0, 0, 0, 1, 1, 1);
  m_adjustment->signal_value_changed().connect(sigc::mem_fun(*this, &MyTextArea::on_adjustment_value_changed));
}

void MyTextArea::initialize() {
  _font_description.set_family(_font_name);
  _font_description.set_weight(_font_weight);
  _font_description.set_size(_font_size * PANGO_SCALE);
  auto context = Gtk::Widget::get_pango_context();
  auto metrics = context->get_metrics(_font_description);
  auto ascent = metrics.get_ascent();
  auto descent = metrics.get_descent();
  _char_height = (ascent + descent) / PANGO_SCALE;
  _char_width = metrics.get_approximate_char_width() / PANGO_SCALE;

  add_events(Gdk::BUTTON_PRESS_MASK);
  add_events(Gdk::SCROLL_MASK);
  set_can_focus(true);
}


bool MyTextArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  double line_count = height / (double)_char_height;

  if (m_textView.update_display_size(line_count)) {
    m_adjustment->set_page_size(m_textView.display_count());
    m_adjustment->set_page_increment(m_textView.display_count());
    m_adjustment->set_upper(m_textView.size());
    m_adjustment->set_value(m_textView.get_display_top_line_position());
  }

  // Draw a white rectangle
  cr->set_source_rgb(1.0, 1.0, 1.0);
  cr->rectangle(0, 0, width, height);
  cr->fill();

  // and some black text
  cr->set_source_rgb(0.0, 0.0, 0.0);
  double y = 0;
  for (unsigned long i = 0; i < m_textView.display_count(); ++i) {
    draw_line(cr, i, 0, y);
    y += _char_height;
  }

  return Widget::on_draw(cr);
}

void MyTextArea::draw_line(const Cairo::RefPtr<Cairo::Context> &cr, unsigned long line_num, double x, double y) {

  auto layout = create_pango_layout(m_textView.get_line(line_num));
  layout->set_font_description(_font_description);
  int text_width;
  int text_height;

  auto attrs = Pango::AttrList();
  auto attr = Pango::Attribute::create_attr_background(0, 65535, 65535);
  attr.set_start_index(2);
  attr.set_end_index(4);
  attrs.insert(attr);
  layout->set_attributes(attrs);

  auto caret_column = m_textView.get_caret_pos(line_num);
  if (caret_column >= 0) {
    get_style_context()->render_insertion_cursor(cr, x, y, layout, caret_column, Pango::Direction::DIRECTION_LTR);
  }

  //get the text dimensions (it updates the variables -- by reference)
  layout->get_pixel_size(text_width, text_height);
  // Position the text in the middle
  cr->move_to(x, y);
  layout->show_in_cairo_context(cr);
}


bool MyTextArea::on_key_press_event(GdkEventKey *event) {
  if (event->keyval == GDK_KEY_Right) {
    m_textView.move_caret_right();
    queue_draw();
    return true;
  }
  else if (event->keyval == GDK_KEY_Left) {
    m_textView.move_caret_left();
    queue_draw();
    return true;
  }
  if (event->keyval == GDK_KEY_Up) {
    m_textView.move_caret_up();
    queue_draw();
    return true;
  }
  else if (event->keyval == GDK_KEY_Down) {
    m_textView.move_caret_down();
    queue_draw();
    return true;
  }
  else if (event->keyval == GDK_KEY_Delete) {
    m_textView.remove(false);
    queue_draw();
    return true;
  }
  else if (event->keyval == GDK_KEY_BackSpace) {
    m_textView.remove(true);
    queue_draw();
    return true;
  }

  return gtk_im_context_filter_keypress(m_imContext, event);
}




bool MyTextArea::on_button_press_event(GdkEventButton *button_event) {
  std::cout << "mouse button event" << std::endl;
  if ((button_event->type == GDK_BUTTON_PRESS) &&
          (button_event->button == 1)) {
    auto column = button_event->x / _char_width;
    auto line = button_event->y / _char_height;
    m_textView.set_caret_relative(line, (unsigned long)lround(column));
    queue_draw();

    return true;
  }
  return Widget::on_button_press_event(button_event);
}




void MyTextArea::on_realize() {
  using namespace std::placeholders;
  Widget::on_realize();
  Gdk::CursorType cursorType = Gdk::CursorType::XTERM;
  auto cursor = Gdk::Cursor::create(cursorType);
  auto window = get_window();
  window->set_cursor(cursor);

  m_imContext = gtk_im_multicontext_new();

  gtk_im_context_set_client_window(m_imContext, Glib::unwrap(window));
  gtk_im_context_focus_in(m_imContext);
  g_signal_connect(m_imContext, "commit",
                   G_CALLBACK(&MyTextArea::on_commit_callback), this);


}

void MyTextArea::on_adjustment_value_changed() {
  m_textView.update_top_line(m_adjustment->get_value());
  queue_draw();
}

bool MyTextArea::on_scroll_event(GdkEventScroll *scroll_event) {
  switch (scroll_event->direction) {
    case GdkScrollDirection::GDK_SCROLL_DOWN:
      m_adjustment->set_value(m_adjustment->get_value() + 3*m_adjustment->get_step_increment());
      break;
    case GdkScrollDirection::GDK_SCROLL_UP:
      m_adjustment->set_value(m_adjustment->get_value() - 3*m_adjustment->get_step_increment());
      break;
    default:
      break;
  }

  return Widget::on_scroll_event(scroll_event);
}

void MyTextArea::on_commit_callback(GtkIMContext *, gchar *str, gpointer user_data) {
  auto textArea = static_cast<MyTextArea*>(user_data);
  textArea->on_commit(str);
}

void MyTextArea::on_commit(gchar *str) {
  auto length = strlen(str);
  m_textView.insert(str, length);
  queue_draw();
}
