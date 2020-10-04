
#include "Renderer2D.h"
#include <GL/glew.h>
#include "Buffer.h"
#include "Debug.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "cameras/OrbitCamera.h"
#include "VertexArray.h"
#include <array>


namespace Shado {

    Renderer2D* Renderer2D::singleton = new Renderer2D();

    Renderer2D::Renderer2D()
        : m_ClearColor(0.0, 0.0, 0.0)
    {
    }

    Renderer2D::~Renderer2D() {
        delete flatColorShader;
        delete textureShader;
        delete m_Cache;
    }

    void Renderer2D::clear() {

        // Check if the Renderer2D has been initialized or not
        singleton->checkForInit();

        glClearColor(singleton->m_ClearColor.red(),
            singleton->m_ClearColor.green(),
            singleton->m_ClearColor.blue(),
            singleton->m_ClearColor.alpha());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        singleton->m_DrawCallsPerFrame = 0;
    }

    void Renderer2D::init() {

        if (singleton->m_hasInitialized)
            Debug::warn("You only have to initialize the Renderer2D once!");

        singleton->flatColorShader = new Shader(FLAT_COLOR_SHADER_PATH);
        singleton->textureShader = new Shader(TEXTURE2D_SHADER_PATH);

        // Initialize Common stuff
        singleton->m_Cache = new Renderer2DDataCache();
        singleton->initializeCache();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        singleton->m_hasInitialized = true;
    }

    void Renderer2D::setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        glViewport(x, y, width, height);
    }

    void Renderer2D::setClearColor(const Color& color) {
        singleton->m_ClearColor = color;
    }

    void Renderer2D::beginScene(const Camera& camera) {
        //singleton->m_Camera = camera;

        singleton->flatColorShader->bind();
        singleton->flatColorShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

        singleton->textureShader->bind();
        singleton->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
    }

    void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec3& rotation, const Color& c) {

        // Check if the Renderer2D has been initialized or not
        singleton->checkForInit();

        auto rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        auto rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 1.0f));
        auto rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        auto transform = glm::translate(glm::mat4(1.0f), position) * rotX * rotY * rotZ * glm::scale(glm::mat4(1.0f), { dimensions, 0.0 });

        singleton->flatColorShader->bind();     // Shader should always be bound before setting the uniforms
        //singleton->flatColorShader->setMat4("u_ViewProjection", singleton->m_Camera.getViewProjectionMatrix());
        singleton->flatColorShader->setMat4("u_Transform", transform);
        singleton->flatColorShader->setFloat4("u_Color", (glm::vec4)c);

        auto* array = singleton->m_Cache->quadFlatColorVAO;
        array->bind();

        {
            glCall(glDrawElements(GL_TRIANGLES, array->getIndexBuffers()->getCount(), GL_UNSIGNED_INT, nullptr));
            singleton->collectStats();
        }

        array->unBind();
    }

    void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec3& rotation,
        Shader& shader) {

        // Check if the Renderer2D has been initialized or not
        singleton->checkForInit();

        auto rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        auto rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 1.0f));
        auto rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        auto transform = glm::translate(glm::mat4(1.0f), position) * rotX * rotY * rotZ * glm::scale(glm::mat4(1.0f), {dimensions, 0.0f});

        shader.bind();     // Shader should always be bound before setting the uniforms
        //shader.setMat4("u_ViewProjection", singleton->m_Camera.getViewProjectionMatrix());
        shader.setMat4("u_Transform", transform);

        auto* array = singleton->m_Cache->quadFlatColorVAO;
        array->bind();

        {
            glCall(glDrawElements(GL_TRIANGLES, array->getIndexBuffers()->getCount(), GL_UNSIGNED_INT, nullptr));
            singleton->collectStats();
        }

        array->unBind();
    }

    void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec3& rotation, const Texture2D& texture) {

        // Check if the Renderer2D has been initialized or not
        singleton->checkForInit();

        auto rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        auto rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 1.0f));
        auto rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        auto translate = glm::translate(glm::mat4(1.0f), position) * rotX * rotY * rotZ * glm::scale(glm::mat4(1.0f), {dimensions, 0.0f});

        singleton->textureShader->bind();
        singleton->textureShader->setInt("u_Texture", 0);
        // singleton->textureShader->setMat4("u_ViewProjection", singleton->m_Camera.getViewProjectionMatrix());
        singleton->textureShader->setMat4("u_Transform", translate);
        texture.bind();

        // Here we are setting Position Vertex and Texture Coordinates
        auto& array = singleton->m_Cache->quadTextureVAO;
        array->bind();

        {
            glCall(glDrawElements(GL_TRIANGLES, array->getIndexBuffers()->getCount(), GL_UNSIGNED_INT, nullptr));
            singleton->collectStats();
        }
    }

    void Renderer2D::drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& rotation,
        const Color& c) {

        // Check if the Renderer2D has been initialized or not
        singleton->checkForInit();

        auto rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        auto rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 1.0f));
        auto rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        auto translate = rotX * rotY * rotZ;

        singleton->flatColorShader->bind();
        singleton->flatColorShader->setFloat4("u_Color", (glm::vec4)c);
        singleton->textureShader->setMat4("u_Transform", translate);

        float positions[] = {
            p1.x, p1.y, p1.z,
            p2.x, p2.y, p2.z,
            p3.x, p3.y, p3.z
        };

        unsigned int indicies[] = {
            0, 1, 2
        };

        auto vertexBuffer = VertexBuffer::create(positions, sizeof(positions));
        vertexBuffer->setLayout({
            {ShaderDataType::Float3, "a_Position"}
            });
        vertexBuffer->bind();

        auto indexBuffer = IndexBuffer::create(indicies, sizeof(indicies));
        indexBuffer->bind();

        auto vertexArray = VertexArray::create();
        vertexArray->addVertexBuffer(vertexBuffer);
        vertexArray->setIndexBuffer(indexBuffer);
        vertexArray->bind();

        {
            glCall(glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffers()->getCount(), GL_UNSIGNED_INT, nullptr));
            singleton->collectStats();
        }

        vertexArray->unBind();
    }

    void Renderer2D::drawEllipse(const glm::vec3& position, const glm::vec2& radiuses, const glm::vec3& rotation, const Color& color) {

        // Check if the Renderer2D has been initialized or not
        singleton->checkForInit();
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(position.x, position.y, position.z);
        static const int circle_points = 100;
        static const float angle = 2.0f * 3.1416f / circle_points;

        // this code (mostly) copied from question:
        glBegin(GL_POLYGON);
        double angle1 = 0.0;
        glVertex2d(radiuses.x * cos(0.0), radiuses.y * sin(0.0));
        int i;
        for (i = 0; i < circle_points; i++)
        {
            glVertex2d(radiuses.x * cos(angle1), radiuses.y * sin(angle1));
            angle1 += angle;
        }
        glEnd();
        glPopMatrix();

    }

    void Renderer2D::drawLine(const glm::vec3& p0, const glm::vec3& p1, Color color) {

        // Check if the Renderer2D has been initialized or not
        singleton->checkForInit();

        auto transform = glm::mat4(1.0f);

        singleton->flatColorShader->bind();     // Shader should always be bound before setting the uniforms
        //singleton->flatColorShader->setMat4("u_ViewProjection", singleton->m_Camera.getViewProjectionMatrix());
        singleton->flatColorShader->setMat4("u_Transform", transform);
        singleton->flatColorShader->setFloat4("u_Color", (glm::vec4)color);

        float positions[] = {
            p0.x, p0.y, p0.z,
            p1.x, p1.y, p1.z
        };

        unsigned int indices[] = { 0, 1 };


        auto buffer = VertexBuffer::create(positions, sizeof(positions));
        buffer->setLayout({
            {ShaderDataType::Float3, "a_Position"}
            });

        auto indexbuffer = IndexBuffer::create(indices, sizeof(indices));

        auto vao = VertexArray::create();
        vao->setIndexBuffer(indexbuffer);
        vao->addVertexBuffer(buffer);
        vao->bind();

        {
            glCall(glDrawElements(GL_LINES, vao->getIndexBuffers()->getCount(), GL_UNSIGNED_INT, nullptr));
            singleton->collectStats();
        }
    }

	void Renderer2D::drawModel(const Object3D& object, const glm::vec3& position) {

        // Check if the Renderer2D has been initialized or not
        singleton->checkForInit();

        auto transform = glm::translate(glm::mat4(1.0f), position); //* glm::scale(glm::mat4(1.0f), { dimensions, 0.0f });;

        singleton->flatColorShader->bind();     // Shader should always be bound before setting the uniforms
        //singleton->flatColorShader->setMat4("u_ViewProjection", singleton->m_Camera.getViewProjectionMatrix());
        singleton->flatColorShader->setMat4("u_Transform", transform);
        singleton->flatColorShader->setFloat4("u_Color", {1, 1, 1, 1});

        object.getVertexArray()->bind();

        {
            glCall(glDrawElements(GL_TRIANGLES, object.getVertexArray()->getIndexBuffers()->getCount(), GL_UNSIGNED_INT, nullptr));
            singleton->collectStats();
        }
    	
	}

	void Renderer2D::draw(const VertexArray& va, const Shader& shader) {

        va.bind();
        shader.bind();

        {
            glCall(glDrawElements(GL_TRIANGLES, va.getIndexBuffers()->getCount(), GL_UNSIGNED_INT, nullptr));
            singleton->collectStats();
        }
    }

    void Renderer2D::draw(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader) {
        draw(*va, *shader);
    }

    unsigned Renderer2D::drawCallsPerFrame() {
        return singleton->m_DrawCallsPerFrame;
    }

    unsigned Renderer2D::totalDrawCalls() {
        return singleton->m_TotalDrawCalls;
    }

    void Renderer2D::collectStats() {
        m_TotalDrawCalls++;
        m_DrawCallsPerFrame++;
    }

    void Renderer2D::checkForInit() const {
        if (!m_hasInitialized) {
            Debug::Assert(false, "The Renderer2D was not initialized. Make sure you call 'Renderer2D::init()' before clear or draw");
        }
    }

    void Renderer2D::initializeCache() {

        float floatColorPositions[] = {
            -0.5f, 0.5f, 0.0f,    // Left top         ID: 0
            -0.5f, -0.5f, 0.0f, // Left bottom      ID: 1
            0.5f, -0.5f, 0.0f,  // Right bottom     ID: 2
            0.5f, 0.5f, 0.0f    // Right left       ID: 3 
        };                      // bottom right corner

        // Here we are setting Position Vertex and Texture Coordinates
        float texturedSquareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        unsigned int indices[] = {  // Left bottom triangle
            0, 1, 2,                // Right top triangle
            2, 3, 0 };              // second triangle (bottom left - top right - bottom right)

        // Create Quad vertex buffer
        auto flatColorVertexbuffer = VertexBuffer::create(floatColorPositions, sizeof(floatColorPositions));
        flatColorVertexbuffer->setLayout({
            { ShaderDataType::Float3, "a_Position" }
            });

        // Create vertex buffer
        auto texturedQuadbuffer = VertexBuffer::create(texturedSquareVertices, sizeof(texturedSquareVertices));
        texturedQuadbuffer->setLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
            });

        auto indexBffer = IndexBuffer::create(indices, sizeof(indices));

        auto* quadFlatColorVAO = new VertexArray();
        quadFlatColorVAO->addVertexBuffer(flatColorVertexbuffer);
        quadFlatColorVAO->setIndexBuffer(indexBffer);
        quadFlatColorVAO->bind();

        auto* quadTextureVAO = new VertexArray();
        quadTextureVAO->addVertexBuffer(texturedQuadbuffer);
        quadTextureVAO->setIndexBuffer(indexBffer);
        quadTextureVAO->bind();

        m_Cache->quadFlatColorVAO = quadFlatColorVAO;
        m_Cache->quadTextureVAO = quadTextureVAO;
    }
}
