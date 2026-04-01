#include "base.hpp"
template <class S> 
int halfPlane(Pnt<S> a) {
    if (cmp(a.y, 0) == 0) {
        return cmp(a.x, 0);
    }
    return cmp(a.y, 0); // 如果需要从负半轴开始则把这里改成负的
}
template <class S>
bool polarCmp(Pnt<S> a, Pnt<S> b) {
    if (halfPlane(a) != halfPlane(b)) {
        return halfPlane(a) > halfPlane(b);
    }
    if (sgn(crs(a, b)) == 0) {
        return len2(a) < len2(b);
    }
    return sgn(crs(a, b)) > 0;
}