#ifndef FADEVALUE_H
#define FADEVALUE_H

#include "application/Events.h"

#include <GxMaths/GxBlendValue.h>

template<typename T> class FadeValue
{
public:
    explicit FadeValue(Events *events) : e(events), dur(0), p(0), prev(0) { }
    FadeValue(Events *events, const T &value) : e(events), v0(value), v1(value), dur(0), p(1), prev(0) { }
    FadeValue(Events *events, const T &start, const T &end, float dur) : e(events), v0(start), v1(end), dur(dur), p(0), prev(0) { }

    void setRange(const T &t0, const T &t1, float duration){ v0 = t0; v1 = t1; p = 0.0f; dur = duration; }

    T value() const { internal(); return Gx::Interpolator<T>()(v0, v1, p.value()); }
    T value(float blend) const { internal(); return Gx::Interpolator<T>()(v0, v1, p.value(blend)); }

private:
    void internal() const;

    Events *e;

    T v0, v1;
    float dur;

    mutable Gx::BlendFloat p;
    mutable float prev;
};

template<typename T> void FadeValue<T>::internal() const
{
    auto elapsed = e->time() - prev;
    prev = e->time();

    p.store();

    p.add(dur ? elapsed / dur : 0);
    if(p.value() > 1) p.set(1);
}

using FadeFloat = FadeValue<float>;

#endif // FADEVALUE_H
