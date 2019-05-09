#pragma once

#ifndef ___GameClones_MemoryPuzzle_Game_h___
#define ___GameClones_MemoryPuzzle_Game_h___

#include "TextureManager.h"
#include "MemoryPanel.h"
#include "JSON.h"
#include "ListUtils.h"

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

namespace GameClones {
	namespace MemoryPuzzle {
		class Game {
			TextureManager m_textureManager;

			std::vector<MemoryPanel> m_panels;

			int m_width;
			int m_height;

			int m_numColumns;

			MemoryPanel* m_heldPanel1;
			MemoryPanel* m_heldPanel2;

			float m_flipBackTimer;
			bool m_waitingForFlipBack;

			int m_score;
			int m_numPairs;

			sf::Font m_font;
			sf::Text m_text;

			sf::RectangleShape m_replayButtonShape;
			sf::FloatRect m_replayButtonRect;
			bool m_isReplayButtonHighlighted;

		public:
			Game(int width, int height) : m_width(width), m_height(height), m_numColumns(6), m_heldPanel1(nullptr), m_flipBackTimer(0.0f), m_waitingForFlipBack(false), m_score(0), m_numPairs(0), m_isReplayButtonHighlighted(false) {}
			~Game() {}

			bool LoadConfiguration(std::string configFilePath) {
				JSON::JSONParser jsonParser(configFilePath);
				JSON::JSONObject* jsonObj = jsonParser.Parse();
				
				JSON::JSONArray* panelsArray = JSON::AsArray(jsonObj->Get("panels"));
				if (panelsArray != nullptr) {
					m_numPairs = panelsArray->Size();

					for (int i = 0; i < panelsArray->Size(); ++i) {
						JSON::JSONObject* panelObj = JSON::AsObject(panelsArray->Get(i));
						const std::string& imagePath = JSON::AsString(panelObj->Get("image"))->GetString();

						const sf::Texture* texture = m_textureManager.GetTexture(imagePath);
						sf::Sprite sprite(*texture);

						JSON::JSONArray* subRectArray = JSON::AsArray(panelObj->Get("subRect"));
						if (subRectArray != nullptr) {
							sf::IntRect subRect(JSON::AsNumber(subRectArray->Get(0))->AsInt32(),
								JSON::AsNumber(subRectArray->Get(1))->AsInt32(),
								JSON::AsNumber(subRectArray->Get(2))->AsInt32(),
								JSON::AsNumber(subRectArray->Get(3))->AsInt32());
							sprite.setTextureRect(subRect);
						}

						m_panels.emplace_back(i, sprite);
						m_panels.emplace_back(i, sprite);
					}
				}

				return true;
			}

			void GenerateBoard() {
				GameClones::ListUtils::Shuffle(m_panels);
				m_score = 0;
				for (int i = 0; i < m_panels.size(); ++i) {
					m_panels[i].SetIsFacingUp(false);
				}

				int panelSize = (m_width - 10.0f * m_numColumns) / m_numColumns;
				for (int i = 0; i < m_panels.size(); ++i) {
					int row = i / m_numColumns;
					m_panels[i].SetPosition(5.0f + (i % m_numColumns) * (panelSize + 10.0f), 90 + row * (10.0f + panelSize));
					m_panels[i].SetSize(panelSize, panelSize);
				}
			}

			void Init() {
				m_font.loadFromFile("Resources/Fonts/DS-Digital/DS-DIGI.TTF");

				m_text.setFont(m_font);
				
				m_replayButtonRect.width = 200;
				m_replayButtonRect.height = 50;
				m_replayButtonRect.left = (m_width - 200) / 2;
				m_replayButtonRect.top = 525;
				m_replayButtonShape.setPosition(m_replayButtonRect.left, m_replayButtonRect.top);
				m_replayButtonShape.setSize(sf::Vector2f(m_replayButtonRect.width, m_replayButtonRect.height));
				m_replayButtonShape.setFillColor(sf::Color::Transparent);
				m_replayButtonShape.setOutlineThickness(5.0f);
			}

			void Draw(sf::RenderTarget& renderTarget) {
				std::string remainingPairsString = "Remaining: " + std::to_string(m_numPairs - m_score);
				m_text.setString(remainingPairsString);
				m_text.setPosition(20.0f, 20.0f);
				m_text.setFillColor(sf::Color::White);
				renderTarget.draw(m_text);

				for (auto& panel : m_panels) {
					panel.Draw(renderTarget);
				}

				if (m_score == m_numPairs) {
					renderTarget.draw(m_replayButtonShape);
					m_text.setString("Play Again");
					m_text.setPosition(m_replayButtonRect.left - m_text.getLocalBounds().left + (m_replayButtonRect.width - m_text.getLocalBounds().width) / 2, m_replayButtonRect.top - m_text.getLocalBounds().top + (m_replayButtonRect.height - m_text.getGlobalBounds().height) / 2);

					m_replayButtonShape.setFillColor(m_isReplayButtonHighlighted ? sf::Color::White : sf::Color::Transparent);
					m_text.setFillColor(m_isReplayButtonHighlighted ? sf::Color::Black : sf::Color::White);
					renderTarget.draw(m_text);
				}
			}

			void Update(float deltaTime) {
				for (auto& panel : m_panels) {
					panel.Update(deltaTime);
				}

				if (m_flipBackTimer > 0.0f) {
					m_flipBackTimer -= deltaTime;
					if (m_flipBackTimer <= 0.0f) {
						m_heldPanel1->Flip();
						m_heldPanel2->Flip();

						m_waitingForFlipBack = true;
					}
				}
				else if (m_waitingForFlipBack && !m_heldPanel1->IsFlipping() && !m_heldPanel2->IsFlipping()) {
					m_heldPanel1 = nullptr;
					m_heldPanel2 = nullptr;
					m_waitingForFlipBack = false;
				}
			}

			void HandleEvent(sf::Event& event) {
				if (event.type == sf::Event::MouseButtonPressed &&
					event.mouseButton.button == sf::Mouse::Left) {
					if (m_score == m_numPairs) {
						if (m_replayButtonRect.contains(event.mouseButton.x, event.mouseButton.y)) {
							GenerateBoard();
						}
					}
					else if (!m_waitingForFlipBack) {
						for (size_t i = 0; i < m_panels.size(); ++i) {
							if (m_panels[i].GetRect().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
								if (!m_panels[i].IsFacingUp() && !m_panels[i].IsFlipping() && (m_heldPanel2 == nullptr)) {
									m_panels[i].Flip();

									if (m_heldPanel1 == nullptr) {
										m_heldPanel1 = &m_panels[i];
									}
									else if (m_heldPanel1->GetId() != m_panels[i].GetId()) {
										m_flipBackTimer = 1.0f;
										m_heldPanel2 = &m_panels[i];
									}
									else {
										m_heldPanel1 = nullptr;
										m_heldPanel2 = nullptr;

										++m_score;
										if (m_score == m_numPairs) {
											std::printf("You win!\n");
										}
									}
								}

								break;
							}
						}
					}
				}
				else if (event.type == sf::Event::MouseMoved) {
					m_isReplayButtonHighlighted = m_replayButtonRect.contains(event.mouseMove.x, event.mouseMove.y);
					if (!m_isReplayButtonHighlighted) {
						for (int i = 0; i < m_panels.size(); ++i) {
							if (!m_panels[i].IsFlipping() && m_panels[i].GetRect().contains(event.mouseMove.x, event.mouseMove.y)) {
								m_panels[i].Highlight();
							}
							else {
								m_panels[i].Unhighlight();
							}
						}
					}
				}
			}
		};
	}
}

#endif // ___GameClones_MemoryPuzzle_Game_h___
