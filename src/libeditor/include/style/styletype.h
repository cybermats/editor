

#pragma once

class Style;
class Stylizer;

class StyleType {
 public:
  virtual ~StyleType() = default;

 protected:
  StyleType(unsigned long start_idx, unsigned long end_idx)
      : m_start_idx(start_idx), m_end_idx(end_idx) {}

 public:

  unsigned long get_start_idx() const {
    return m_start_idx;
  }

  unsigned long get_end_idx() const {
    return m_end_idx;
  }

 private:
  friend class Style;

  virtual void accept(Stylizer* stylizer) const = 0;

  virtual StyleType* clone() const = 0;

  unsigned long m_start_idx;
  unsigned long m_end_idx;
};


class ForegroundStyleType: public StyleType {
protected:
    ForegroundStyleType(unsigned char red, unsigned char green, unsigned char blue, unsigned long start, unsigned long end)
            : StyleType(start, end)
            , m_red(red), m_green(green), m_blue(blue)
    {}

public:
    unsigned char get_red() const {
      return m_red;
    }

    unsigned char get_green() const {
      return m_green;
    }

    unsigned char get_blue() const {
      return m_blue;
    }

    void accept(Stylizer* stylizer) const override;

private:
  friend class Style;
  StyleType* clone() const override {
    return new ForegroundStyleType(
        m_red, m_green, m_blue,
        get_start_idx(), get_end_idx());
  }

    unsigned char m_red;
    unsigned char m_green;
    unsigned char m_blue;
};

class BackgroundStyleType: public StyleType {
protected:
    BackgroundStyleType(unsigned char red, unsigned char green, unsigned char blue, unsigned long start, unsigned long end)
            : StyleType(start, end)
            , m_red(red), m_green(green), m_blue(blue)
    {}

public:
    unsigned char get_red() const {
      return m_red;
    }

    unsigned char get_green() const {
      return m_green;
    }

    unsigned char get_blue() const {
      return m_blue;
    }

    void accept(Stylizer* stylizer) const override;


private:
  friend class Style;
  StyleType* clone() const override {
    return new BackgroundStyleType(
        m_red, m_green, m_blue,
        get_start_idx(), get_end_idx());
  }

  unsigned char m_red;
  unsigned char m_green;
  unsigned char m_blue;
};

class BoldStyleType: public StyleType {
public:
    BoldStyleType(unsigned long start, unsigned long end)
    : StyleType(start, end) {}

    void accept(Stylizer* stylizer) const override;

 private:
  friend class Style;
  StyleType* clone() const override {
    return new BoldStyleType(
        get_start_idx(), get_end_idx());
  }


};
