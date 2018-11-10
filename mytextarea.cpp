//
// Created by mats on 2018-11-06.
//

#include "mytextarea.h"
#include <pangomm/fontmap.h>
#include <iostream>


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
  _buffer.load(filename);
  if (m_adjustment) {
    m_adjustment->set_upper(_buffer.line_count());
    m_adjustment->set_value(0);

  }
}

void MyTextArea::set_adjustment(Glib::RefPtr<Gtk::Adjustment> adjustment) {
  m_adjustment = adjustment;
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

  if (((unsigned long)line_count) > _current_line_count || _top_line != _top_line_new){
    _screen_lines.clear();
    _screen_line_sizes.clear();
    _top_line = _top_line_new;
    _current_line_count = (unsigned long)line_count;
    bool fraction = (line_count - _current_line_count) > 0.1;
    if (_current_line_count + _top_line >= _buffer.line_count()) {
      _current_line_count = _buffer.line_count() - _top_line;
      fraction = false;
    }

    auto range = _current_line_count;
    if (fraction)
      range++;

    for (unsigned long i = 0; i < range; ++i) {
      auto line_no = _top_line + i;
      auto line = _buffer.get_line(line_no);
      auto length = strlen(line);
      _screen_lines.push_back(std::vector<char>(line, line + length + 1));
      _screen_line_sizes.push_back(length);
    }

    m_adjustment->set_page_size(_current_line_count);
    m_adjustment->set_page_increment(_current_line_count);
  }

  // Draw a white rectangle
  cr->set_source_rgb(1.0, 1.0, 1.0);
  cr->rectangle(0, 0, width, height);
  cr->fill();

  // and some black text
  cr->set_source_rgb(0.0, 0.0, 0.0);
  double y = 0;
  for (unsigned long i = 0; i < _current_line_count; ++i) {
    draw_line(cr, i, 0, y);
    y += _char_height;
  }

  return Widget::on_draw(cr);
}

void MyTextArea::draw_line(const Cairo::RefPtr<Cairo::Context> &cr, unsigned long line_num, double x, double y) {

  auto line = &_screen_lines[line_num][0];
  auto layout = create_pango_layout(line);
  layout->set_font_description(_font_description);
  int text_width;
  int text_height;

  if ((line_num + _top_line) == _caret_line) {
    if (_caret_pos == END) {
      _caret_column = _screen_line_sizes[_caret_line - _top_line];
    }
    _caret_pos = KNOWN;
    auto caret_column = std::min(_caret_column, _screen_line_sizes[_caret_line - _top_line]);
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
    move_caret_right();
    queue_draw();
    return true;
  }
  else if (event->keyval == GDK_KEY_Left) {
    move_caret_left();
    queue_draw();
    return true;
  }
  if (event->keyval == GDK_KEY_Up) {
    move_caret_up();
    queue_draw();
    return true;
  }
  else if (event->keyval == GDK_KEY_Down) {
    move_caret_down();
    queue_draw();
    return true;
  }

  return Widget::on_key_press_event(event);
}

bool MyTextArea::move_caret_left() {
  if (_caret_column > 0) {
    _caret_column--;
  } else {
    if (move_caret_up())
      _caret_pos = END;
  }
  return true;
}

bool MyTextArea::move_caret_right() {
  if (_caret_column < _screen_line_sizes.at(_caret_line - _top_line)) {
    _caret_column++;
  } else {
    if (move_caret_down())
      _caret_column = 0;
  }
  return true;
}

bool MyTextArea::move_caret_up() {
  std::cout << "caret_line: " << _caret_line
            << ", top_line: " << _top_line
            << ", adjustment: " << m_adjustment->get_value()
            << ", current_line: " << _current_line_count;

  // The caret is above the current viewing window
  if (_caret_line < _top_line) {
    std::cout << ", above";
    m_adjustment->set_value(_caret_line-2);
  }
    // The caret is below the current viewing window
  else if (_caret_line > _top_line + _current_line_count) {
    auto top_line = _caret_line - _current_line_count + 1;
    std::cout << ", below, new top line: " << top_line;
    m_adjustment->set_value(top_line);
  }
    // The caret is within the current viewing window
  else {
    std::cout << ", within";
    if (_caret_line == _top_line) {
      std::cout << ", moving";
      if (_top_line > 0)
        m_adjustment->set_value(_top_line - 1);
      else
        m_adjustment->set_value(0);
    }
  }


  if (_caret_line == 0) {
    std::cout << ", at beginning" << std::endl;
    _caret_column = 0;
    return false;
  }
  _caret_line--;
  std::cout << "." << std::endl;
  return true;
}

bool MyTextArea::move_caret_down() {
  std::cout << "caret_line: " << _caret_line
            << ", top_line: " << _top_line
            << ", adjustment: " << m_adjustment->get_value()
            << ", current_line: " << _current_line_count;


  // The caret is above the current viewing window
  if (_caret_line < _top_line) {
    std::cout << ", above";
    m_adjustment->set_value(_caret_line);
  }
  // The caret is below the current viewing window
  else if (_caret_line > _top_line + _current_line_count) {
    auto top_line = _caret_line - _current_line_count + 3;
    std::cout << ", below, new top line: " << top_line;
    m_adjustment->set_value(top_line);
  }
  // The caret is within the current viewing window
  else {
    std::cout << ", within";
    if (_caret_line == _top_line + _current_line_count - 1) {
      std::cout << ", moving";
      m_adjustment->set_value(_top_line + 1);
    }
  }
  if (_caret_line == _buffer.line_count() - 1) {
    std::cout << ", at end" << std::endl;
    _caret_line = _buffer.line_count() - 1;
    _caret_pos = END;
    return false;
  }
  _caret_line++;
  std::cout << "." << std::endl;
  return true;
}


bool MyTextArea::on_button_press_event(GdkEventButton *button_event) {
  std::cout << "mouse button event" << std::endl;
  if ((button_event->type == GDK_BUTTON_PRESS) &&
          (button_event->button == 1)) {
    auto column = button_event->x / _char_width;
    auto line = button_event->y / _char_height;
    _caret_line = line + _top_line;
    _caret_column = (column + 0.5);
    queue_draw();

    return true;
  }
  return Widget::on_button_press_event(button_event);
}

void MyTextArea::on_realize() {
  Widget::on_realize();
  Gdk::CursorType cursorType = Gdk::CursorType::XTERM;
  auto cursor = Gdk::Cursor::create(cursorType);
  auto window = get_window();
  window->set_cursor(cursor);
}

void MyTextArea::on_adjustment_value_changed() {
  std::cout << "adjustment: " << m_adjustment->get_value()
  << ", line_count: " << _current_line_count
  << ", buffer.size(): " << _buffer.line_count()
  <<std::endl;
  _top_line_new = m_adjustment->get_value();
  queue_draw();
}

bool MyTextArea::on_scroll_event(GdkEventScroll *scroll_event) {
  std::cout << "Scroll event, direction: " << scroll_event->direction
          << ", delta_x: " << scroll_event->delta_x
          << ", delta_y: " << scroll_event->delta_y
  << std::endl;

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
