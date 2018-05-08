#ifndef __TokenManager__
#define __TokenManager__
#include <vector>
#include <token/TokenInterface.hpp>

namespace jub {


	class TokenManager
	{
	public:
		//TokenInterface* getToken(int index) {return 0};
		~TokenManager();
		bool enumToken();
		int getTokenCount() { return m_token_list.size(); };
		TokenInterface* getToken(JUB_UINT16 index) {
			if (index>m_token_list.size())
			{
				return nullptr;
			}
			return m_token_list[index];
		};

	private:
		bool _enumTokenHid();
		std::vector<TokenInterface*>  m_token_list;
	};

}

#endif

