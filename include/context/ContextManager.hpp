#pragma once
#ifndef __ContextManager__
#define __ContextManager__
#include <map>
#include <JUB_SDK.h>

namespace jub {

	template <class T>
	class ContextManager
	{
	public:
		ContextManager() {};
		~ContextManager() {
			for (auto it : m_context_list)
			{
				delete it.second;
			}
		};

		JUB_UINT16 addContext(T* ctx) {

			JUB_UINT16 contextID = rand();
			m_context_list.insert(std::make_pair(contextID, ctx));
			return contextID;
		};

		void clearContext(JUB_UINT16 contextID)
		{
			auto it = m_context_list.find(contextID);
			if (it != m_context_list.end())
			{
				delete it->second;
				m_context_list.erase(it);
			}
		}

		T* getContext(JUB_UINT16 contextID)
		{
			auto it = m_context_list.find(contextID);
			if (it != m_context_list.end())
			{
				return it->second;
			}
			return nullptr;
		}


	private:
		std::map<JUB_UINT16, T*> m_context_list;
	};

}

#endif