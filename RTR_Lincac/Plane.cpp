#include"Plane.h"

Plane::Plane():VAO(-1) {
	planeshader = std::make_shared<Shader>("shader/gridplane/grid.vs", "shader/gridplane/grid.fs", "shader/gridplane/grid.gs");
	init();
}

Plane::~Plane() {
	glDeleteVertexArrays(1, &VAO);
}

void Plane::render() {
	glm::mat4 model(1.0);
	planeshader->use();
	planeshader->setMat4("MVP",projection * view * model);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void Plane::init() {
	float planeVertices[] = { -10.0f,-1.0f,-10.0f };
	planeVertices[0] = center.x - 5.0f;
	planeVertices[1] = _PlaneY;
	planeVertices[2] = center.z - 5.0f;

	GLuint planeVBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Plane::update() {
	float planeVertices[] = {-10.0f,-1.0f,-10.0f};
	planeVertices[0] = center.x - 5.0f;
	planeVertices[1] = _PlaneY;
	planeVertices[2] = center.z - 5.0f;

	GLuint planeVBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}