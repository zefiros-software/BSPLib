# The Classic BSP Interface
Since BSP originally was a C library, the whole interface was designed 
to be used from C. We made the interface easier to work with from C++, 
but we also provide the classic interface implementation.

[TOC]

##Program Logic
###bsp_init
**Interface**
```c++
void bsp_init(void*spmd(void), int argc, char **argv)
```

**Parameters**

* `spmd`
* `argc`
* `argv`

**Details**

This is an alias of [`BSPLib::Init()`](localhost).

###bsp_begin
**Interface**
```c++
void bsp_begin( uint32_t p )
```

**Parameters**

* `p`

**Details**

This is an alias of [`BSPLib::Begin()`](localhost).

###bsp_end
**Interface**
```c++
void bsp_end()
```

**Details**

This is an alias of [`BSPLib::End()`](localhost).

##Utilities
###bsp_pid
**Interface**
```c++
size_t bsp_pid()
```

**Returns**

The id of the current processor.

**Details**

This is an alias of [`BSPLib::ProcId()`](localhost).

###bsp_nprocs
**Interface**
```c++
size_t bsp_nprocs()
```

**Returns**

The number of processors.

**Details**

This is an alias of [`BSPLib::NProcs()`](localhost).

###bsp_time
**Interface**
```c++
double bsp_time()
```

**Returns**

The the time in seconds for this processor since the beginning of computation.

**Details**

This is an alias of [`BSPLib::Time()`](localhost). It provides the runtime of the 
thread since the start of the program.

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

This is an alias of `BSPLib::Abort()`.

###bsp_vabort
**Interface**
```c++
void bsp_vabort(const char *errorMessage, va_list args )
```

**Parameters**

* `errorMessage`
* `args`

**Details**

This is an alias of `BSPLib::VAbort()`.

##Synchronisation Point
###bsp_sync
**Interface**
```c++
void bsp_sync()
```

**Details**

This is an alias of `BSPLib::Sync()`.


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

This is an alias of `BSPLib::PushReg()`.

###bsp_pop_reg
**Interface**
```c++
void bsp_push_reg( const void *ident)
```

**Parameters**

* `ident`

**Details**

This is an alias of `BSPLib::PopReg()`.

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

This is an alias of `BSPLib::Put()`.

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

This is an alias of `BSPLib::Get()`.

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

This is an alias of `BSPLib::Send()`.

###bsp_qsize
**Interface**
```c++
void bsp_qsize( size_t *packets, size_t *accumulatedSize )
```

**Parameters**

* `packets`
* `accumulatedSize`

**Details**

This is an alias of `BSPLib::QSize()`.

###bsp_move
**Interface**
```c++
void bsp_move( void *payload, size_t maxCopySize )
```

**Parameters**

* `payload`
* `maxCopySize`

**Details**

This is an alias of `BSPLib::Move()`.

##Messaging Utilities
###bsp_set_tagsize
**Interface**
```c++
void bsp_set_tagsize( size_t *size )
```

**Parameters**

* `size`

**Details**

This is an alias of `BSPLib::SetTagSize()`.

###bsp_get_tag
**Interface**
```c++
void bsp_get_tag( size_t *status, void *tag )
```

**Parameters**

* `size`
* `tag`

**Details**

This is an alias of `BSPLib::GetTag()`.

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

This is an alias of `BSPLib::HPMove()`.

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

This is an alias of `BSPLib::HPSend()`.

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

This is an alias of `BSPLib::HPPut()`.

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

This is an alias of `BSPLib::HPGet()`.