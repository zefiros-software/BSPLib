# The Classic BSP Interface
Since BSP originally was a C library, the whole interface was designed 
to be used from C. We made the interface easier to work with from C++, 
but we also provide the classic interface implementation.

##Program Logic

| Classic                         | Modern                                       |
| ------------------------------- | -------------------------------------------- |
| [`bsp_init()`](logic/init.md)   | [`BSPLib::Classic::Init()`](logic/init.md)   |
| [`bsp_begin()`](logic/begin.md) | [`BSPLib::Classic::Begin()`](logic/begin.md) |
| [`bsp_end()`](logic/begin.md)   | [`BSPLib::Classic::End()`](logic/end.md)     |

##Utilities

| Classic                          | Modern                                |
| -------------------------------- | ------------------------------------- |
| [`bsp_pid()`](util/procid.md)    | [`BSPLib::ProcId()`](util/procid.md)  |
| [`bsp_nprocs()`](util/nprocs.md) | [`BSPLib::NProcs()`](util/nprocs.md)  |
| [`bsp_time()`](util/time.md)     | [`BSPLib::Time()`](util/time.md)      |

##Halting

| Classic                            | Modern                                          |
| ---------------------------------- | ----------------------------------------------- |
| [`bsp_abort()`](halting/abort.md)  | [`BSPLib::Classic::Abort()`](halting/abort.md)  |
| [`bsp_vabort()`](halting/abort.md) | [`BSPLib::Classic::VAbort()`](halting/abort.md) |


##Synchronisation Point

| Classic                      | Modern                            |
| ---------------------------- | --------------------------------- |
| [`bsp_sync()`](sync/sync.md) | [`BSPLib::Sync()`](sync/sync.md)  |

##Registration & Deregistration

| Classic                               | Modern                                |
| ------------------------------------- | ------------------------------------- |
| [`bsp_push_reg()`](regdereg/push.md)  | [`BSPLib::Push()`](regdereg/push.md)  |
| [`bsp_pop_reg()`](regdereg/pop.md)    | [`BSPLib::Pop()`](regdereg/pop.md)    |


##Communication

| Classic                   | Modern                            |
| ------------------------- | --------------------------------- |
| [`bsp_put()`](com/put.md) | [`BSPLib::Put()`](com/put.md) |
| [`bsp_get()`](com/get.md) | [`BSPLib::Get()`](com/get.md) |


##Messaging

| Classic                             | Modern                                  |
| ----------------------------------- | --------------------------------------- |
| [`bsp_send()`](messaging/send.md)   | [`BSPLib::Send()`](messaging/send.md)   |
| [`bsp_qsize()`](messaging/qsize.md) | [`BSPLib::QSize()`](messaging/qsize.md) |
| [`bsp_move()`](messaging/move.md)   | [`BSPLib::Move()`](messaging/move.md)   |


##Messaging Utilities

| Classic                                            | Modern                                                |
| -------------------------------------------------- | ----------------------------------------------------- |
| [`bsp_set_tagsize()`](messagingutil/settagsize.md) | [`BSPLib::SetTagSize()`](messagingutil/settagsize.md) |
| [`bsp_get_tag()`](messagingutil/gettag.md)         | [`BSPLib::GetTag()`](messagingutil/gettag.md)         |


## High Performance

| Classic                        | Modern                                |
| ------------------------------ | ------------------------------------- |
| [`bsp_hpmove()`](hp/hpmove.md) | [`BSPLib::HPMove()`](hp/hpmove.md)    |
| [`bsp_hpget()`](hp/hpget.md)   | [`BSPLib::HPGet()`](hp/hpget.md)      |
| [`bsp_hpput()`](hp/hpput.md)   | [`BSPLib::HPPut()`](hp/hpput.md)      |
| [`bsp_hpsend()`](hp/hpsend.md) | [`BSPLib::HPSend()`](hp/hpsend.md)    |