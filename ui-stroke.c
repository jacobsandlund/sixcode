#include "ui-stroke.h"
#include <stdio.h>
#include <string.h>

const char UI_STROKE_VERTEX_SHADER_SOURCE[] =
"attribute vec4 position;\n"
"attribute vec2 gridPosition;\n"
"attribute vec2 gridPosition2;\n"
"\n"
"uniform mat4 viewMatrix;\n"
"uniform vec2 gridSize;\n"
"uniform vec2 gridPositionOffset;\n"
"uniform vec4 strokeColor;\n"
"\n"
"uniform sampler2D gridStyles;\n"
"\n"
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	vec2 styleCoord = (gridPosition + gridPositionOffset) / gridSize;\n"
"	vec2 styleCoord2 = (gridPosition2 + gridPositionOffset) / gridSize;\n"
"	float style = texture2D(gridStyles, styleCoord).a;\n"
"	float style2 = texture2D(gridStyles, styleCoord2).a;\n"
"\n"
"	bool hexEdge = style + style2 >= 0.5;\n"
"	color = hexEdge ? strokeColor : vec4(1.0);\n"
"\n"
"	gl_Position = viewMatrix * position;\n"
"}\n";

i8 ui_stroke_initialize(UiStroke *ui, UiGrid *ui_grid)
{
	ui->ui_grid = ui_grid;

	////////////////////////
	// load/create/link

	GLuint vertex_shader = shader_load(GL_VERTEX_SHADER, UI_STROKE_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);

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

	ui->attributes.gridPosition2 = glGetAttribLocation(
			ui->shader.program,
			"gridPosition2");

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

	ui->uniforms.strokeColor = glGetUniformLocation(
			ui->shader.program,
			"strokeColor");

	ui->uniforms.gridStyles = glGetUniformLocation(
			ui->shader.program,
			"gridStyles");

	//////////////////
	// mesh + buffers

	i32 size = UI_STROKE_MESH_MAX_SIZE;

	for (i32 i = 0; i < UI_STROKE_NUM_MESHES; ++i) {
		StrokeMesh *mesh = &ui->meshes[i];
		UiStrokeBuffers *buffers = &ui->buffers[i];

		stroke_mesh_initialize(mesh, size, size);

		glGenBuffers(1, &buffers->vertices);
		glBindBuffer(GL_ARRAY_BUFFER, buffers->vertices);
		glBufferData(
				GL_ARRAY_BUFFER,
				mesh->vertices_length * sizeof *mesh->vertices,
				mesh->vertices,
				GL_STATIC_DRAW);

		size >>= 1;
	}
	
	return 1;
}

void ui_stroke_terminate(UiStroke *ui)
{
	shader_program_delete(&ui->shader);
	glDeleteShader(ui->shader.vertex);

	for (i32 i = 0; i < UI_STROKE_NUM_MESHES; ++i) {
		stroke_mesh_terminate(&ui->meshes[i]);

		glDeleteBuffers(1, &ui->buffers[i].vertices);
	}
}

static i32 ui_stroke_draw_mesh_index(UiStroke *ui, SizeQuad *draw_quad)
{
	for (i32 i = 0; i < UI_STROKE_NUM_MESHES; ++i) {
		if (
			draw_quad->size.c > ui->meshes[i].size_c ||
			draw_quad->size.r > ui->meshes[i].size_r
		) {
			return i > 0 ? i - 1 : 0;
		}
	}

	return UI_STROKE_NUM_MESHES - 1;
}

void ui_stroke_draw(UiStroke *ui, View *vw, Grid *g, Quad *viewport_quad)
{
	UiGrid *ui_grid = ui->ui_grid;

	glUseProgram(ui->shader.program);

	SizeQuad draw_quad;
	ui_grid_size_quad_for_draw(&draw_quad, &g->styles_quad, viewport_quad);

	i32 mesh_index = ui_stroke_draw_mesh_index(ui, &draw_quad);
	StrokeMesh *mesh = &ui->meshes[mesh_index];
	UiStrokeBuffers *buffers = &ui->buffers[mesh_index];

	glBindBuffer(GL_ARRAY_BUFFER, buffers->vertices);

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

	glVertexAttribPointer(
			ui->attributes.gridPosition2,
			2,
			GL_BYTE,
			GL_FALSE,
			sizeof mesh->vertices[0],
			(GLvoid *) (
				2 * sizeof mesh->vertices[0].x +
				2 * sizeof mesh->vertices[0].c
			));
	glEnableVertexAttribArray(ui->attributes.gridPosition2);

	// Uniforms + Textures

	glUniform2f(
			ui->uniforms.gridSize,
			(f32) g->storage_quad.size.c,
			(f32) g->storage_quad.size.r);

	glUniform4f(
			ui->uniforms.strokeColor,
			0.2,
			0.2,
			0.2,
			1.0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui_grid->textures.grid_styles);
	glUniform1i(ui->uniforms.gridStyles, 0);

	// Draw

	Hex draw_quad_min_offset = hex_sub(draw_quad.min, g->storage_quad.min);

	Hex h;
	mat4 *view_matrix = &ui_grid->view_matrix;
	f64 trans_x = ui_grid->translation_x;
	f64 trans_y = ui_grid->translation_y;
	f64 size_x = vw->viewport_size.x;
	f64 size_y = vw->viewport_size.y;

	for (h.r = 0; h.r < draw_quad.size.r; h.r += UI_STROKE_MESH_MAX_SIZE) {
		for (h.c = 0; h.c < draw_quad.size.c; h.c += UI_STROKE_MESH_MAX_SIZE) {
			vec2 v = view_hex_to_world(
					hex_to_vec(
					hex_from_storage(
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

			glDrawArrays(
					GL_LINES,
					0,
					mesh->vertices_length);
		}
	}
}
