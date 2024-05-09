#include <glm/glm.hpp>

class OrthographicCamera 
{
    public:

        OrthographicCamera(const glm::vec3 position, uint32_t width, uint32_t height);

        ~OrthographicCamera();

        // Getters
        //
        inline const glm::mat4& GetProjectionMatrix() const { return mProjection; }

        inline const glm::mat4& GetViewMatrix() const { return mView; }

        inline const glm::vec3& GetPosition() const { return mPosition; }

        // Setters
        //
        void SetProjectionMatrix(const glm::mat4& proj) { mProjection = proj; }

        void SetViewMatrix(const glm::mat4& view) { mView = view; }

        void SetPosition(const glm::vec3& pos) { mPosition = pos; }
    
    private:

        void RecalculateTransforms();

        glm::mat4 mProjection = glm::mat4(1.0f);

        glm::mat4 mView = glm::mat4(1.0f);

        glm::vec3 mPosition{0.0f};
};