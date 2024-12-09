#include "include/model/twelvedata/TError.h"

void from_json(const nlohmann::json& j, TError& e)
{
	if (!j.at("code").is_null())
		j.at("code").get_to(e.code);
	if (!j.at("message").is_null())
		j.at("message").get_to(e.message);
	if (!j.at("status").is_null())
		j.at("status").get_to(e.status);

}
