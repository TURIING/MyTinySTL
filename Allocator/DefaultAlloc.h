//
// Created by TURIING on 2023/7/14.
//

#ifndef MYTINYSTL_DEFAULTALLOC_H
#define MYTINYSTL_DEFAULTALLOC_H

#include <cstddef>
#include <cassert>
#include <string.h>
#include "MallocAlloc.h"

namespace STL {
    constexpr int ALIGN = 8;                                                            // freelist每个下标对应内存块递增的大小
    constexpr int MAX_BYTES = 128;                                                      // freelist最后一个下标对应内存块的大小
    constexpr int NFREELISTS = MAX_BYTES / ALIGN;                                       // freelist数组的size
    /*
     * 二级空间配置器
     */
    class DefaultAlloc {
        union obj {
            union obj *free_list_link;                                                  // 指向下一个节点
            char client_data[1];                                                        // 指向资源
        };
    private:
        static char *m_startFree;                                                       // 内存池起始位置
        static char *m_endFree;                                                         // 内存池结束位置
        static size_t m_headSize;

        static obj *volatile m_freeList[NFREELISTS];
    private:
        /*
         * 通过字节数找到freelist对应下标
         */
        static size_t m_computeIndexForBytes(size_t _bytes) {
            return (_bytes + static_cast<size_t>(ALIGN) - 1) / static_cast<size_t>(ALIGN) - 1;
        }
        static void *m_refill(size_t _n);
        static char *m_chunkAlloc(size_t _size, int &_nobjs);
        /*
         * 将bytes上调至8的倍数
         */
        static size_t m_roundUp(size_t _bytes) {
            return ((_bytes + static_cast<size_t>(ALIGN) - 1) & ~(static_cast<size_t>(ALIGN) - 1));
        }
    public:
        static void *allocate(size_t _n);
        static void deallocate(void *_p, size_t _n);
        static void *reallocate(void *_p, size_t _old_sz, size_t _new_sz);
    };
}


#endif //MYTINYSTL_DEFAULTALLOC_H
