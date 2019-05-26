#pragma once

#ifndef ___Common_Scene_h___
#define ___Common_Scene_h___

#include <vector>

#include "Drawable.h"
#include "Matrix.h"

namespace Common {
	class Scene {
		Matrix m_transformationMatrix;

		std::vector<Drawable*> m_drawables;
		std::vector<Scene*> m_children;

	public:
		Scene() {}
		~Scene() {
			m_drawables.clear();

			for (size_t i = 0; i < m_children.size(); ++i) {
				delete m_children[i];
			}
			m_children.clear();
		}

		void AddChild(Scene* scene) {
			if (nullptr != scene) {
				m_children.push_back(scene);
			}
		}

		const size_t& GetChildCount() const {
			return m_children.size();
		}

		Scene* GetChild(size_t index) {
			return m_children[index];
		}

		const Scene* GetChild(size_t index) const {
			return m_children[index];
		}

		const size_t GetDrawableCount() const {
			return m_drawables.size();
		}

		void AddDrawable(Drawable* drawable) {
			if (nullptr != drawable) {
				m_drawables.push_back(drawable);
			}
		}

		Drawable* GetDrawable(size_t index) {
			return m_drawables[index];
		}

		const Drawable* GetDrawable(size_t index) const {
			return m_drawables[index];
		}

		void Draw(const Matrix& transformationMatrix) {
			for (size_t i = 0; i < m_drawables.size(); ++i) {
				if (nullptr != m_drawables[i]) {
					m_drawables[i]->Draw(transformationMatrix);
				}
			}

			for (size_t i = 0; i < m_children.size(); ++i) {
				m_children[i]->Draw(transformationMatrix * m_transformationMatrix);
			}
		}
	};
}

#endif // ___Common_Scene_h___
