#pragma once
#ifndef __xManager__
#define __xManager__

#include <JUB_SDK.h>
#include <map>
#include <vector>
#include "util.h"

template <class T>
class xManager
{
public:
	xManager() {};
	~xManager() {
		for (auto it : _list)
		{
			delete it.second;
		}
	};

	JUB_UINT16 addOne(T* x) {

		JUB_UINT16 contextID = rand();
		_list.insert(std::make_pair(contextID, x));
		return contextID;
	};

    JUB_UINT16 addOne(JUB_UINT16 ID, T* x) {
        _list.insert(std::make_pair(ID, x));

        return ID;
    };

	void clearOne(JUB_UINT16 ID)
	{
		auto it = _list.find(ID);
		if (it != _list.end())
		{
			delete it->second;
			_list.erase(it);
		}
	}

	T* getOne(JUB_UINT16 ID)
	{
		auto it = _list.find(ID);
		if (it != _list.end())
		{
			return it->second;
		}
		return nullptr;
	}

	std::vector<JUB_UINT16> getHandleList() {
		std::vector<JUB_UINT16> h_list;
		for (auto it : _list)
		{
			h_list.push_back(it.first);
		}
		return h_list;
	}


private:
	std::map<JUB_UINT16, T*> _list;
};

#endif