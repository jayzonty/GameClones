#ifndef ___Common_ShaderProgram_h___
#define ___Common_ShaderProgram_h___

#include <GL/glew.h>

#include <string>

namespace Common
{
	class ShaderProgram
	{
		GLuint m_program;
		std::string m_lastErrorMessage;

	public:
		ShaderProgram();
		~ShaderProgram();

		bool Load(const char* vertexShaderPath, const char* fragmentShaderPath);
		bool IsLoaded() const;
		
		void Use();
		void Unuse();

		const std::string& GetLastErrorMessage() const;

		void SetUniform1i(const char* uniformName, int val);
		void SetUniform3f(const char* uniformName, float val1, float val2, float val3);
		void SetUniform4f(const char* uniformName, float val1, float val2, float val3, float val4);
		void SetUniformMatrix4f(const char* uniformName, const float mat[16]);

	private:
		bool CompileShader(const GLuint& shader, const char* shaderSource);
	};
}

#endif // ___Common_ShaderProgram_h___
