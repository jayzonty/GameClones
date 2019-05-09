#pragma once

#ifndef GameClones_TextureManager_h___
#define GameClones_TextureManager_h___

#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace GameClones {
	class TextureManager {
		std::unordered_map<std::string, sf::Texture*> m_textures;

	public:
		TextureManager() {}
		~TextureManager() {
			for (auto& pair : m_textures) {
				delete pair.second;
			}
			m_textures.clear();
		}

		bool LoadTexture(std::string textureFilePath) {
			sf::Texture* texture = new sf::Texture();
			if (texture->loadFromFile(textureFilePath)) {
				m_textures.insert({ textureFilePath, texture });
				return true;
			}

			return false;
		}

		const sf::Texture* GetTexture(std::string textureFilePath, bool doLoad = true) {
			if (m_textures.find(textureFilePath) == m_textures.end()) {
				if (doLoad && LoadTexture(textureFilePath)) {
					return m_textures[textureFilePath];
				}
				else {
					return nullptr;
				}
			}

			return m_textures[textureFilePath];
		}
	};
}

#endif //GameClones_TextureManager_h___
