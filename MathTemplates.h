#include <type_traits>

template<typename T>
inline T Min(const T& a, const T& b) { return (a < b)? a : b; }
 
template<typename T>
inline T Max(const T& a, const T& b) { return (a > b)? a : b; }
 
template<typename T>
inline T Clamp(const T& value, const T& lb, const T& ub) { return Max<T>(lb, Min<T>(ub, value)); }