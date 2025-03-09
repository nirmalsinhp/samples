#include <bits/stdc++.h>
using namespace std;
/*
- lock-based concurrent ds provides simple,intutive model for concurrency, but it can lead to deadlocks if used in-correctly, and limits concurrency based on granualarity of locks.
- Algorithms and data structures that use mutexes, condition variables, and futures to synchronize the data are called blocking data structures and algorithms.
- Data structures and algorithms that don’t use blocking library functions are said to be nonblocking.
- Obstruction-Free : If all other threads are paused, then any given thread will complete its operation in a bounded number of steps.
- Lock-Free : If multiple threads are operating on a data structure, then after a bounded number of steps one of them will complete its operation.
- Wait-Free : Every thread operating on a data structure will complete its operation in a bounded number of steps, even if other threads are also operating on the data structure.
- writing wait-free data structures correctly is extremely hard.
- the primary reason for using lock-free data structures is to enable maximum concurrency.
- robustness
- The flip side here is that if you can’t exclude threads from accessing the data structure, then you must be careful to ensure that the invariants are upheld or choose alternative invariants that can be upheld. Also, you must pay attention to the ordering constraints you impose on the operations. To avoid the undefined behavior associated with a data race, you must use atomic operations for the modifications. But that alone isn’t enough; you must ensure that changes become visible to other threads in the correct order. All this means that writing thread-safe data structures without using locks is considerably harder than writing them with locks.
- lock-free data structures rely on the use of atomic operations and the associated memory-ordering guarantees in order to ensure that data becomes visible to other threads in the correct order.
- The term hazard pointers is a reference to a technique discovered by Maged Michael.[1] They are so called because deleting a node that might still be referenced by other threads is hazardous.
- hazard pointers are patented, may not be able to use.
- aprt from hazard pointers, other techniques are using atomic shared pointers for head node.
- using reference count technique with atomic ints & internal/external ref count pairs.
# GuideLines
- use std::memory_order_seq_cst for prototyping
- use a lock-free memory reclamation scheme
- watch out for the ABA problem
- identify busy-wait loops and help the other thread
*/


template <typename T>
class lf_stack
{
private:
    struct node
    {
        shared_ptr<T> data;
        node* next;

        node(const T& data_) : data(make_shared<T>(data_))
        {}
    };
    atomic<node*> head;
    atomic<node*> to_be_deleted;
    atomic<unsigned> threads_in_pop;

    static void delete_nodes(node* ohead)
    {
        while (ohead)
        {
            node* next = ohead->next;
            delete ohead;
            ohead = next;
        }
    }
    void try_reclaim(node* old)
    {
        if (threads_in_pop == 1)
        {
            node* to_delete = to_be_deleted.exchange(nullptr);
            if (!--threads_in_pop)
            {
                delete_nodes(to_delete);
            }
            else if (to_delete)
            {
                chain_pending_nodes(to_delete);
            }
            delete old;
        }
        else
        {
            chain_pending_node(old);
            --threads_in_pop;
        }
    }

    void chain_pending_nodes(node* to_delete)
    {
        auto last = to_delete;
        while (node* const next = last->next)
        {
            last = next;
        }
        chain_pending_nodes(to_delete, last);
    }

    void chain_pending_nodes(node* first, node* last)
    {
        last->next = to_be_deleted;
        while (!to_be_deleted.compare_exchange_weak(last->next, first));
    }

    void chain_pending_node(node* old)
    {
        chain_pending_nodes(old, old);
    }

public:
    void push(const T& data)
    {
        node* const n_node = new node(data);
        n_node->next = head;
        while (!head.compare_exchange_weak(n_node->next, n_node));
    }

    shared_ptr<T> pop()
    {
        ++threads_in_pop;
        node* o_head = head.load();
        while (o_head && head.compare_exchange_weak(o_head, o_head->next));
        shared_ptr<T> res;
        if (o_head)
            res.swap(o_head->data);
        try_reclaim(o_head);
        return res;
    }
};

// below works fine foe single producer/single consumer.

template<typename T>
class lock_free_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        node* next;
        node() :
            next(nullptr)
        {}
    };
    std::atomic<node*> head;
    std::atomic<node*> tail;
    node* pop_head()
    {
        node* const old_head = head.load();
        if (old_head == tail.load())
        {
            return nullptr;
        }
        head.store(old_head->next);
        return old_head;
    }
public:
    lock_free_queue() :
        head(new node), tail(head.load())
    {}
    lock_free_queue(const lock_free_queue& other) = delete;
    lock_free_queue& operator=(const lock_free_queue& other) = delete;
    ~lock_free_queue()
    {
        while (node* const old_head = head.load())
        {
            head.store(old_head->next);
            delete old_head;
        }
    }
    std::shared_ptr<T> pop()
    {
        node* old_head = pop_head();
        if (!old_head)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(old_head->data);
        delete old_head;
        return res;
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
        node* p = new node;
        node* const old_tail = tail.load();
        old_tail->data.swap(new_data);
        old_tail->next = p;
        tail.store(p);
    }
};