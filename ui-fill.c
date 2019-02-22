#include <stdio.h>
#include <string.h>
#include "space.h"
#include "matrix.h"
#include "ui-fill.h"

#define UI_FILL_ZOOMED_OUT_CUTOFF 10.0f

const char UI_FILL_VERTEX_SHADER_SOURCE[] =
"attribute vec4 position;\n"
"attribute vec2 gridPosition;\n"
"\n"
"uniform mat4 viewMatrix;\n"
"uniform vec2 gridSize;\n"
"uniform vec2 gridPositionOffset;\n"
"uniform float styleOffset;\n"
"\n"
"uniform sampler2D gridStyles;\n"
"uniform sampler2D fillColors;\n"
"\n"
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	vec2 styleCoord = (gridPosition + gridPositionOffset) / gridSize;\n"
"	float style = texture2D(gridStyles, styleCoord).a + styleOffset;\n"
"	color = texture2D(fillColors, vec2(style, 0.5));\n"
"	gl_Position = viewMatrix * position;\n"
"}\n";

i8 ui_fill_initialize(UiFill *ui, UiGrid *ui_grid)
{
	ui->ui_grid = ui_grid;

	////////////////////////
	// load/create/link

	GLuint vertex_shader = shader_load(GL_VERTEX_SHADER, UI_FILL_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);

	if (
		!shader_program_create(&ui->shader, vertex_shader, ui_grid->fragment_shader, __FILE__, __LINE__) ||
		!shader_program_link(&ui->shader, __FILE__, __LINE__)
	) {
		glDeleteShader(vertex_shader);

		return 0;
	}

	////////////////////
	// attributes

	ui->attributes.position = glGetAttribLocation(
			ui->shader.program,
			"position");

	ui->attributes.gridPosition = glGetAttribLocation(
			ui->shader.program,
			"gridPosition");

	////////////////////
	// uniforms

	ui->uniforms.viewMatrix = glGetUniformLocation(
			ui->shader.program,
			"viewMatrix");

	ui->uniforms.gridSize = glGetUniformLocation(
			ui->shader.program,
			"gridSize");

	ui->uniforms.gridPositionOffset = glGetUniformLocation(
			ui->shader.program,
			"gridPositionOffset");

	ui->uniforms.styleOffset = glGetUniformLocation(
			ui->shader.program,
			"styleOffset");

	ui->uniforms.gridStyles = glGetUniformLocation(
			ui->shader.program,
			"gridStyles");

	ui->uniforms.fillColors = glGetUniformLocation(
			ui->shader.program,
			"fillColors");

	//////////////////
	// mesh + buffers

	i32 size = UI_FILL_MESH_MAX_SIZE;

	for (i32 i = 0; i < UI_FILL_NUM_MESHES; ++i) {
		FillMesh *mesh = &ui->meshes[i];
		UiFillBuffers *buffers = &ui->buffers[i];

		fill_mesh_initialize(mesh, size, size);

		glGenBuffers(1, &buffers->vertices);
		glBindBuffer(GL_ARRAY_BUFFER, buffers->vertices);
		glBufferData(
				GL_ARRAY_BUFFER,
				mesh->vertices_length * sizeof *mesh->vertices,
				mesh->vertices,
				GL_STATIC_DRAW);

		glGenBuffers(1, &buffers->indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->indices);
		glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				mesh->indices_length * sizeof *mesh->indices,
				mesh->indices,
				GL_STATIC_DRAW);

		size >>= 1;
	}
	
	return 1;
}

void ui_fill_terminate(UiFill *ui)
{
	shader_program_delete(&ui->shader);
	glDeleteShader(ui->shader.vertex);

	for (i32 i = 0; i < UI_FILL_NUM_MESHES; ++i) {
		fill_mesh_terminate(&ui->meshes[i]);

		glDeleteBuffers(1, &ui->buffers[i].vertices);
		glDeleteBuffers(1, &ui->buffers[i].indices);
	}
}

static i32 ui_fill_draw_mesh_index(UiFill *ui, SizeQuad *draw_quad)
{
	for (i32 i = 0; i < UI_FILL_NUM_MESHES; ++i) {
		if (
			draw_quad->size.c > ui->meshes[i].size_c ||
			draw_quad->size.r > ui->meshes[i].size_r
		) {
			return i > 0 ? i - 1 : 0;
		}
	}

	return UI_FILL_NUM_MESHES - 1;
}

void ui_fill_draw(UiFill *ui, View *vw, Grid *g, Quad *viewport_quad)
{
	UiGrid *ui_grid = ui->ui_grid;

	glUseProgram(ui->shader.program);

	SizeQuad draw_quad;
	ui_grid_size_quad_for_draw(&draw_quad, &g->styles_quad, viewport_quad);

	i32 mesh_index = ui_fill_draw_mesh_index(ui, &draw_quad);
	FillMesh *mesh = &ui->meshes[mesh_index];
	UiFillBuffers *buffers = &ui->buffers[mesh_index];

	glBindBuffer(GL_ARRAY_BUFFER, buffers->vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->indices);

	ui_grid_update_view_matrix(ui_grid, vw);

	// Attributes

	glVertexAttribPointer(
			ui->attributes.position,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof mesh->vertices[0],
			0);
	glEnableVertexAttribArray(ui->attributes.position);

	glVertexAttrib4f(ui->attributes.position, 0.0f, 0.0f, 0.0f, 1.0f);

	glVertexAttribPointer(
			ui->attributes.gridPosition,
			2,
			GL_BYTE,
			GL_FALSE,
			sizeof mesh->vertices[0],
			(GLvoid *) (2 * sizeof mesh->vertices[0].x));
	glEnableVertexAttribArray(ui->attributes.gridPosition);

	// Uniforms + Textures

	glUniform2f(
			ui->uniforms.gridSize,
			(f32) g->storage_quad.size.c,
			(f32) g->storage_quad.size.r);

	f32 styleOffset = vw->scale < UI_FILL_ZOOMED_OUT_CUTOFF ? 0.25f : 0.0f;

	glUniform1f(
			ui->uniforms.styleOffset,
			styleOffset);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui_grid->textures.grid_styles);
	glUniform1i(ui->uniforms.gridStyles, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ui_grid->textures.fill_colors);
	glUniform1i(ui->uniforms.fillColors, 1);

	// Draw

	Hex draw_quad_min_offset = hex_sub(draw_quad.min, g->storage_quad.min);

	Hex h;
	mat4 *view_matrix = &ui_grid->view_matrix;
	f64 trans_x = ui_grid->translation_x;
	f64 trans_y = ui_grid->translation_y;
	f64 size_x = vw->viewport_size.x;
	f64 size_y = vw->viewport_size.y;

	for (h.r = 0; h.r < draw_quad.size.r; h.r += UI_FILL_MESH_MAX_SIZE) {
		for (h.c = 0; h.c < draw_quad.size.c; h.c += UI_FILL_MESH_MAX_SIZE) {
			vec2 v = space_hex_to_world(
					space_hex_to_vec(
					space_storage_to_hex(
					hex_add(h, draw_quad.min))));

			view_matrix->m[3][0] = (trans_x + v.x) / size_x;
			view_matrix->m[3][1] = (trans_y + v.y) / size_y;

			glUniformMatrix4fv(
					ui->uniforms.viewMatrix,
					1,
					GL_FALSE,
					(GLfloat*) &view_matrix->m[0][0]);

			Hex grid_offset = hex_add(h, draw_quad_min_offset);

			glUniform2f(
					ui->uniforms.gridPositionOffset,
					(f32) grid_offset.c,
					(f32) grid_offset.r);

			glDrawElements(
					GL_TRIANGLES,
					mesh->indices_length,
					GL_UNSIGNED_SHORT,
					0);
		}
	}
}
