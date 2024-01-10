#include"Event.h"

template <class... ArgTypes>
Engine::listenerID Engine::Event<ArgTypes...>::AddListenerID(Callback callback)
{
	listenerID listenerID = m_activeIDCount++;
	m_callback.emplace(listenerID, callback);
	return listenerID;
}

template <class... ArgTypes>
Engine::listenerID Engine::Event<ArgTypes...>::operator +=(Callback callback)
{
	return AddListenerID(callback);
}

template <class... ArgTypes>
bool Engine::Event<ArgTypes...>::RemoveListenerID(listenerID listenerID)
{
	return m_callback.erase(listenerID);
}

template <class... ArgTypes>
bool Engine::Event<ArgTypes...>::operator -=(listenerID listenerID)
{
	return RemoveListenerID(listenerID);
}

template <class... ArgTypes>
void Engine::Event<ArgTypes...>::RemoveAllListeners()
{
	m_callback.clear();
}

template <class... ArgTypes>
Engine::listenerID Engine::Event<ArgTypes...>::GetListenerCount()
{
	return m_activeIDCount;
}

template <class... ArgTypes>
void Engine::Event<ArgTypes...>::Invoke(ArgTypes... arg)
{
	for (auto const& [key, value] : m_callback)
		value(arg...);
}