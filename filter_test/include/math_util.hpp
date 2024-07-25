#pragma once
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ratio>
#include <vector>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace math_util
{

  /**
   * @brief 数学・物理定数
   *
   */
  inline namespace constants
  {
    /// 円周率
    constexpr double PI = 3.1415926535897932384626433832795;

    /// 円周率 / 2
    constexpr double HALF_PI = PI / 2.0;

    /// 円周率 * 2
    constexpr double TWO_PI = PI * 2.0;

    /// 円周率 / 4
    constexpr double QUARTER_PI = PI / 4.0;

    /// degree -> radians
    constexpr double DEG_TO_RAD = PI / 180.0;

    /// radian -> degree
    constexpr double RAD_TO_DEG = 180.0 / PI;
  } // namespace constants

  // Function
  constexpr inline bool approx_eq(const double a, const double b, double range = 1e-12) { return (std::abs(a - b) < range); }
  constexpr inline bool approx_zero(const double a, double range = 1e-12) { return (std::abs(a) < range); }

  /**
   * @brief
   *
   * @tparam T
   * @param x
   * @param min
   * @param max
   * @return true
   * @return false
   */
  template <typename T>
  static constexpr bool out_range(T x, T min, T max)
  {
    return ((min > x || x >= max) ? true : false);
  }

  // Vector
  //  conversion
  /**
   * @brief 二次元ベクトルの型の変換
   *
   * @param BEFORE_VECTOR
   * @return constexpr AFTER_VECTOR
   */
  template <class BEFORE_VECTOR, class AFTER_VECTOR>
  constexpr inline AFTER_VECTOR conversion_vector2(BEFORE_VECTOR in_v)
  {
    AFTER_VECTOR out_v;
    out_v.x = in_v.x;
    out_v.y = in_v.y;
    return out_v;
  }

  /**
   * @brief ２次元の点加算
   *
   * @param a
   * @param b
   * @return constexpr POINT
   */

  template <class POINT>
  constexpr inline POINT add(const POINT &a, const POINT &b)
  {
    POINT out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    return out;
  }

  /**
   * @brief ２次元の点減算
   *
   * @param a
   * @param b
   * @return constexpr POINT
   */

  template <class POINT>
  constexpr inline POINT sub(const POINT &a, const POINT &b)
  {
    POINT out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
  }

  // rotation
  /**
   * @brief ２次元の点回転
   *
   * @param in
   * @param theta
   * @return constexpr POINT
   */
  template <class POINT>
  constexpr inline POINT rotation(const POINT &in, double theta)
  {
    POINT out;
    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);
    out.x = in.x * cos_theta - in.y * sin_theta;
    out.y = in.x * sin_theta + in.y * cos_theta;
    return out;
  }

  // transform
  /**
   * @brief ２次元の相似変換
   *
   * @param in
   * @param s
   * @param theta
   * @return constexpr POINT
   */
  template <class POINT>
  constexpr inline POINT transform(const POINT &in, double s, double theta)
  {
    POINT out;
    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);
    out.x = s * (in.x * cos_theta - in.y * sin_theta);
    out.y = s * (in.x * sin_theta + in.y * cos_theta);
    return out;
  }

  /**
   * @brief ２次元の相似変換
   *
   * @param in
   * @param s
   * @param sin_theta
   * @param cos_theta
   * @return constexpr POINT
   */
  template <class POINT>
  constexpr inline POINT transform(const POINT &in, double s, double sin_theta, double cos_theta)
  {
    POINT out;
    out.x = s * (in.x * cos_theta - in.y * sin_theta);
    out.y = s * (in.x * sin_theta + in.y * cos_theta);
    return out;
  }

} // namespace math_util
