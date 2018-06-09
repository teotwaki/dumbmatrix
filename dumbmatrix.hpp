#ifndef DUMBMATRIX_HPP
#define DUMBMATRIX_HPP

#include <cstdint>

#include "matrix_config.hpp"

namespace dm {

struct RGB24 {
  using TSize = uint8_t;

  TSize r;
  TSize g;
  TSize b;
};

struct Point {
  using TSize = uint8_t;

  TSize x;
  TSize y;
};

class DumbMatrix {
  using TSize = uint8_t;

  private:
  static constexpr TSize _columns = 64;
  static constexpr TSize _rows    = 32;
  RGB24 _pixels[_rows][_columns];

  // GPIO setup
  void init_gpio() const;

  // runtime GPIO manipulation
  void gpio_select_row(const TSize row) const;

  void paint_row(const TSize row) const;

  public:
  DumbMatrix();

  void set(const Point p, const RGB24 c);
  void blank();
  void fill(const RGB24 c);

  void update() const;
};

} // namespace dm

#endif // DUMBMATRIX_HPP
