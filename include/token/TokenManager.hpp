#ifndef __TokenManager__
#define __TokenManager__
#include <map>
#include <token/TokenInterface.hpp>

namespace jub {


	class TokenManager
	{
	public:
		//TokenInterface* getToken(int index) {return 0};
		~TokenManager();
		bool enumToken();
		int getTokenCount() { return m_token_list.size(); };
		void getHandleList(JUB_UINT16 hlist[MAX_DEVICE]) {
			int index = 0;
			for (auto it : m_token_list)
			{
				hlist[index] = it.first;
				index++;
			}
		};
		TokenInterface* getToken(JUB_UINT16 index) {
			auto it = m_token_list.find(index);
			if (it != m_token_list.end())
			{
				return it->second;
			}
			return nullptr;
		};
		void clearToken();

	private:
		bool _enumTokenHid();
		std::map<JUB_UINT16, TokenInterface*> m_token_list;
	};

}

#endif

