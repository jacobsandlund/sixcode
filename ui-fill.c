#include "ui-fill.h"
#include "quad.h"

#define UI_FILL_COLORS_COUNT 256
#define UI_FILL_COLOR_COMPONENTS_LENGTH 1024  // 256 * 4
#define UI_FILL_GRID_STYLES_BUFFER_CAPACITY_MAX 1048576	// 1 MB
#define UI_FILL_ALPHA_SCALE 8.0

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

const char UI_FILL_FRAGMENT_SHADER_SOURCE[] =
"varying lowp vec4 color;\n"
"\n"
"void main() {\n"
"	gl_FragColor = color;\n"
"}\n";

const u8 UI_EMPTY_FILL_COLOR_NO_BLEND[] = {
	88, 88, 88, 255,
};

const u8 UI_FILL_COLORS[UI_FILL_COLOR_COMPONENTS_LENGTH] = {
	100, 100, 100, 100,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,

	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,

	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,

	140, 255, 140, 255,
	140, 140, 255, 255,
	255, 255, 40, 255,
	255, 40, 255, 255,

	//64, 239, 233, 255,
	//190, 190, 190, 255,
	//255, 140, 140, 255,

	//40, 255, 255, 255,
	//255, 190, 90, 255,
	//255, 90, 190, 255,
	//190, 255, 90, 255,

	//90, 255, 190, 255,
	//190, 90, 255, 255,
	//90, 190, 255, 255,
	//220, 190, 140, 255,
};

bool ui_fill_initialize(UiFill *ui)
{
	ui->blend_enabled = true;

	////////////////////////
	// load/create/link

	GLuint vertex_shader = shader_load(GL_VERTEX_SHADER, UI_FILL_VERTEX_SHADER_SOURCE, __FILE__, __LINE__);
	GLuint fragment_shader = shader_load(GL_FRAGMENT_SHADER, UI_FILL_FRAGMENT_SHADER_SOURCE, __FILE__, __LINE__);

	if (!vertex_shader || !fragment_shader) {
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return false;
	}

	if (
		!shader_program_create(&ui->shader, vertex_shader, fragment_shader, __FILE__, __LINE__) ||
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

	////////////////
	// textures

	texture_initialize(&ui->grid_styles_texture, UI_FILL_GRID_STYLES_BUFFER_CAPACITY_MAX);
	texture_initialize(&ui->fill_colors_texture, 0);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			UI_FILL_COLORS_COUNT,
			1,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			UI_FILL_COLORS);

	//////////////////
	// mesh + buffers

	ViewLayout view_layouts[VIEW_NUM_LAYOUTS] = {
		VIEW_LAYOUT_HEX,
		VIEW_LAYOUT_RECT,
	};

	for (int i = 0; i < VIEW_NUM_LAYOUTS; i++) {
		UiFillLayoutData *layout = &ui->layouts[i];

		int size = UI_FILL_MESH_MAX_SIZE;

		for (int j = 0; j < UI_FILL_NUM_MESHES; ++j) {
			FillMesh *mesh = &layout->meshes[j];
			UiFillBuffers *buffers = &layout->buffers[j];

			fill_mesh_initialize(mesh, view_layouts[i], size, size);

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
	}

	instance_mesh_initialize(&ui->instance_mesh, 32);
	glGenBuffers(1, &ui->instanceBuffer);
	
	return true;
}

void ui_fill_terminate(UiFill *ui)
{
	shader_program_delete(&ui->shader);
	glDeleteShader(ui->shader.vertex);
	glDeleteShader(ui->shader.fragment);

	texture_terminate(&ui->grid_styles_texture);
	texture_terminate(&ui->fill_colors_texture);

	for (int i = 0; i < VIEW_NUM_LAYOUTS; i++) {
		for (int j = 0; j < UI_FILL_NUM_MESHES; j++) {
			fill_mesh_terminate(&ui->layouts[i].meshes[j]);

			glDeleteBuffers(1, &ui->layouts[i].buffers[j].vertices);
			glDeleteBuffers(1, &ui->layouts[i].buffers[j].indices);
		}
	}

	instance_mesh_terminate(&ui->instance_mesh);
	glDeleteBuffers(1, &ui->instanceBuffer);
}

static void ui_fill_size_quad_for_draw(SizeQuad *out_sq, Quad *grid_styles_quad, Quad *viewport_quad)
{
	Quad intersect_quad;
	quad_intersect(&intersect_quad, viewport_quad, grid_styles_quad);
	quad_to_size_quad(out_sq, &intersect_quad);
	size_quad_even_align(out_sq, out_sq);
}

static int ui_fill_draw_mesh_index(UiFill *ui, SizeQuad *draw_quad)
{
	for (int i = 0; i < UI_FILL_NUM_MESHES; ++i) {
		if (
			draw_quad->size.x > ui->layouts[0].meshes[i].size_x ||
			draw_quad->size.y > ui->layouts[0].meshes[i].size_y
		) {
			return i > 0 ? i - 1 : 0;
		}
	}

	return UI_FILL_NUM_MESHES - 1;
}

void ui_fill_draw(UiFill *ui, View *vw, Grid *g)
{
	// Setup

	glViewport(0, 0, vw->viewport_size.x, vw->viewport_size.y);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(ui->shader.program);

	Quad viewport_quad;
	view_viewport_to_quad(vw, &viewport_quad);

	SizeQuad draw_quad;
	ui_fill_size_quad_for_draw(&draw_quad, &g->styles_quad, &viewport_quad);

	int mesh_index = ui_fill_draw_mesh_index(ui, &draw_quad);
	FillMesh *mesh = &ui->layouts[vw->layout].meshes[mesh_index];
	UiFillBuffers *buffers = &ui->layouts[vw->layout].buffers[mesh_index];

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
			(float) g->size_quad.size.x,
			(float) g->size_quad.size.y);

	glUniform1f(
			ui->uniforms.styleOffset,
			0.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ui->grid_styles_texture.texture);
	glUniform1i(ui->uniforms.gridStyles, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ui->fill_colors_texture.texture);
	glUniform1i(ui->uniforms.fillColors, 1);

	vec2 draw_offset = vec2_from_ivec(draw_quad.min);
	view_update_matrix(vw, draw_offset);

	glUniformMatrix4fv(
			ui->uniforms.viewMatrix,
			1,
			GL_FALSE,
			(GLfloat*) &vw->view_matrix.m[0][0]);

	// Blend + Fill color

	u8 *empty_fill_color;

	if (
			ui->blend_enabled &&
			vw->layout_independent_scale >= UI_FILL_ALPHA_SCALE) {
		empty_fill_color = (u8 *) UI_FILL_COLORS;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	} else {
		empty_fill_color = (u8 *) UI_EMPTY_FILL_COLOR_NO_BLEND;
		glDisable(GL_BLEND);
	}

	glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0,
			0,
			1,
			1,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			empty_fill_color);

	// Instance buffer

	InstanceMesh *imesh = &ui->instance_mesh;

	int num_instances = (
		((draw_quad.size.y - 1) / UI_FILL_MESH_MAX_SIZE + 1) *
		((draw_quad.size.x - 1) / UI_FILL_MESH_MAX_SIZE + 1)
	);
	instance_mesh_resize(imesh, num_instances);

	ivec2 h;
	int i = 0;
	ivec2 draw_quad_min_offset = ivec2_sub(draw_quad.min, g->size_quad.min);

	for (h.y = 0; h.y < draw_quad.size.y; h.y += UI_FILL_MESH_MAX_SIZE) {
		for (h.x = 0; h.x < draw_quad.size.x; h.x += UI_FILL_MESH_MAX_SIZE) {
			InstanceMeshVertex *vx = &imesh->vertices[i];

			vx->positionOffset = vec2_from_ivec(h);

			ivec2 grid_offset = ivec2_add(h, draw_quad_min_offset);
			vx->gridPositionOffset.x = grid_offset.x;
			vx->gridPositionOffset.y = grid_offset.y;
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
			GL_INT,
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
