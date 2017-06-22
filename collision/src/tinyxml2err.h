#pragma once

#include "exception.h"

#include <tinyxml2.h>

namespace jaw
{
	struct ErrException : Exception
	{
		ErrException(tinyxml2::XMLError err, const std::string& file)
		{
			msg = file + ": " + "TinyXML2 error: " + std::to_string((int)err);
		}
	};

	void txml_check_err(tinyxml2::XMLError err, const std::string& file);
}