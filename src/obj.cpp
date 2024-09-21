#include "obj.h"

ts::Singleton<Obj> a;
ts::Singleton<Obj,Foo> b;

template<> Obj* ts::Allocate<Obj,Foo>()
{
    return new Obj(47, "Obj,int");
}
