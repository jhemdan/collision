#include "renderer.h"
#include "sprite_graphic.h"
#include "tilemap_graphic.h"
#include "text_graphic.h"
#include "font.h"
#include "particle_graphic.h"

#include <GL/glew.h>

namespace jaw
{
	Renderer::Renderer()
	{

	}

	void Renderer::init()
	{
		sprite_shader.create("../assets/sprite.vert", "../assets/sprite.frag", { { "position", 0 },{ "tex_coords", 1 },{ "color", 2 } });
		text_shader.create("../assets/text.vert", "../assets/text.frag", { { "position", 0 },{ "tex_coords", 1 },{ "color", 2 } });

		proj_mat = vcm::orthographic(0, 800, 600, 0, -1, 1);
	}

	void Renderer::clean()
	{
		sprite_shader.destroy();
		text_shader.destroy();
	}

	void Renderer::render(SpriteGraphic* sprite, const vcm::mat4& transform)
	{
		if (!sprite->visible)
			return;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sprite->texture->id);

		glUseProgram(sprite_shader.id);

		/*
		vcm::mat4 tran_mat = vcm::mat4
		{
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ sprite->position.x, sprite->position.y, 0, 1 }
		};
		*/

		vcm::mat4 temp_mvp = proj_mat * transform;
		glUniformMatrix4fv(sprite_shader.get_uniform("mvp"), 1, GL_FALSE, &temp_mvp[0][0]);

		glUniform1i(sprite_shader.get_uniform("tex"), 0);
		glUniform2f(sprite_shader.get_uniform("quad_size"), (float)sprite->get_clip_rect().w, (float)sprite->get_clip_rect().h);
		glUniform4f(sprite_shader.get_uniform("my_color"), sprite->color.x, sprite->color.y, sprite->color.z, sprite->color.w);

		glBindBuffer(GL_ARRAY_BUFFER, sprite->model.mbuffers.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->model.mbuffers.ibo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(2 * sizeof(float)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(4 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, sprite->model.mbuffers.isize, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	void Renderer::render(TilemapGraphic* tilemap, const vcm::mat4& transform)
	{
		if (!tilemap->visible)
			return;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tilemap->texture->id);

		//using sprite_shader for now, might change later
		glUseProgram(sprite_shader.id);

		vcm::mat4 temp_mvp = proj_mat * transform;
		glUniformMatrix4fv(sprite_shader.get_uniform("mvp"), 1, GL_FALSE, &temp_mvp[0][0]);

		glUniform1i(sprite_shader.get_uniform("tex"), 0);
		glUniform2f(sprite_shader.get_uniform("quad_size"), (float)1, (float)1);
		glUniform4f(sprite_shader.get_uniform("my_color"), 1.0f, 1.0f, 1.0f, 1.0f);

		glBindBuffer(GL_ARRAY_BUFFER, tilemap->model.mbuffers.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tilemap->model.mbuffers.ibo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(2 * sizeof(float)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(4 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, tilemap->model.mbuffers.isize, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	void Renderer::render(TextGraphic* text, const vcm::mat4& transform)
	{
		if (!text->visible)
			return;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, text->font->texture.id);

		//using sprite_shader for now, might change later
		glUseProgram(text_shader.id);

		vcm::mat4 temp_mvp = proj_mat * transform;
		glUniformMatrix4fv(sprite_shader.get_uniform("mvp"), 1, GL_FALSE, &temp_mvp[0][0]);

		glUniform1i(sprite_shader.get_uniform("tex"), 0);
		glUniform2f(sprite_shader.get_uniform("quad_size"), (float)1, (float)1);

		glBindBuffer(GL_ARRAY_BUFFER, text->model.mbuffers.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, text->model.mbuffers.ibo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(2 * sizeof(float)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(4 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, text->model.mbuffers.isize, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	void Renderer::render(ParticleGraphic* particles, const vcm::mat4& transform)
	{
		if (!particles->visible)
			return;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, particles->texture->id);

		//using sprite_shader for now, might change later
		glUseProgram(sprite_shader.id);

		vcm::mat4 temp_mvp = proj_mat * transform;
		glUniformMatrix4fv(sprite_shader.get_uniform("mvp"), 1, GL_FALSE, &temp_mvp[0][0]);

		glUniform1i(sprite_shader.get_uniform("tex"), 0);
		glUniform2f(sprite_shader.get_uniform("quad_size"), (float)1, (float)1);
		glUniform4f(sprite_shader.get_uniform("my_color"), 1.0f, 1.0f, 1.0f, 1.0f);

		glBindBuffer(GL_ARRAY_BUFFER, particles->model.mbuffers.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particles->model.mbuffers.ibo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(2 * sizeof(float)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(4 * sizeof(float)));

		glDrawElements(GL_TRIANGLES, particles->model.mbuffers.isize, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}