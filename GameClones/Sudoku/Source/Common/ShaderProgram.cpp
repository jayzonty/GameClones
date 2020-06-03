#include "ShaderProgram.h"

#include "FileUtils.h"

#include <sstream>

namespace Common {
	ShaderProgram::ShaderProgram() : m_program(0) {

	}

	ShaderProgram::~ShaderProgram() {

	}

	bool ShaderProgram::Load(const char* vertexShaderPath, const char* fragmentShaderPath) {
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		
		std::string shaderCode;
		FileUtils::ReadFile(vertexShaderPath, shaderCode);
		if (CompileShader(vertexShader, shaderCode.c_str())) {
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			shaderCode.clear();
			FileUtils::ReadFile(fragmentShaderPath, shaderCode);
			if (CompileShader(fragmentShader, shaderCode.c_str())) {
				m_program = glCreateProgram();

				glAttachShader(m_program, vertexShader);
				glAttachShader(m_program, fragmentShader);

				glLinkProgram(m_program);

				GLint success = GL_FALSE;
				glGetProgramiv(m_program, GL_LINK_STATUS, &success);
				
				glDetachShader(m_program, vertexShader);
				glDetachShader(m_program, fragmentShader);

				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);

				return true;
			}
			else {
				m_lastErrorMessage = std::string(vertexShaderPath) + ":\n"
					+ m_lastErrorMessage;
			}

			glDeleteShader(fragmentShader);
		}
		else {
			m_lastErrorMessage = std::string(vertexShaderPath) + ":\n"
				+ m_lastErrorMessage;
		}

		glDeleteShader(vertexShader);

		return false;
	}

	bool ShaderProgram::IsLoaded() const {
		return (m_program > 0);
	}

	void ShaderProgram::Use() {
		if (IsLoaded()) {
			glUseProgram(m_program);
		}
	}

	void ShaderProgram::Unuse() {
		glUseProgram(0);
	}

	const std::string& ShaderProgram::GetLastErrorMessage() const {
		return m_lastErrorMessage;
	}

	void ShaderProgram::SetUniform1i(const char* uniformName, int val) {
		GLint location = glGetUniformLocation(m_program, uniformName);
		glUniform1i(location, val);
	}

	void ShaderProgram::SetUniform3f(const char* uniformName, float val1, float val2, float val3) {
		GLint location = glGetUniformLocation(m_program, uniformName);
		glUniform3f(location, val1, val2, val3);
	}

	void ShaderProgram::SetUniform4f(const char* uniformName,
		float val1, float val2, float val3, float val4) {
		GLint location = glGetUniformLocation(m_program, uniformName);
		glUniform4f(location, val1, val2, val3, val4);
	}

	void ShaderProgram::SetUniformMatrix4f(const char* uniformName, const float mat[16]) {
		GLint location = glGetUniformLocation(m_program, uniformName);
		glUniformMatrix4fv(location, 1, GL_FALSE, mat);
	}

	bool ShaderProgram::CompileShader(const GLuint& shader, const char* shaderSource) {
		m_lastErrorMessage.clear();

		glShaderSource(shader, 1, &shaderSource, nullptr);
		glCompileShader(shader);

		GLint result = GL_FALSE;
		GLint infoLogLength = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			m_lastErrorMessage.resize(infoLogLength);
			glGetShaderInfoLog(shader, infoLogLength, nullptr, &m_lastErrorMessage[0]);
		}

		return (result == GL_TRUE);
	}
}