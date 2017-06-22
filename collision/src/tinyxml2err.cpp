#include "tinyxml2err.h"

namespace jaw
{
	void txml_check_err(tinyxml2::XMLError err, const std::string& file)
	{
		if (err)
			throw ErrException(err, file);
	};
}