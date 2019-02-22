#include <stdio.h>
#include <string.h>
#include "space.h"
#include "matrix.h"
#include "ui-points.h"

#define UI_POINT_MESH_SIZE 256
#define UI_POINTS_POINT_SIZE_FACTOR 0.75

// 0.001953125 = 0.5 / 256
const char UI_POINTS_VERTEX_SHADER_SOURCE[] =
"attribute vec4 position;\n"
"attribute vec2 gridPosition;\n"
"\n"
"uniform mat4 viewMatrix;\n"
"uniform vec2 gridSize;\n"
"uniform vec2 gridPositionOffset;\n"
"uniform float pointSize;\n"
"\n"
"uniform sampler2D gridStyles;\n"
"uniform sampler2D fillColors;\n"
"\n"
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	vec2 styleCoord = (gridPosition + gridPositionOffset) / gridSize;\n"
"	float style = texture2D(gridStyles, styleCoord).a + 0.25;\n"
"	color = texture2D(fillColors, vec2(style, 0.5));\n"
"	gl_Position = viewMatrix * position;\n"
"	gl_PointSize = pointSize;\n"
"}\n";

i8 ui_points_initialize(UiPoints *ui, UiGrid *ui_grid)
{
	ui->ui_grid = ui_grid;

	////////////////////////
	// load/create/link

	GLuint vertex_shader = shader_load(GL_VERTEX_SHADER, UI_POINTS_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);

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

	ui->uniforms.pointSize = glGetUniformLocation(
			ui->shader.program,
			"pointSize");

	ui->uniforms.gridStyles = glGetUniformLocation(
			ui->shader.program,
			"gridStyles");

	ui->uniforms.fillColors = glGetUniformLocation(
			ui->shader.program,
			"fillColors");

	//////////////////
	// mesh + buffers

	points_mesh_initialize(&ui->mesh, UI_POINT_MESH_SIZE, UI_POINT_MESH_SIZE);

	glGenBuffers(1, &ui->buffers.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, ui->buffers.vertices);
	glBufferData(
			GL_ARRAY_BUFFER,
			ui->mesh.vertices_length * sizeof *ui->mesh.vertices,
			ui->mesh.vertices,
			GL_STATIC_DRAW);

	return 1;
}

void ui_points_terminate(UiPoints *ui)
{
	shader_program_delete(&ui->shader);
	glDeleteShader(ui->shader.vertex);
	glDeleteBuffers(1, &ui->buffers.vertices);

	points_mesh_terminate(&ui->mesh);
}

void ui_points_draw(UiPoints *ui, View *vw, Grid *g, Quad *viewport_quad)
{
	UiGrid *ui_grid = ui->ui_grid;

	glUseProgram(ui->shader.program);

	SizeQuad draw_quad;
	ui_grid_size_quad_for_draw(&draw_quad, &g->styles_quad, viewport_quad);

	glBindBuffer(GL_ARRAY_BUFFER, ui->buffers.vertices);

	ui_grid_update_view_matrix(ui_grid, vw);

	// Attributes

	glVertexAttribPointer(
			ui->attributes.position,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof ui->mesh.vertices[0],
			0);
	glEnableVertexAttribArray(ui->attributes.position);

	glVertexAttrib4f(ui->attributes.position, 0.0f, 0.0f, 0.0f, 1.0f);

	glVertexAttribPointer(
			ui->attributes.gridPosition,
			2,
			GL_SHORT,
			GL_FALSE,
			sizeof ui->mesh.vertices[0],
			(GLvoid *) (2 * sizeof ui->mesh.vertices[0].x));
	glEnableVertexAttribArray(ui->attributes.gridPosition);

	// Uniforms + Textures

	glUniform2f(
			ui->uniforms.gridSize,
			(f32) g->storage_quad.size.c,
			(f32) g->storage_quad.size.r);

	f32 pointSize = (f64) vw->scale * UI_POINTS_POINT_SIZE_FACTOR;

	glUniform1f(
			ui->uniforms.pointSize,
			pointSize);

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

	for (h.r = 0; h.r < draw_quad.size.r; h.r += UI_POINT_MESH_SIZE) {
		for (h.c = 0; h.c < draw_quad.size.c; h.c += UI_POINT_MESH_SIZE) {
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

			glDrawArrays(
					GL_POINTS,
					0,
					ui->mesh.vertices_length);
		}
	}
}
