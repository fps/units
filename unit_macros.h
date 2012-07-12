#ifndef FPS_IO_MACROS_HH
#define FPS_IO_MACROS_HH

#define TEMPLATE(name, T1) \
template<class T1> \
struct name { }; \
\
template<class T1> make_name(T1 t1) { return name<T1>(); }

#endif
