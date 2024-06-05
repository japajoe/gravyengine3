#ifndef GRAVYENGINE_SHADER_HPP
#define GRAVYENGINE_SHADER_HPP

#include "../Core/Object.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>

namespace GravyEngine
{
    enum class ShaderType
    {
        Vertex,
        Geometry,
        Fragment,
        Program
    };

    class Shader : public Object
    {
    friend class Graphics;
    public:
        Shader();
        Shader(const std::string &vertexSource, const std::string &fragmentSource);
        Shader(const std::string &vertexSource, const std::string &fragmentSource, const std::string &geometrySource);
        uint32_t GetId() const;
        void Use();
        void Delete();
        static Shader *Add(const std::string &name, const Shader &shader);
        static void Remove(const std::string &name);
        static Shader *Find(const std::string &name);
        void SetMat2(const std::string &name, const float *value, bool transpose = false);
        void SetMat3(const std::string &name, const float *value, bool transpose = false);
        void SetMat4(const std::string &name, const float *value, bool transpose = false);
        void SetFloat(const std::string &name, float value);
        void SetFloat2(const std::string &name, const float *value);
        void SetFloat3(const std::string &name, const float *value);
        void SetFloat4(const std::string &name, const float *value);
        void SetInt(const std::string &name, int32_t value);
        void SetBool(const std::string &name, bool value);
        void SetMat2(int32_t location, const float *value, bool transpose = false);
        void SetMat3(int32_t location, const float *value, bool transpose = false);
        void SetMat4(int32_t location, const float *value, bool transpose = false);
        void SetFloat(int32_t location, float value);
        void SetFloat2(int32_t location, const float *value);
        void SetFloat3(int32_t location, const float *value);
        void SetFloat4(int32_t location, const float *value);
        void SetInt(int32_t location, int32_t value);
        void SetBool(int32_t location, bool value);
        static void AddIncludeFile(const std::string &name, const std::string &code);
    private:
        uint32_t id;
        static std::unordered_map<std::string,Shader> shaders;
        static std::unordered_map<std::string, std::string> includesMap;
        static bool CheckShader(uint32_t shader, ShaderType type);
        static std::string AddIncludes(const std::string &shaderSource);
    };
};

#endif