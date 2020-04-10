#pragma once
#include <map>

template<typename pI, typename T, typename F>
class xFactory {

protected:
	typedef std::map<T, F> FactoryMap;
	FactoryMap m_FactoryMap;

public:
	~xFactory() { m_FactoryMap.clear(); }

	void Register(const T & name, F pfnCreate) {
		m_FactoryMap[name] = pfnCreate;
	};

	template<typename... Args>
	pI Create(const T &name, Args&&... args) {
		auto it = m_FactoryMap.find(name);
        if (it != m_FactoryMap.end()) {
			return it->second(std::forward<Args>(args)...);
        }
		return nullptr;
	}
}; // class xFactory end
