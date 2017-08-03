#pragma once

#include "exception.h"
#include "log.h"

#include <tinyxml2.h>

namespace jaw
{
	struct TinyXML2ErrException : Exception
	{
		TinyXML2ErrException(tinyxml2::XMLError err, const std::string& file)
		{
			msg = file + ": " + "TinyXML2 error: " + std::to_string((int)err);

			switch (err)
			{
			case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
				log_line("TinyXML2 error: FILE COULD NOT BE OPENED.");
				break;
			case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
				log_line("TinyXML2 error: FILE NOT FOUND.");
				break;
			case tinyxml2::XML_ERROR_FILE_READ_ERROR:
				log_line("TinyXML2 error: FILE READ ERROR.");
				break;
			default:
				log_line("TinyXML2 error: DEFAULT ERROR.");
				break;
			}
		}
	};

	void txml_check_err(tinyxml2::XMLError err, const std::string& file);
}