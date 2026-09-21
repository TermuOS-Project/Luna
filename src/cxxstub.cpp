#include <stddef.h>

extern "C" void *memcpy(void *d, const void *s, size_t n);
extern "C" void *memset(void *d, int c, size_t n);

void *operator new(size_t size)
{
    (void)size;
    return nullptr; /* no heap yet — don't new anything */
}

void *operator new[](size_t size)
{
    (void)size;
    return nullptr;
}

void operator delete(void *p) noexcept
{
    (void)p;
}

void operator delete(void *p, size_t) noexcept
{
    (void)p;
}

void operator delete[](void *p) noexcept
{
    (void)p;
}

void operator delete[](void *p, size_t) noexcept
{
    (void)p;
}

extern "C" void __cxa_pure_virtual(void)
{
    for (;;)
        ;
}
