#include "include/controller/twelvedata/TAuthentication.h"
#include <controller/WinHttp.h>
#include <utility/Toolbox.h>
#include <model/twelvedata/TError.h>

void const TAuthentication::authenticate()
{
	WinHttp connection;
	std::wstring apiKey = m_configuraiton->getSecretKey();
	std::wstring path = L"/time_series?symbol=TEST_SYMBOL&interval=1day&apikey=" + apiKey;

	connection.open();
	connection.connect(L"api.twelvedata.com");
	connection.requestHandler(L"GET", path.c_str());
	connection.sendRequest();

	std::wstring wResponse = connection.recieveResponse();

	nlohmann::json jRes = nlohmann::json::parse(wResponse);
	TError ans = jRes.template get<TError>();

	if(ans.code == 404)
		m_isAuthenticated = true;
	else
		throw AuthenticationError();

	m_auth = new TAuth(toString(apiKey));
	OutputDebugStringW(L"Twelvedata account authenticated.\n");

}
