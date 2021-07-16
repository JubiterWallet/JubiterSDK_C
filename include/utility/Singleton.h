#pragma once
#ifndef COINPASS_SINGLETON_HPP
#define COINPASS_SINGLETON_HPP

#include <mutex>

template<typename T>
class Singleton {

public:
	template<typename... Args>
	static T* GetInstance(Args&&... args) {
		std::call_once(instanceFlag_, [&](){
			m_pInstance = new T(std::forward<Args>(args)...);
		});
		return m_pInstance;
	}

	static void DestroyInstance() {
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	Singleton(void);
	virtual ~Singleton(void);
	Singleton(const Singleton&);
	Singleton& operator = (const Singleton&);

private:
	static std::once_flag instanceFlag_;
	static T* m_pInstance;
}; // class Singleton end

template <class T> T* Singleton<T>::m_pInstance = nullptr;
template <class T> std::once_flag Singleton<T>::instanceFlag_;

#endif
