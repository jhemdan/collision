#include "font.h"
#include "exception.h"
#include "tinyxml2err.h"

#include <tinyxml2.h>

namespace jaw
{
	Font::Font()
	{

	}

	void Font::create(const std::string& path)
	{
		tinyxml2::XMLDocument doc;

		auto err = doc.LoadFile(path.c_str());
		txml_check_err(err, path);

		auto font_elem = doc.FirstChildElement("font");
		if (font_elem)
		{
			auto common_elem = font_elem->FirstChildElement("common");
			if (common_elem)
			{
				int line_height, num_pages;

				err = common_elem->QueryIntAttribute("lineHeight", &line_height);
				txml_check_err(err, path);
				err = common_elem->QueryIntAttribute("pages", &num_pages);
				txml_check_err(err, path);

				if (num_pages == 1)
				{
					auto pages_elem = font_elem->FirstChildElement("pages");
					if (pages_elem)
					{
						auto page_elem = pages_elem->FirstChildElement();

						std::string image_path;

						image_path = page_elem->Attribute("file", nullptr);

						std::string my_path = path;
						int last_slash = -1;
						for (int i = 0; i < (int)my_path.size(); ++i)
						{
							if (my_path[i] == '\\')
								my_path[i] = '/';

							if (my_path[i] == '/')
								last_slash = i;
						}

						std::string final_path;

						if (last_slash != -1)
						{
							final_path = my_path.substr(0, last_slash + 1) + image_path;
						}
						else
						{
							final_path = image_path;
						}

						image.create(final_path);

						auto chars_elem = font_elem->FirstChildElement("chars");
						if (chars_elem)
						{
							for (auto c_elem = chars_elem->FirstChildElement(); c_elem != nullptr; c_elem = c_elem->NextSiblingElement())
							{
								Glyph glyph;

								err = c_elem->QueryIntAttribute("id", &glyph.id);
								txml_check_err(err, path);
								err = c_elem->QueryIntAttribute("x", &glyph.x);
								txml_check_err(err, path);
								err = c_elem->QueryIntAttribute("y", &glyph.y);
								txml_check_err(err, path);
								err = c_elem->QueryIntAttribute("width", &glyph.width);
								txml_check_err(err, path);
								err = c_elem->QueryIntAttribute("height", &glyph.height);
								txml_check_err(err, path);
								err = c_elem->QueryIntAttribute("xoffset", &glyph.xoffset);
								txml_check_err(err, path);
								err = c_elem->QueryIntAttribute("yoffset", &glyph.yoffset);
								txml_check_err(err, path);
								err = c_elem->QueryIntAttribute("xadvance", &glyph.xadvance);
								txml_check_err(err, path);

								glyphs[glyph.id] = glyph;
							}
						}

						auto kernings_elem = font_elem->FirstChildElement("kernings");
						if (kernings_elem)
						{
							for (auto k_elem = kernings_elem->FirstChildElement(); k_elem != nullptr; k_elem = k_elem->NextSiblingElement())
							{
								Kerning kerning;

								err = k_elem->QueryIntAttribute("first", &kerning.first);
								txml_check_err(err, path);
								err = k_elem->QueryIntAttribute("second", &kerning.second);
								txml_check_err(err, path);
								err = k_elem->QueryIntAttribute("amount", &kerning.amount);
								txml_check_err(err, path);

								kernings[{kerning.first, kerning.second}] = kerning;
							}
						}
					}
				}
				else
				{
					throw Exception("Bad number of pages in font file " + path);
				}
			}
		}

		if (glyphs.empty())
			throw Exception("Bad font file " + path);
	}

	void Font::destroy()
	{
		image.destroy();
		glyphs.clear();
		kernings.clear();
	}
}