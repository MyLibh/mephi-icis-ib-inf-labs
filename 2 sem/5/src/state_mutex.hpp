#pragma once

#include <shared_mutex>
#include <condition_variable>
#include <atomic>

class state_mutex
{
private:
	using ul = std::unique_lock<std::mutex>;

public:
	state_mutex() :
		m_lmtx(),
		m_write_queue(),
		m_read_queue(),
		m_mutex(),
		m_has_wlock(false),
		m_has_xlock(false),
		m_readers(0)
	{ }

	void rlock()
	{
		ul lock(m_lmtx);
		m_read_queue.wait(lock, [&]() { return !(m_has_xlock); });

		m_mutex.lock_shared();
		
		++m_readers;
	}

	void runlock()
	{
		m_mutex.unlock_shared();

		if (!--m_readers)
			m_write_queue.notify_one();
	}

	void wlock()
	{
		ul lock(m_lmtx);
		m_write_queue.wait(lock, [&]() { return !(m_readers || m_has_wlock || m_has_xlock); });

		m_mutex.lock_shared();

		m_has_wlock = true;
	}

	void wunlock()
	{
		m_mutex.unlock_shared();

		m_has_wlock = false;

		m_write_queue.notify_all();
	}

	void xlock()
	{
		ul lock(m_lmtx);
		m_write_queue.wait(lock, [&]() { return !(m_readers || m_has_wlock || m_has_xlock); });
		
		m_mutex.lock();

		m_has_xlock = true;
	}

	void xunlock()
	{
		m_mutex.unlock();

		m_has_xlock = false;

		m_write_queue.notify_one();
		m_read_queue.notify_all();
	}

	void wlock_to_xlock()
	{
		ul lock(m_lmtx);
		
		while (m_readers);
		
		m_mutex.unlock_shared();
		m_mutex.lock();

		m_has_wlock = false;
		m_has_xlock = true;
	}

private:
	std::mutex              m_lmtx;
	std::condition_variable m_write_queue;
	std::condition_variable m_read_queue;
	std::shared_mutex       m_mutex;
	std::atomic_bool        m_has_wlock;
	std::atomic_bool        m_has_xlock;
	std::atomic_int         m_readers;
};

#pragma region r-lock

template <class _Mutex = state_mutex>
class r_lock
{
public:
	using mutex_type = _Mutex;

public:
	explicit r_lock(mutex_type& mutex) :
		m_mutex(std::addressof(mutex)),
		m_owns(true)
	{
		m_mutex->rlock();
	}

	r_lock(const r_lock& other) = delete;

	r_lock(r_lock&& other) noexcept :
		m_mutex(other.m_mutex),
		m_owns(other.m_owns)
	{
		other.m_mutex = nullptr;
		other.m_owns = false;
	}

	~r_lock() noexcept
	{
		if (m_owns)
			m_mutex->runlock();
	}

	r_lock& operator=(const r_lock& other) = delete;

	r_lock& operator=(r_lock&& other) noexcept
	{
		if (m_owns)
			m_mutex->runlock();

		m_mutex = other.m_mutex;
		m_owns = other.m_owns;
		other.m_mutex = nullptr;
		other.m_owns = false;

		return *this;
	}

	void unlock()
	{
		m_mutex->runlock();
		m_owns = false;
	}

private:
	mutex_type* m_mutex;
	bool        m_owns;
};
#pragma endregion

#pragma region w-lock

template <class _Mutex = state_mutex>
class w_lock
{
public:
	using mutex_type = _Mutex;

public:
	template<typename _Mutex>
	friend class x_lock;

public:
	explicit w_lock(mutex_type& mutex) :
		m_mutex(std::addressof(mutex)),
		m_owns(true)
	{
		m_mutex->wlock();
	}

	w_lock() noexcept :
		m_mutex(nullptr),
		m_owns(false)
	{ }

	w_lock(const w_lock& other) = delete;
	
	w_lock(w_lock&& other) noexcept :
		m_mutex(other.m_mutex),
		m_owns(other.m_owns)
	{
		other.m_mutex = nullptr;
		other.m_owns = false;
	}

	~w_lock() noexcept
	{
		if (m_owns)
			m_mutex->wunlock();
	}

	w_lock& operator=(const w_lock& other) = delete;

	w_lock& operator=(w_lock&& other) noexcept
	{
		if (m_owns)
			m_mutex->wunlock();

		m_mutex = other.m_mutex;
		m_owns = other.m_owns;
		other.m_mutex = nullptr;
		other.m_owns = false;

		return *this;
	}

	void lock()
	{
		m_mutex->wlock();
		m_owns = true;
	}

	void unlock()
	{
		m_mutex->wunlock();
		m_owns = false;
	}

private:
	mutex_type* m_mutex;
	bool        m_owns;
};

#pragma endregion

#pragma region x-lock

template <class _Mutex = state_mutex>
class x_lock
{
public:
	using mutex_type = _Mutex;

public:
	explicit x_lock(mutex_type& mutex) :
		m_mutex(std::addressof(mutex)),
		m_owns(true)
	{
		m_mutex->xlock();
	}

	x_lock() noexcept :
		m_mutex(nullptr),
		m_owns(false)
	{ }

	x_lock(const x_lock& other) = delete;

	x_lock(x_lock&& other) noexcept :
		m_mutex(other.m_mutex),
		m_owns(other.m_owns)
	{
		other.m_mutex = nullptr;
		other.m_owns = false;
	}

	x_lock(const w_lock<_Mutex>& other) = delete;

	x_lock(w_lock<_Mutex>&& other) noexcept :
		m_mutex(other.m_mutex),
		m_owns(other.m_owns)
	{
		other.m_mutex = nullptr;
		other.m_owns = false;

		if(m_mutex)
			m_mutex->wlock_to_xlock();
	}

	~x_lock() noexcept
	{
		if (m_owns)
			m_mutex->xunlock();
	}

	x_lock& operator=(const x_lock& other) = delete;

	x_lock& operator=(x_lock&& other) noexcept
	{
		if (m_owns)
			m_mutex->xunlock();

		m_mutex = other.m_mutex;
		m_owns = other.m_owns;
		other.m_mutex = nullptr;
		other.m_owns = false;

		return *this;
	}

	x_lock& operator=(const w_lock<_Mutex>& other) = delete;

	x_lock& operator=(w_lock<_Mutex>&& other) noexcept
	{
		if (m_owns)
			m_mutex->xunlock();

		m_mutex = other.m_mutex;
		m_owns = other.m_owns;
		other.m_mutex = nullptr;
		other.m_owns = false;

		if(m_mutex)
			m_mutex->wlock_to_xlock(); 

		return *this;
	}

	void lock()
	{
		m_mutex->xlock();
		m_owns = true;
	}

	void unlock()
	{
		m_mutex->xunlock();
		m_owns = false;
	}

private:
	mutex_type* m_mutex;
	bool        m_owns;
};

#pragma endregion