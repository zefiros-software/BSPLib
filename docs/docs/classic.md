# The Classic BSP Interface
Since BSP originally was a C library, the whole interface was designed 
to be used from C. We made the interface easier to work with from C++, 
but we also provide the classic interface implementation.

[TOC]

##Program Logic

###bsp_init

**Interface**

```c++
void bsp_init(void*spmd(void), int32_t argc, char **argv)
```

**Details**

This is an alias of [`BSPLib::ClassicInit()`](logic.md#init).

###bsp_begin

**Interface**

```c++
void bsp_begin( uint32_t p )
```

**Details**

This is an alias of [`BSPLib::Classic::Begin()`](logic.md#begin).

###bsp_end

**Interface**

```c++
void bsp_end()
```

**Details**

This is an alias of [`BSPLib::Classic::End()`](logic.md#end).

##Utilities

###bsp_pid

**Interface**

```c++
uint32_t bsp_pid()
```

**Details**

This is an alias of [`BSPLib::Classic::ProcId()`](utility.md#procid).

###bsp_nprocs

**Interface**

```c++
uint32_t bsp_nprocs()
```

**Details**

This is an alias of [`BSPLib::Classic::NProcs()`](utility.md#nprocs).

###bsp_time

**Interface**

```c++
double bsp_time()
```

**Details**

This is an alias of [`BSPLib::Classic::Time()`](utility.md#time). 

---

##Halting

###bsp_abort

**Interface**

```c++
void bsp_abort(const char *errorMessage, ... )
```

**Parameters**

* `errorMessage`
* `...`

**Details**

This is an alias of `BSPLib::Classic::Abort()`.

###bsp_vabort

**Interface**

```c++
void bsp_vabort(const char *errorMessage, va_list args )
```

**Parameters**

* `errorMessage`
* `args`

**Details**

This is an alias of `BSPLib::Classic::VAbort()`.

---

##Synchronisation Point

###bsp_sync

**Interface**

```c++
void bsp_sync()
```

**Details**

This is an alias of `BSPLib::Classic::Sync()`.

---

##Registration & Deregistration

###bsp_push_reg

**Interface**

```c++
void bsp_push_reg( const void *ident, size_t size )
```

**Parameters**

* `ident`
* `size`

**Details**

This is an alias of `BSPLib::Classic::PushReg()`.

###bsp_pop_reg

**Interface**

```c++
void bsp_push_reg( const void *ident)
```

**Parameters**

* `ident`

**Details**

This is an alias of `BSPLib::Classic::PopReg()`.

---

##Communication

###bsp_put

**Interface**

```c++
void bsp_put( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
```

**Parameters**

* `pid`
* `src`
* `dst`
* `offset`
* `nbytes`

**Details**

This is an alias of `BSPLib::Classic::Put()`.

###bsp_get

**Interface**

```c++
void bsp_get( uint32_t pid, const void *src, ptrdiff_t offset, void *dst, size_t nbytes )
```

**Parameters**

* `pid`
* `src`
* `offset`
* `dst`
* `nbytes`

**Details**

This is an alias of `BSPLib::Classic::Get()`.

---

##Messaging

###bsp_send

**Interface**

```c++
void bsp_send( uint32_t pid, const void *tag, const void *payload, size_t size )
```

**Parameters**

* `pid`
* `tag`
* `payload`
* `size`

**Details**

This is an alias of `BSPLib::Classic::Send()`.

###bsp_qsize

**Interface**

```c++
void bsp_qsize( size_t *packets, size_t *accumulatedSize )
```

**Parameters**

* `packets`
* `accumulatedSize`

**Details**

This is an alias of `BSPLib::Classic::QSize()`.

###bsp_move

**Interface**

```c++
void bsp_move( void *payload, size_t maxCopySize )
```

**Parameters**

* `payload`
* `maxCopySize`

**Details**

This is an alias of `BSPLib::Classic::Move()`.

---

##Messaging Utilities

###bsp_set_tagsize

**Interface**

```c++
void bsp_set_tagsize( size_t *size )
```

**Parameters**

* `size`

**Details**

This is an alias of `BSPLib::Classic::SetTagSize()`.

###bsp_get_tag

**Interface**

```c++
void bsp_get_tag( size_t *status, void *tag )
```

**Parameters**

* `size`
* `tag`

**Details**

This is an alias of `BSPLib::Classic::GetTag()`.

---

## High Performance

###bsp_hpmove

**Interface**

```c++
void bsp_hpmove( void **tagPtr, void **payloadPtr )
```

**Parameters**

* `tagPtr`
* `payloadPtr`

**Details**

This is an alias of `BSPLib::Classic::HPMove()`.

###bsp_hpsend

**Interface**

```c++
void bsp_hpsend( uint32_t pid, const void *tag, const void *payload, size_t size )
```

**Parameters**

* `pid`
* `tag`
* `payload`
* `size`

**Details**

This is an alias of `BSPLib::Classic::HPSend()`.

###bsp_hpput

**Interface**

```c++
void bsp_hpput( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
```

**Parameters**

* `pid`
* `src`
* `dst`
* `offset`
* `nbytes`

**Details**

This is an alias of `BSPLib::Classic::HPPut()`.

###bsp_hpget

**Interface**

```c++
void bsp_hpget( uint32_t pid, const void *src, void *dst, ptrdiff_t offset, size_t nbytes )
```

**Parameters**

* `pid`
* `src`
* `dst`
* `offset`
* `nbytes`

**Details**

This is an alias of `BSPLib::Classic::HPGet()`.