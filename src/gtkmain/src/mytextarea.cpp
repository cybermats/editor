#include <utility>

//
// Created by mats on 2018-11-06.
//

#include "mytextarea.h"
#include <iostream>
#include <functional>
#include <formatter/regexformatter.h>
#include <formatter/selectionformatter.h>


MyTextArea::MyTextArea(BaseObjectType* cobject,
                         const Glib::RefPtr<Gtk::Builder>& refBuilder)
        : Gtk::DrawingArea(cobject)
        , m_refBuilder(refBuilder)
        , m_formatters()
        , m_stylizer()
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
  auto context = Gtk::Widget::get_pango_context();
  auto metrics = context->get_metrics(m_stylizer.get_font_description());
  auto ascent = metrics.get_ascent();
  auto descent = metrics.get_descent();
  _char_height = (ascent + descent) / PANGO_SCALE;
  _char_width = metrics.get_approximate_char_width() / PANGO_SCALE;

  add_events(Gdk::BUTTON_PRESS_MASK);
  add_events(Gdk::BUTTON_RELEASE_MASK);
  add_events(Gdk::SCROLL_MASK);
  add_events(Gdk::BUTTON1_MOTION_MASK);
  set_can_focus(true);


  m_stylizer.add_formatter(new RegexFormatter());
  m_stylizer.add_formatter(new SelectionFormatter(&m_textView));

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

void MyTextArea::draw_line(
    const Cairo::RefPtr<Cairo::Context> &cr,
    unsigned long display_line, double x, double y) {
  auto line = m_textView.get_line(display_line);
  auto layout = create_pango_layout(line);

  auto actual_line = m_textView.get_actual_line(display_line);

  m_stylizer.format(layout, actual_line, line);

  auto caret_column = m_textView.get_caret_pos(display_line);
  if (caret_column >= 0) {
    get_style_context()->render_insertion_cursor(cr, x, y, layout, caret_column, Pango::Direction::DIRECTION_LTR);
  }

  // Position the text in the middle
  cr->move_to(x, y);
  layout->show_in_cairo_context(cr);
}


bool MyTextArea::on_key_press_event(GdkEventKey *event) {
  std::cout << "key_press" << std::endl;
  if (event->keyval == GDK_KEY_Left) {
    m_textView.move_caret_backward("character");
    queue_draw();
    return true;
  }
  else if (event->keyval == GDK_KEY_Right) {
    m_textView.move_caret_forward("character");
    queue_draw();
    return true;
  }
  if (event->keyval == GDK_KEY_Up) {
    m_textView.move_caret_backward("line");
    queue_draw();
    return true;
  }
  else if (event->keyval == GDK_KEY_Down) {
    m_textView.move_caret_forward("line");
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

void MyTextArea::on_commit_callback(GtkIMContext *, gchar *str, gpointer user_data) {
  auto textArea = static_cast<MyTextArea*>(user_data);
  textArea->on_commit(str);
}

void MyTextArea::on_commit(gchar *str) {
  std::cout << "on_commit" << std::endl;
  auto length = strlen(str);
  m_textView.insert(str, length);
  queue_draw();
}

bool MyTextArea::on_button_press_event(GdkEventButton *button_event) {
  if ((button_event->type == GDK_BUTTON_PRESS) &&
      (button_event->button == 1)) {
    auto line = (unsigned long)(button_event->y / _char_height);
    auto column = (unsigned long)lround(button_event->x / _char_width);
    m_textView.set_caret_relative(line, column);
    m_textView.set_selection_start_relative(line, column);
    queue_draw();

    return true;
  }
  return Widget::on_button_press_event(button_event);
}

bool MyTextArea::on_button_release_event(GdkEventButton *button_event) {
  if ((button_event->type == GDK_BUTTON_RELEASE) &&
      (button_event->button == 1)) {
    auto line = (unsigned long)(button_event->y / _char_height);
    auto column = (unsigned long)lround(button_event->x /_char_width);
    m_textView.set_caret_relative(line, column);
    m_textView.set_selection_finish_relative(line, column);
    queue_draw();

    return true;
  }
  return Widget::on_button_release_event(button_event);
}

bool MyTextArea::on_motion_notify_event(GdkEventMotion *motion_event) {
  if (motion_event->state & GDK_BUTTON1_MASK) {
    auto line = (unsigned long)(motion_event->y / _char_height);
    auto column = (unsigned long)lround(motion_event->x / _char_width);
    m_textView.set_caret_relative(line, column);
    m_textView.set_selection_finish_relative(line, column);
    queue_draw();

    return true;
  }

  return Widget::on_motion_notify_event(motion_event);
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
