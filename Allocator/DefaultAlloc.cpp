//
// Created by TURIING on 2023/7/14.
//

#include "DefaultAlloc.h"

namespace STL {
    char *DefaultAlloc::m_startFree = nullptr;
    char *DefaultAlloc::m_endFree = nullptr;
    size_t DefaultAlloc::m_headSize = 0;
    DefaultAlloc::obj *volatile DefaultAlloc::m_freeList[NFREELISTS] = { nullptr };

    void *DefaultAlloc::allocate(size_t _n) {
        obj *volatile *myFreeList;
        obj *result;

        /* 如果欲分配大小超过128，则采用一级分配器 */
        if(_n > MAX_BYTES) return MallocAlloc::allocate(_n);
        /* 从自由链表里取出内存块 */
        myFreeList = m_freeList + m_computeIndexForBytes(_n);
        result = *myFreeList;
        // 未找到可用free_list，准备填充free_list
        if(result == nullptr) {
            void *r = m_refill(m_roundUp(_n));
            return r;
        }
        // 取出内存块
        *myFreeList = result->free_list_link;
        return result;
    }

    /*
     * 当free_list无可用区块时，重新填充空间
     * n: 每块内存块的大小
     */
    void *DefaultAlloc::m_refill(size_t _n) {
        int nobjs = 20;                                                                             // 欲从内存池获取的内存块数
        char *chunk = m_chunkAlloc(_n, nobjs);
        obj *volatile *myFreeList;
        obj *result, *currentObj, *nextObj;

        /* 若只获取了一个区块则直接分配给调用者，不加入free_list */
        if(nobjs == 1) return chunk;

        /* 如果返回了多块，则返回第一块，将剩余的内存块挂载到freelist相应下标中的链表 */
        myFreeList = m_freeList + m_computeIndexForBytes(_n);
        result = reinterpret_cast<obj *>(chunk);
        // chunk+n意味着跳过了第一块（即要分配出去那块）
        *myFreeList = nextObj = reinterpret_cast<obj *>(chunk + _n);
        // 将分配的内存，划分成内存块，并连接起来（从1开始，是因为第0块已经分配出去了）
        for(auto i = 1;; i++) {
            currentObj = nextObj;
            // 先转成char *是为了将步长调整为1字节
            nextObj = reinterpret_cast<obj *>(reinterpret_cast<char *>(nextObj) + _n);

            if(nobjs - 1 == i) {
                currentObj->free_list_link = nullptr;
                break;
            }else
                currentObj->free_list_link = nextObj;
        }

        return result;
    }

    /*
     * 从内存池中取出内存
     * size: 每块内存块的大小， n_objs: 传入参数，既是请求分配的块数也是最后反馈的分配块数
     */
    char *DefaultAlloc::m_chunkAlloc(size_t _size, int &_nobjs) {
        char *result;
        size_t totalBytes = _size * _nobjs;                                         // 请求分配的总大小
        size_t bytesLeft = m_endFree - m_startFree;                                 // 内存池剩余空间
        /* 剩余容量满足要求 */
        if(bytesLeft >= totalBytes) {
            result = m_startFree;
            m_startFree += totalBytes;
            return result;
        }
        /* 容量不足，但满足至少一个内存块 */
        else if(bytesLeft > _size) {
            _nobjs = static_cast<int>(bytesLeft / _size);
            totalBytes = _size * _nobjs;
            result = m_startFree;
            m_startFree += totalBytes;
            return result;
        }
        /* 连一个内存块都分配不出 */
        else {
            /* 打算在原来内存池已申请的内存大小的基础上多申请两倍所需内存, heap_size代表之前内存池已申请的大小，右移4位，相当于除以16，是为了内存对齐 */
            auto bytesToGet = 2 * totalBytes + m_roundUp(m_headSize >> 4);
            /* 内存池还有剩余空间，先分配给自由链表 */
            if(bytesLeft > 0) {
                obj *volatile *myFreeList = m_freeList + m_computeIndexForBytes(bytesLeft);
                reinterpret_cast<obj *>(m_startFree)->free_list_link = *myFreeList;
                *myFreeList = reinterpret_cast<obj *>(m_startFree);
            }
            /* 分配空间 */
            m_startFree = reinterpret_cast<char *>(malloc(bytesToGet));
            // 堆空间不足，malloc失败
            if(!m_startFree) {
                obj *volatile *myFreeList;
                obj *p;
                // 在free_list中检查是否有符合需求的区块
                for(auto i = _size; i <= static_cast<size_t>(MAX_BYTES); i += static_cast<size_t>(ALIGN)) {
                    myFreeList = m_freeList + m_computeIndexForBytes(i);
                    p = *myFreeList;
                    // 存在足以分配的区块
                    if(p) {
                        *myFreeList = p->free_list_link;
                        m_startFree = reinterpret_cast<char *>(p);
                        m_endFree = m_startFree + i;
                        return m_chunkAlloc(_size, _nobjs);
                    }
                }
                m_endFree = nullptr;
                m_startFree = reinterpret_cast<char *>(MallocAlloc::allocate(bytesToGet));
            }
            m_headSize += bytesToGet;
            m_endFree = m_startFree + bytesToGet;
            return m_chunkAlloc(_size, _nobjs);
        }
    }

    void DefaultAlloc::deallocate(void *_p, size_t _n) {
        assert(_p != nullptr);
        assert(_n > 0);

        /* 如果欲回收的内存大于128byte，则直接free */
        if(_n > MAX_BYTES)
            MallocAlloc::deallocate(_p, _n);
        /* 如果小于，则放回自由链表 */
        else {
            obj *volatile *myFreeList = m_freeList + m_computeIndexForBytes(_n);
            obj *q = reinterpret_cast<obj *>(_p);
            q->free_list_link = *myFreeList;
            *myFreeList = q;
        }
    }

    void *DefaultAlloc::reallocate(void *_p, size_t _old_size, size_t _new_size) {
        void *result;
        size_t copy_size;
        /* 超过128则调用realloc */
        if(_old_size > MAX_BYTES && _new_size > MAX_BYTES)
            return realloc(_p, _new_size);
        /* 欲扩容空间大小等于旧空间，则不需要分配 */
        if(m_roundUp(_old_size) == m_roundUp(_new_size))
            return _p;
        /* 从自由链表里申请内存 */
        result = allocate(_new_size);
        copy_size = _new_size > _old_size ? _old_size : _new_size;
        memcpy(result, _p, copy_size);
        deallocate(_p, _old_size);

        return result;
    }
}
