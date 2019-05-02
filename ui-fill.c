#include "ui-fill.h"
#include <stdio.h>
#include <string.h>

const char UI_FILL_VERTEX_SHADER_SOURCE[] =
"attribute vec2 position;\n"
"attribute vec2 gridPosition;\n"
"attribute vec2 positionOffset;\n"
"attribute vec2 gridPositionOffset;\n"
"\n"
"uniform mat4 viewMatrix;\n"
"uniform vec2 gridSize;\n"
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
"	gl_Position = viewMatrix * vec4(position + positionOffset, 0.0, 1.0);\n"
"}\n";

bool ui_fill_initialize(UiFill *ui, UiGrid *ui_grid)
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

		return false;
	}

	////////////////////
	// attributes

	ui->attributes.position = glGetAttribLocation(
			ui->shader.program,
			"position");

	ui->attributes.gridPosition = glGetAttribLocation(
			ui->shader.program,
			"gridPosition");

	ui->attributes.positionOffset = glGetAttribLocation(
			ui->shader.program,
			"positionOffset");

	ui->attributes.gridPositionOffset = glGetAttribLocation(
			ui->shader.program,
			"gridPositionOffset");

	////////////////////
	// uniforms

	ui->uniforms.viewMatrix = glGetUniformLocation(
			ui->shader.program,
			"viewMatrix");

	ui->uniforms.gridSize = glGetUniformLocation(
			ui->shader.program,
			"gridSize");

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

	int size = UI_FILL_MESH_MAX_SIZE;

	for (int i = 0; i < UI_FILL_NUM_MESHES; ++i) {
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

	instance_mesh_initialize(&ui->instance_mesh, 32);
	glGenBuffers(1, &ui->instanceBuffer);
	
	return true;
}

void ui_fill_terminate(UiFill *ui)
{
	shader_program_delete(&ui->shader);
	glDeleteShader(ui->shader.vertex);

	for (int i = 0; i < UI_FILL_NUM_MESHES; ++i) {
		fill_mesh_terminate(&ui->meshes[i]);

		glDeleteBuffers(1, &ui->buffers[i].vertices);
		glDeleteBuffers(1, &ui->buffers[i].indices);
	}

	instance_mesh_terminate(&ui->instance_mesh);
	glDeleteBuffers(1, &ui->instanceBuffer);
}

static int ui_fill_draw_mesh_index(UiFill *ui, SizeQuad *draw_quad)
{
	for (int i = 0; i < UI_FILL_NUM_MESHES; ++i) {
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

	int mesh_index = ui_fill_draw_mesh_index(ui, &draw_quad);
	FillMesh *mesh = &ui->meshes[mesh_index];
	UiFillBuffers *buffers = &ui->buffers[mesh_index];

	glBindBuffer(GL_ARRAY_BUFFER, buffers->vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->indices);

	// Attributes

	glVertexAttribPointer(
			ui->attributes.position,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof mesh->vertices[0],
			0);
	glEnableVertexAttribArray(ui->attributes.position);

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
			(float) g->storage_quad.size.c,
			(float) g->storage_quad.size.r);

	glUniform1f(
			ui->uniforms.styleOffset,
			0.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui_grid->textures.grid_styles);
	glUniform1i(ui->uniforms.gridStyles, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ui_grid->textures.fill_colors);
	glUniform1i(ui->uniforms.fillColors, 1);

	vec2 draw_offset = view_hex_to_world(
			hex_to_vec(
			hex_from_storage(draw_quad.min)));
	ui_grid_update_view_matrix(ui_grid, vw, draw_offset);

	glUniformMatrix4fv(
			ui->uniforms.viewMatrix,
			1,
			GL_FALSE,
			(GLfloat*) &ui->ui_grid->view_matrix.m[0][0]);

	// Instance buffer

	InstanceMesh *imesh = &ui->instance_mesh;

	int num_instances = (
		((draw_quad.size.r - 1) / UI_FILL_MESH_MAX_SIZE + 1) *
		((draw_quad.size.c - 1) / UI_FILL_MESH_MAX_SIZE + 1)
	);
	instance_mesh_resize(imesh, num_instances);

	Hex h;
	int i = 0;
	Hex draw_quad_min_offset = hex_sub(draw_quad.min, g->storage_quad.min);

	for (h.r = 0; h.r < draw_quad.size.r; h.r += UI_FILL_MESH_MAX_SIZE) {
		for (h.c = 0; h.c < draw_quad.size.c; h.c += UI_FILL_MESH_MAX_SIZE) {
			InstanceMeshVertex *vx = &imesh->vertices[i];

			vx->positionOffset = view_hex_to_world(
					hex_to_vec(
					hex_from_storage(h)));

			Hex grid_offset = hex_add(h, draw_quad_min_offset);
			vx->gridPositionOffset.x = grid_offset.c;
			vx->gridPositionOffset.y = grid_offset.r;
			i++;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, ui->instanceBuffer);
	glBufferData(
			GL_ARRAY_BUFFER,
			imesh->vertices_length * sizeof *imesh->vertices,
			imesh->vertices,
			GL_STREAM_DRAW);

	// Instance attributes

	glVertexAttribPointer(
			ui->attributes.positionOffset,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof imesh->vertices[0],
			0);
	glVertexAttribDivisor(ui->attributes.positionOffset, 1);
	glEnableVertexAttribArray(ui->attributes.positionOffset);

	glVertexAttribPointer(
			ui->attributes.gridPositionOffset,
			2,
			GL_SHORT,
			GL_FALSE,
			sizeof imesh->vertices[0],
			(GLvoid *) (2 * sizeof imesh->vertices[0].positionOffset.x));
	glVertexAttribDivisor(ui->attributes.gridPositionOffset, 1);
	glEnableVertexAttribArray(ui->attributes.gridPositionOffset);

	// Draw

	glDrawElementsInstanced(
			GL_TRIANGLES,
			mesh->indices_length,
			GL_UNSIGNED_SHORT,
			0,
			num_instances);
}
