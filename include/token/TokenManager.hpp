#ifndef __TokenManager__
#define __TokenManager__
#include <map>
#include <token/TokenInterface.hpp>

namespace jub {


	class TokenManager
	{
	public:
		TokenManager();
		~TokenManager();
		bool enumToken();
		int getTokenCount() { return m_token_list.size(); };
		void getHandleList(JUB_UINT16 hlist[MAX_DEVICE]);
		TokenInterface* getToken(JUB_UINT16 index);
		void clearToken();

	private:
		bool _enumTokenHid();
		std::map<JUB_UINT16, TokenInterface*> m_token_list;
	};

}

#endif

