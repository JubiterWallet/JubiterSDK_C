#pragma once
#ifndef COINPASS_SINGLETON_HPP
#define COINPASS_SINGLETON_HPP


template<typename T>
class Singleton
{
public:
	template<typename... Args>
	static T* GetInstance(Args&&... args)
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T(std::forward<Args>(args)...);
		}
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
private:
	Singleton(void);
	virtual ~Singleton(void);
	Singleton(const Singleton&);
	Singleton& operator = (const Singleton&);
private:
	static T* m_pInstance;
};

template <class T> T* Singleton<T>::m_pInstance = nullptr;


#endif